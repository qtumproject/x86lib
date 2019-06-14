#ifndef X86LIB_C_H
#define X86LIB_C_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* qx86VM_t;

#define QX86ERR_NONE 0
#define QX86ERR_FAULT 1
#define QX86ERR_MEM 2
struct qx86Error{
    char* msg;
    int type; //if type != 0, then error needs to be destroyed after usage
    int code;
    void* extra;
};

//for dumb memory
struct qx86Memory{
    void* memory;
    uint32_t size;
    char* id;
    bool writeable;
};
typedef qx86Memory qx86Memory_t;

//returns 0 for success, otherwise 1
//reason not yet used
typedef int (*qx86CallMemoryOp)(uint32_t address, uint32_t size, void* buffer, int reason);
typedef int (*qx86CallMemoryCheck)(uint32_t address, uint32_t size);
struct qx86CallMemory{
    qx86CallMemoryOp read;
    qx86CallMemoryOp write;
    qx86CallMemoryCheck readable;
    qx86CallMemoryCheck writeable;
};
typedef qx86CallMemory qx86CallMemory_t;

typedef int (*qx86HandleInt)(int number, qx86VM_t* vm);
struct qx86Hypervisor{
    qx86HandleInt handler;
};
typedef qx86Hypervisor qx86Hypervisor_t;

//not yet used
#define QX86MEM_CODEFETCH 0
#define QX86MEM_DATA 1
#define QX86MEM_INTERNAL 2
#define QX86MEM_SYSCALL 3

int qx86CreateVM(qx86VM_t** vm);
int qx86DestroyVM(qx86VM_t* vm);
int qx86AddMemory(qx86VM_t* vm, qx86Memory_t* memory);
int qx86AddCallMemory(qx86VM_t* vm, qx86CallMemory_t* memory);
int qx86SetHypervisor(qx86VM_t* vm, qx86Hypervisor_t* hv);
uint32_t qx86GetReg32(qx86VM_t* vm, int reg);
void qx86SetReg32(qx86VM_t* vm, int reg, uint32_t val);
//returns 0 for success, returns 1 for error
int qx86ReadMemory(qx86VM_t* vm, uint32_t address, uint32_t size, void* buffer);
int qx86WriteMemory(qx86VM_t* vm, uint32_t address, uint32_t size, void* buffer);

//returns 0 for success, anything else for error (along with qx86Error being populated)
int qx86Execute(qx86VM_t* vm, int maxSteps, qx86Error* error);
int qx86DestroyError(qx86Error* error);
int qx86SetGasLimit(qx86VM_t* vm, uint64_t gasLimit);
int qx86AddGasUsed(qx86VM_t* vm, int64_t gas);
uint64_t qx86GetGasUsed(qx86VM_t* vm);
int qx86GasExceeded(qx86VM_t* vm);

#ifdef __cplusplus
}
#endif

#endif
