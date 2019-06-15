//C interface to x86Lib essentials. Designed for the mock_qtum Rust project

#include <x86lib.h>
#include <x86lib_c.h>

using namespace x86Lib;


//Wrapper classes

class WrapperMemory: public MemoryDevice{
    public:
    qx86Memory* wrapped;
    WrapperMemory(qx86Memory *mem) : wrapped(mem){
        
    }
    virtual void Read(uint32_t address,int count,void *buffer){
        char* mem = (char*)wrapped->memory;
        if(address + count > wrapped->size){
	        throw new MemoryException(address);
	    }
        memcpy(buffer, &mem[address], count);
    }
	virtual void Write(uint32_t address,int count,const void *data)
    {
        char* mem = (char*)wrapped->memory;
        if(!wrapped->writeable){
	        throw new MemoryException(address);
        }
        if(address + count > wrapped->size){
	        throw new MemoryException(address);
	    }
        memcpy(&mem[address], data, count);
    }
	virtual int Readable(uint32_t address,int count)
    {
		return 1;
		//This is optional. It is currently not used in the CPU code
	}
	virtual int Writeable(uint32_t address,int count){
		return wrapped->writeable;
	}
	virtual ~WrapperMemory()
    {

    }
};

class WrapperCallMemory: public MemoryDevice{
    public:
    qx86CallMemory* wrapped;
    WrapperCallMemory(qx86CallMemory *mem) : wrapped(mem)
    {
        
    }
    virtual void Read(uint32_t address,int count,void *buffer)
    {
        if(wrapped->read(address, count, buffer, 0)){
	        throw new MemoryException(address);
        }
    }
	virtual void Write(uint32_t address,int count,const void *data)
    {
        if(wrapped->write(address, count, data, 0)){
	        throw new MemoryException(address);
        }
    }
	virtual int Readable(uint32_t address,int count)
    {
        return wrapped->readable(address, count);
	}
	virtual int Writeable(uint32_t address,int count){
        return wrapped->readable(address, count);
	}
	virtual ~WrapperCallMemory()
    {

    }
};

class WrapperHypervisor : public InterruptHypervisor{
    qx86HandleInt* wrapped;
public:
    WrapperHypervisor(qx86HandleInt* hv) : wrapped(hv)
    {
    }
    virtual void HandleInt(int number, x86CPU &vm){
        int err = (*wrapped)(number, (qx86VM_t*)&vm);
        if(err == QX86ERR_FAULT){
            throw new CPUFaultException("wrapped exception", 0);
        }
        if(err == QX86ERR_MEM){
            throw new MemoryException(0);
        }
    }
    virtual ~WrapperHypervisor(){

    }
    //this is a bit of a hack, but here we store the memories created
    //Otherwise it would be difficult to delete them afterwards
    std::vector<WrapperCallMemory> callMemories;
    std::vector<WrapperMemory> memories;
};

int qx86CreateVM(qx86VM_t** vm, qx86HandleInt* hv)
{
    x86CPU *cpu = new x86CPU();
    cpu->Reset();
    MemorySystem* msys = new MemorySystem();
    cpu->Memory = msys;
    WrapperHypervisor *whv = new WrapperHypervisor(hv);
    cpu->Hypervisor = whv;

    *vm = (qx86VM_t*) cpu;
    return 0;
}
int qx86DestroyVM(qx86VM_t* vm)
{
    x86CPU *cpu = (x86CPU*) vm;
    delete cpu->Memory;
    //WrapperHypervisor *whv = (WrapperHypervisor*) cpu->Hypervisor;
    delete (WrapperHypervisor*) cpu->Hypervisor;
    delete cpu;
    return 0;
}
int qx86AddMemory(qx86VM_t* vm, qx86Memory_t* memory)
{
    
    return 0;
}
int qx86AddCallMemory(qx86VM_t* vm, qx86CallMemory_t* memory)
{

    return 0;
}
uint32_t qx86GetReg32(qx86VM_t* vm, int reg)
{

    return 0;
}
void qx86SetReg32(qx86VM_t* vm, int reg, uint32_t val)
{

}
//returns 0 for success, returns 1 for error
int qx86ReadMemory(qx86VM_t* vm, uint32_t address, uint32_t size, void* buffer)
{

    return 0;
}
int qx86WriteMemory(qx86VM_t* vm, uint32_t address, uint32_t size, void* buffer)
{

    return 0;
}

//returns 0 for success, anything else for error (along with qx86Error being populated)
int qx86Execute(qx86VM_t* vm, int maxSteps, qx86Error* error)
{

    return 0;
}
int qx86DestroyError(qx86Error* error)
{

    return 0;
}
int qx86SetGasLimit(qx86VM_t* vm, uint64_t gasLimit)
{

    return 0;
}
int qx86AddGasUsed(qx86VM_t* vm, int64_t gas)
{

    return 0;
}
uint64_t qx86GetGasUsed(qx86VM_t* vm)
{

    return 0;
}
int qx86GasExceeded(qx86VM_t* vm)
{

    return 0;
}
