
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char uint8_t;
typedef unsigned int size_t;
//#include <stdint.h>

// copy-paste some library functions until we get a basic libc working
void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size) {
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;
    for (size_t i = 0; i < size; i++) {
        dst[i] = src[i];
    }
    return dstptr;
}

int memcmp(const void* aptr, const void* bptr, size_t size) {
    const unsigned char* a = (const unsigned char*) aptr;
    const unsigned char* b = (const unsigned char*) bptr;
    for (size_t i = 0; i < size; i++) {
        if (a[i] < b[i])
            return -1;
        else if (b[i] < a[i])
            return 1;
    }
    return 0;
}

#define NULL ((void*)0)


static void outd(uint16_t port, uint32_t val)
{
    asm volatile ( "out %0, %1" : : "a"(val), "Nd"(port) );
}

static void outp(uint16_t port, volatile void* valp)
{
    uint32_t val = (uint32_t) valp;
    asm volatile ( "out %0, %1" : : "a"(val), "Nd"(port) );
}

static void cli(){
    asm volatile ("cli");
}
static void sti(){
    asm volatile ("sti");
}

static uint32_t ind(uint16_t port)
{
    uint32_t ret;
    asm volatile ( "in %1, %0"
    : "=a"(ret)
    : "Nd"(port) );
    return ret;
}



#define PACKED __attribute__((__packed__))

typedef uint8_t address_t[32];



//Exit ABIs

#define PORT_EXIT 0xF0
#define PORT_EXIT_RESULT 0xF1

typedef struct {
    uint32_t code;
    uint32_t resultAddress;
    uint32_t resultSize;
    
}PACKED ExitResult_ABI ;

void qtum_exit(int code){
    outd(0xF0, code); //exit with only error code
}

void qtum_exit_result(int code, void* result, size_t size){
    ExitResult_ABI res;
    res.code = code;
    res.resultAddress = (uint32_t) result;
    res.resultSize = size;
    outp(0xF1, &res);
}

void assert(int condition){
    if(condition){
        qtum_exit(-1);
    }
}

//Memory management ABIs

#define PORT_MEMORY_MAN 0xF2

// Allocate new memory
#define ABI_ALLOCATE_MEMORY     1
// Free allocated memory
#define ABI_FREE_MEMORY         2
// Check if a specified chunk of memory in the allocable space is accessible
#define ABI_CHECK_CHUNK         3
// Get total amount of memory allocated
#define ABI_MEMORY_SIZE         4

#define MIN_ALLOC_SPACE 0x40000000
#define MAX_ALLOC_SPACE 0x80000000

typedef volatile struct {
    uint8_t method;
    uint32_t address; //address to allocate or free memory at
    uint32_t memorySize; //only used for allocate_memory
    uint32_t volatile returnError; //filled with error code after return (0 for success)
}PACKED ManageMemory_ABI;


void* allocateMemory(uint32_t address, size_t size){
    assert(address >= MIN_ALLOC_SPACE && address+size < MAX_ALLOC_SPACE);
    if(size == 0){
        return NULL;
    }
    ManageMemory_ABI abi;
    abi.method = ABI_ALLOCATE_MEMORY;
    abi.address = address;
    abi.memorySize = size;
    outp(PORT_MEMORY_MAN, &abi);
    if(abi.returnError == 0){
        return (void*) address;
    }else{
        return NULL;
    }
}

int freeMemory(void* block){
    uint32_t address = (uint32_t) block;
    assert(address >= MIN_ALLOC_SPACE && address < MAX_ALLOC_SPACE);
    ManageMemory_ABI abi;
    abi.method = ABI_FREE_MEMORY;
    abi.address = (uint32_t) address;
    abi.memorySize = 0;
    outp(PORT_MEMORY_MAN, &abi);
    return abi.returnError;    
}

static volatile size_t autoAlloc_currentAddress=0;
void* autoAlloc(size_t size){
    cli();
    size_t c = autoAlloc_currentAddress;
    if(autoAlloc_currentAddress + size > MAX_ALLOC_SPACE){
        return NULL;
    }
    autoAlloc_currentAddress += size + 16; // add 16 to add a gap between each block to catch buffer overruns
    sti(); //TODO we need to restore previous IF value, not set it.. they might've called with with CLI
    if(allocateMemory(autoAlloc_currentAddress, size)){
        return NULL;
    }
    return (void*) c;
}

//returns 0 for whole block accessible RW, 1 for accessible read-only, else error (or if memory to be checked is outside allotable space)
int allotedMemoryAccessible(void* block, size_t size){
    uint32_t address = (uint32_t) block;
    if(address < MIN_ALLOC_SPACE || address+size > MAX_ALLOC_SPACE){
        return -1;
    }
    if(size == 0){
        size = 1;
    }
    ManageMemory_ABI abi;
    abi.method = ABI_CHECK_CHUNK;
    abi.address = address;
    abi.memorySize = size;
    outp(PORT_MEMORY_MAN, &abi);
    return abi.returnError;
}

size_t totalAllotedMemory(){
    ManageMemory_ABI abi;
    abi.method = ABI_MEMORY_SIZE;
    abi.address = 0;
    abi.memorySize = 0;
    outp(PORT_MEMORY_MAN, &abi);
    return abi.returnError;
}


// Transaction Info

#define PORT_SENDER 0x01

void getSender(address_t* buf){
    outp(PORT_SENDER, buf);
}

// Determines the size of the input data
#define INPORT_INPUT_DATA_SIZE 0x10
// "Mounts" the data as read-only memory (no allocation required and no allocation cost)
#define PORT_INPUT_DATA_MOUNT 0X11
// Copies the input data into the chosen address (must be allocated)
#define PORT_INPUT_DATA_COPY 0x12

typedef volatile struct {

    uint32_t address;
    uint32_t maxSize;
    uint32_t volatile returnError;

}PACKED InputDataMemory_ABI;

size_t getInputDataSize(){
    return ind(INPORT_INPUT_DATA_SIZE);
}

void* mountInputData(uint32_t address, size_t maxSize){
    assert(address >= MIN_ALLOC_SPACE && address < MAX_ALLOC_SPACE);
    if(maxSize == 0) maxSize = 0x10000; //1MB is default
    InputDataMemory_ABI abi;
    abi.address = address;
    abi.maxSize = maxSize;
    outp(PORT_INPUT_DATA_MOUNT, &abi);
    if(abi.returnError != 0){
        return NULL;
    }
    return (void*) address;
}

void* copyInputData(void* address, size_t maxSize){
    if(maxSize == 0) maxSize = 0x10000; //1MB is default
    InputDataMemory_ABI abi;
    abi.address = (uint32_t) address;
    abi.maxSize = maxSize;
    outp(PORT_INPUT_DATA_COPY, &abi);
    if(abi.returnError != 0){
        return NULL;
    }
    return (void*) address;
}






struct ABI_MakeLog{
    uint16_t dataSize;
    uint32_t dataAddress;
    uint16_t topicCount;
    uint32_t topicsAddress;
}__attribute__((__packed__));



struct ABI_TransferValueSilent{
    uint64_t value;
    uint8_t address[32]; //normal addresses are only 20 bytes, but we use 32 bytes for future use
}__attribute__((__packed__));

struct ABI_TransferValue{
    uint64_t value;
    uint8_t address[32];
    uint16_t dataSize;
    uint32_t dataAddress;
}__attribute__((__packed__));

struct ABI_GetGasInfo{
    uint32_t gasLimit;
    uint32_t blockGasLimit;
    uint64_t gasPrice;
}__attribute__((__packed__));

struct ABI_AllocateMemory{
    uint32_t desiredAddress; //can not be less than 0x100000
    uint32_t size;
}__attribute__((__packed__));

struct ABI_PersistMemory{
    uint32_t address; //this will become storageAddress for restoring
    uint32_t size;
    uint32_t storageAddress;
}__attribute__((__packed__));

struct ABI_RestoreMemory{
    uint32_t desiredAddress;
    uint32_t size; //should be a multiple of 32
    uint32_t storageAddress;
}__attribute__((__packed__));


struct ABI_GetMsgInfo{
    uint8_t sender[32];
}__attribute__((__packed__));

struct ABI_Selfdestruct{
    uint8_t transferTo[32];
}__attribute__((__packed__));




extern char __binary_end;

void start() __attribute__((section(".text.start")));
void start(){
    while(1);
}
