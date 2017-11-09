#include <stdint.h>
#include <x86Lib.h>

using namespace x86Lib;

void x86CPU::Push(uint32_t val){
    if(Use32BitAddress()) {
        regs32[ESP] -= 4;
        WriteWord(cSS, regs32[ESP], val);
    }else{
        SetReg16(SP, Reg16(SP) - 2);
        WriteWord(cSS, Reg16(SP), val);
    }
}
uint32_t x86CPU::Pop(){
    uint16_t register tmp;
    if(Use32BitAddress()) {
        tmp = ReadWord(cSS, regs32[ESP]);
        regs32[ESP] += 4;
    }else{
        tmp = ReadWord(cSS, Reg16(SP));
        SetReg16(SP, Reg16(SP) + 2);
    }
    return tmp;
}

void x86CPU::SetIndex8(){ //this just makes my code look better...
    if(freg.df==0){
        SetReg16(SI, Reg(SI) + 1);
        SetReg16(DI, Reg(DI) + 1);
    }else{
        SetReg16(SI, Reg(SI) - 1);
        SetReg16(DI, Reg(DI) - 1);
    }
}

void x86CPU::SetIndex(){
    if(OperandSize16){
        SetIndex16();
    }else{
        SetIndex32();
    }
}
void x86CPU::SetIndex16(){
    if(freg.df==0){
        SetReg16(SI, Reg(SI) + 2);
        SetReg16(DI, Reg(DI) + 2);
    }else{
        SetReg16(SI, Reg(SI) - 2);
        SetReg16(DI, Reg(DI) - 2);
    }
}

void x86CPU::SetIndex32(){
    if(freg.df==0){
        (regs32[ESI])+=4;
        (regs32[EDI])+=4;
    }else{
        (regs32[ESI])-=4;
        (regs32[EDI])-=4;
    }
}
void x86CPU::CalculatePF8(uint8_t val){
    unsigned int i;
    unsigned int count=0;
    for(i=0;i<=7;i++){
        if((val&((1<<i)))!=0){count++;}
    }
    if((count%2)==0){freg.pf=1;}else{freg.pf=0;}
}

void x86CPU::CalculatePF(uint32_t val){
    if(OperandSize16){
        CalculatePF16(val);
    }else{
        CalculatePF32(val);
    }
}

void x86CPU::CalculatePF16(uint16_t val){
#ifndef USE_NATIVE
    unsigned int i;
    unsigned int count=0;
    for(i=0;i<=15;i++){
        /* TODO (Jordan#4#): speed this up! */
        if((val&((1<<i)))!=0){count++;}
    }
    if((count%2)==0){freg.pf=1;}else{freg.pf=0;}
#else
    //x86 ASM optimization..
    __asm(".intel_syntax noprefix\n"
    "cmp WORD PTR [ebp-10],0\n"
    "jp .yes__\n"
    ".att_syntax\n");
    val=0;
	__asm(".intel_syntax noprefix\n"
	"jmp .end__\n"
    ".local .yes__:\n"
    ".att_syntax\n");
    val=1;
    __asm(".intel_syntax noprefix\n"
    ".local .end__:\n"
    ".att_syntax\n");
    freg.pf=val;
    return;
#endif
}

void x86CPU::CalculatePF32(uint32_t val){
#ifndef USE_NATIVE
    unsigned int i;
    unsigned int count=0;
    for(i=0;i<=31;i++){
        /* TODO (Jordan#4#): speed this up! */
        if((val&((1<<i)))!=0){count++;}
    }
    if((count%2)==0){freg.pf=1;}else{freg.pf=0;}
#else
    #error "not yet supported"
    //x86 ASM optimization..
    __asm(".intel_syntax noprefix\n"
    "cmp DWORD PTR [ebp-10],0\n"
    "jp .yes__\n"
    ".att_syntax\n");
    val=0;
	__asm(".intel_syntax noprefix\n"
	"jmp .end__\n"
    ".local .yes__:\n"
    ".att_syntax\n");
    val=1;
    __asm(".intel_syntax noprefix\n"
    ".local .end__:\n"
    ".att_syntax\n");
    freg.pf=val;
    return;
#endif
}

//these calculate SF for the given operand size
void x86CPU::CalculateSF8(uint8_t val){
    if((val&0x80)==0){freg.sf=0;}else{freg.sf=1;}
}

void x86CPU::CalculateSF(uint32_t val){
    if(OperandSize16){
        CalculateSF16(val);
    }else{
        CalculateSF32(val);
    }
}
void x86CPU::CalculateSF16(uint16_t val){
    if((val&0x8000)==0){freg.sf=0;}else{freg.sf=1;}
}

void x86CPU::CalculateSF32(uint32_t val){
    if((val&0x80000000)==0){freg.sf=0;}else{freg.sf=1;}
}

void x86CPU::Jmp_nearW(uint32_t off){
    if(OperandSize16){
        Jmp_near16(off);
    }else{
        Jmp_near32(off);
    }
}

void x86CPU::Jmp_near32(uint32_t off){
    //I thought there would be a good way to do this, but I suppose this works..
    if((off&0x80000000)==0){ //if unsigned
        eip=eip+off;
    }else{
        eip=eip-((uint32_t)-off);
    }
    eip = W(eip);
}

void x86CPU::Jmp_near16(uint16_t off){
    //I thought there would be a good way to do this, but I suppose this works..
    if((off&0x8000)==0){ //if unsigned
        eip=eip+off;
    }else{
        eip=eip-((uint16_t)-off);
    }
    eip = W(eip);
}

void x86CPU::Jmp_near8(uint8_t off){
    //I thought there would be a good way to do this, but I suppose this works..
    if((off&0x80)==0){ //if unsigned
        eip=eip+off;
    }else{
        eip=eip-((uint8_t)-off);
    }
    eip = W(eip);
}

void x86CPU::Int16(uint8_t num){
    throw CpuPanic_excp("Unsupported operation (segment register modification)", UNSUPPORTED_EXCP);
}

void x86CPU::ResetSegments(){
    ES=cES;
    CS=cCS;
    SS=cSS;
    DS=cDS;
    FS=cFS;
    GS=cGS;
}

void x86CPU::SetSegments(uint8_t segm){
    ES=segm;
    CS=segm;
    SS=segm;
    DS=segm;
    FS=segm;
    GS=segm;
}

void x86CPU::Read(void* buffer, uint32_t off, size_t count){
    Memory->WaitLock(busmaster);
    Memory->Read(off,count,buffer);
}
void x86CPU::Write(uint32_t off, void* buffer, size_t count){
    Memory->WaitLock(busmaster);
    Memory->Read(off,count,buffer);
}

uint8_t x86CPU::ReadByte(uint8_t segm, uint32_t off){
    Memory->WaitLock(busmaster);
    uint8_t res=0;
    Memory->Read(off,1,&res);
    return res;
}

uint16_t x86CPU::ReadWord(uint8_t segm,uint32_t off){
    Memory->WaitLock(busmaster);
    uint16_t res=0;
    Memory->Read(off,2,&res);
    return res;
}

uint32_t x86CPU::ReadDword(uint8_t segm,uint32_t off){
    Memory->WaitLock(busmaster);
    uint32_t res=0;
    Memory->Read(off,4,&res);
    return res;
}

uint64_t x86CPU::ReadQword(uint8_t segm,uint32_t off){
    Memory->WaitLock(busmaster);
    uint64_t res=0;
    Memory->Read(off,8,&res);
    return res;
}

void x86CPU::WriteByte(uint8_t segm,uint32_t off,uint8_t val){
    Memory->WaitLock(busmaster);
    Memory->Write(off,1,&val);
}

void x86CPU::WriteWord(uint8_t segm,uint32_t off,uint16_t val){
    Memory->WaitLock(busmaster);
    Memory->Write(off,2,&val);
}

void x86CPU::WriteDword(uint8_t segm,uint32_t off,uint32_t val){
    Memory->WaitLock(busmaster);
    Memory->Write(off,4,&val);
}

void x86CPU::WriteW(uint8_t segm, uint32_t off, uint32_t val){
    if(OperandSize16){
        WriteWord(segm, off, val);
    }else{
        WriteDword(segm, off, val);
    }
}

uint32_t x86CPU::ReadW(uint8_t segm, uint32_t off){
    if(OperandSize16){
        return ReadWord(segm, off);
    }else{
        return ReadDword(segm, off);
    }
}

void x86CPU::WriteWA(uint8_t segm, uint32_t off, uint32_t val){
    if(AddressSize16){
        off = off & 0xFFFF;
    }
    WriteW(segm, off, val);
}

uint32_t x86CPU::ReadWA(uint8_t segm, uint32_t off){
    if(AddressSize16){
        off = off & 0xFFFF;
    }
    return ReadW(segm, off);
}

uint8_t x86CPU::ReadByteA(uint8_t segm,uint32_t off){
    if(AddressSize16){
        off = off & 0xFFFF;
    }
    return ReadByte(segm, off);
}

void x86CPU::WriteByteA(uint8_t segm, uint32_t off, uint8_t val){
    if(AddressSize16){
        off = off & 0xFFFF;
    }
    WriteByte(segm, off, val);
}

uint32_t x86CPU::ReadCode32(int index){
    return ReadDword(CS, index + eip);
}
uint16_t x86CPU::ReadCode16(int index){
    return ReadWord(CS, index + eip);
}
uint8_t x86CPU::ReadCode8(int index){
    return ReadByte(CS, index + eip);
}
uint32_t x86CPU::ReadCodeW(int index){
    if(OperandSize16){
        return ReadCode16(index);
    }else{
        return ReadCode32(index);
    }
}

void x86CPU::ReadCode(void* buf, int index, size_t count){
    Read(buf, index + eip, count);
}


