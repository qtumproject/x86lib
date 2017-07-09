#include <stdint.h>
#include <x86Lib.h>

using namespace x86Lib;

void x86CPU::Push16(uint16_t val){
    if(Use32BitAddress()) {
        regs16[ESP] -= 4;
        WriteWord(cSS, regs32[ESP], val);
    }else{
        *regs16[SP] -= 2;
        WriteWord(cSS, *regs16[SP], val);
    }
}
uint16_t x86CPU::Pop16(){
    uint16_t register tmp;
    if(Use32BitAddress()) {
        tmp = ReadWord(cSS, regs32[ESP]);
        regs32[ESP] += 4;
    }else{
        tmp = ReadWord(cSS, *regs16[SP]);
        *regs16[SP] += 2;
    }
    return tmp;
}

void x86CPU::Push32(uint32_t val){
    regs32[ESP]-=4;
    WriteDword(cSS,regs32[ESP],val);
}
uint32_t x86CPU::Pop32(){
    uint32_t register tmp;
    tmp=ReadDword(cSS,regs32[ESP]);
    regs32[ESP]+=4;
    return tmp;
}

void x86CPU::SetIndex8(){ //this just makes my code look better...
    if(freg.df==0){
        (*regs16[SI])++;
        (*regs16[DI])++;
    }else{
        (*regs16[SI])--;
        (*regs16[DI])--;
    }
}

void x86CPU::SetIndex16(){
    if(freg.df==0){
        (*regs16[SI])+=2;
        (*regs16[DI])+=2;
    }else{
        (*regs16[SI])-=2;
        (*regs16[DI])-=2;
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

//these calculate SF for the given operand size
void x86CPU::CalculateSF8(uint8_t val){
    if((val&0x80)==0){freg.sf=0;}else{freg.sf=1;}
}

void x86CPU::CalculateSF16(uint16_t val){
    if((val&0x8000)==0){freg.sf=0;}else{freg.sf=1;}
}

void x86CPU::CalculateSF32(uint32_t val){
    if((val&0x80000000)==0){freg.sf=0;}else{freg.sf=1;}
}


void x86CPU::Jmp32_near32(uint32_t off){
    //I thought there would be a good way to do this, but I suppose this works..
    if((off&0x80000000)==0){ //if unsigned
        eip=eip+off;
    }else{
        eip=eip-((uint32_t)-off);
    }
}

void x86CPU::Jmp16_near16(uint16_t off){
    //I thought there would be a good way to do this, but I suppose this works..
    if((off&0x8000)==0){ //if unsigned
        eip=eip+off;
    }else{
        eip=eip-((uint16_t)-off);
    }
}

void x86CPU::Jmp16_near8(uint8_t off){
    //I thought there would be a good way to do this, but I suppose this works..
    if((off&0x80)==0){ //if unsigned
        eip=eip+off;
    }else{
        eip=eip-((uint8_t)-off);
    }
    //eip++;
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



uint8_t x86CPU::ReadByte(uint8_t segm,uint32_t off){
    Memory->WaitLock(busmaster);
    uint8_t res=0;
    Memory->Read((seg[segm]<<4)|off,1,&res);
    return res;
}

uint16_t x86CPU::ReadWord(uint8_t segm,uint32_t off){
    Memory->WaitLock(busmaster);
    uint16_t res=0;
    Memory->Read((seg[segm]<<4)|off,2,&res);
    return res;
}

uint32_t x86CPU::ReadDword(uint8_t segm,uint32_t off){
    Memory->WaitLock(busmaster);
    uint32_t res=0;
    Memory->Read((seg[segm]<<4)|off,4,&res);
    return res;
}

uint64_t x86CPU::ReadQword(uint8_t segm,uint32_t off){
    Memory->WaitLock(busmaster);
    uint64_t res=0;
    Memory->Read((seg[segm]<<4)|off,8,&res);
    return res;
}

void x86CPU::WriteByte(uint8_t segm,uint32_t off,uint8_t val){
    Memory->WaitLock(busmaster);
    Memory->Write((seg[segm]<<4)|off,1,&val);
}

void x86CPU::WriteWord(uint8_t segm,uint32_t off,uint16_t val){
    Memory->WaitLock(busmaster);
    Memory->Write((seg[segm]<<4)|off,2,&val);
}

void x86CPU::WriteDword(uint8_t segm,uint32_t off,uint32_t val){
    Memory->WaitLock(busmaster);
    Memory->Write((seg[segm]<<4)|off,4,&val);
}