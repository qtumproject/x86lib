/**
Copyright (c) 2007 - 2010 Jordan "Earlz/hckr83" Earls  <http://www.Earlz.biz.tm>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.
   
THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

This file is part of the x86Lib project.
**/
#include <x86Lib.h>
namespace x86Lib{
using namespace std;






void x86CPU::op_mov_r8_imm8(){ //0xB0+r
	*regs8[op_cache[0]-0xB0]=op_cache[1];
	eip++;
}

void x86CPU::op_mov_rW_immW(){ //0xB8+r
    WriteReg(op_cache[0]-0xB8, W(*(uint32_t*)&op_cache[1]));
	eip+=OperandSize();
}

void x86CPU::op_mov_sr_rm16(){ //0x8E
    eip++;
    ModRM rm(this);
    //need ModRM for parsing, but otherwise it's a no-op
}

void x86CPU::op_mov_rm16_sr(){ //0x8C
    eip++;
    ModRM rm(this);
    rm.WriteWord(0);
}


void x86CPU::op_mov_rW_rmW(){
	eip++;
	ModRM rm(this);
	WriteReg(Reg(rm.GetExtra()), rm.ReadW());
}

void x86CPU::op_mov_rmW_rW(){
	eip++;
	ModRM rm(this);
	rm.WriteW(Reg(rm.GetExtra()));
}

void x86CPU::op_mov_al_m8(){
    *regs8[AL] = ReadByteA(DS, ImmA());
}
void x86CPU::op_mov_axW_mW(){
    WriteReg(AX, ReadWA(DS, ImmA()));
}

void x86CPU::op_mov_rm8_r8(){
	eip++;
	ModRM rm(this);
	rm.WriteByte(*regs8[rm.GetExtra()]);
}

void x86CPU::op_mov_r8_rm8(){
	eip++;
	ModRM rm(this);
	*regs8[rm.GetExtra()]=rm.ReadByte();
}
void x86CPU::op_mov_m8_al(){
    WriteByte(DS, ImmA(), *regs8[AL]);
}

void x86CPU::op_mov_mW_axW(){
    WriteWA(DS, ImmA(), Reg(AX));
}

void x86CPU::op_mov_rm8_imm8(){
	eip++;
	ModRM rm(this);
	
	//eventually fix this so that if r is used, then invalid opcode...
	rm.WriteByte(ReadByte(cCS,eip+rm.GetLength()));
	eip++;
}

void x86CPU::op_mov_rmW_immW(){
    eip++;
    ModRM rm(this);
    rm.WriteW(ReadW(cCS,eip+rm.GetLength()));
    eip+=OperandSize();
}

void x86CPU::op_lds(){
    throw new CpuInt_excp(GPF_IEXCP);
}

void x86CPU::op_les(){
    throw new CpuInt_excp(GPF_IEXCP);
}

void x86CPU::op_lea(){
	eip++;
	ModRM rm(this);
	WriteReg(rm.GetExtra(), rm.ReadOffset());
}



void x86CPU::op_push_imm8(){
	eip++;
	Push(op_cache[1]);
}
void x86CPU::op_push_rmW(ModRM &rm){
	Push(rm.ReadW());
}

void x86CPU::op_push_immW(){ //0x68
    Push(ImmW());
}

void x86CPU::op_push_rW(){ //0x50+reg
	Push(Reg(op_cache[0]-0x50));
}

void x86CPU::op_push_es(){
    Push(0);
}

void x86CPU::op_push_cs(){
    Push(0);
}

void x86CPU::op_push_ds(){
    Push(0);
}

void x86CPU::op_push_ss(){
    Push(0);
}

void x86CPU::op_pop_rmW(ModRM &rm){
	rm.WriteW(Pop());
}

void x86CPU::op_pop_rW(){ //0x58+reg
	WriteReg(op_cache[0]-0x58, Pop());
}

void x86CPU::op_pop_es(){
    Pop();
}

void x86CPU::op_pop_ss(){
    Pop();
}

void x86CPU::op_pop_ds(){
    Pop();
}

void x86CPU::op_out_imm8_al(){
	Ports->Write(op_cache[1],1,(void*)regs8[AL]);
	eip++;
}

void x86CPU::op_out_imm8_axW(){
    if(OperandSize16){
        Ports->Write(op_cache[1],2,(void*)regs16[AX]);
    }else{
        Ports->Write(op_cache[1],4,(void*)&regs32[EAX]);
    }
	eip++;
}

void x86CPU::op_out_dx_al(){
	Ports->Write(*regs16[DX],1,(void*)regs8[AL]);
}

void x86CPU::op_out_dx_axW(){
    if(OperandSize16){
        Ports->Write(*regs16[DX],2,(void*)regs16[AX]);
    }else{
        Ports->Write(*regs16[DX],4,(void*)&regs32[EAX]);
    }
}

void x86CPU::op_in_al_imm8(){
	Ports->Read(op_cache[1],1,(void*)regs8[AL]);
	eip++;
}

void x86CPU::op_in_axW_imm8(){
    if(OperandSize16){
    	Ports->Read(op_cache[1],2,(void*)regs16[AX]);
    }else{
        Ports->Read(op_cache[1],4,(void*)&regs32[EAX]);
    }
	eip++;
}

void x86CPU::op_in_al_dx(){
	Ports->Read(*regs16[DX],1,(void*)regs8[AL]);
}

void x86CPU::op_in_axW_dx(){
    if(OperandSize16){
    	Ports->Read(*regs16[DX],2,(void*)regs16[AX]);
    }else{
        Ports->Read(*regs16[DX],4,(void*)&regs32[EAX]);
    }
}



void x86CPU::op_xchg_rm8_r8(){
	#ifndef X86_MULTITHREADING
	if(IsLocked()==1){
		eip--;
		return;
	}
	#endif
	Lock();
	eip++;
	ModRM rm(this);
	uint8_t tmp=*regs8[rm.GetExtra()];
	*regs8[rm.GetExtra()]=rm.ReadByte();
	rm.WriteByte(tmp);
	Unlock();
}
void x86CPU::op_xchg_rmW_rW(){
	#ifndef X86_MULTITHREADING
	if(IsLocked()==1){
		eip--;
		return;
	}
	#endif
	Lock();
	eip++;
	ModRM rm(this);
	uint32_t tmp=Reg(rm.GetExtra());
    WriteReg(rm.GetExtra(), rm.ReadW());
	rm.WriteW(tmp);
	Unlock();
}

void x86CPU::op_xchg_axW_rW(){ //0x90+r
	uint32_t tmp=Reg(AX);
	WriteReg(AX, Reg(op_cache[0]-0x90));
	WriteReg(op_cache[0]-0x90, tmp);
}

void x86CPU::op_xlatb(){
    *regs8[AL] = ReadByteA(DS, RegA(BX) + (*regs8[AL]));
}

void x86CPU::op_movzx_rW_rm8(){
    eip++;
    ModRM rm(this);
    WriteReg(rm.GetExtra(), rm.ReadByte());
}
void x86CPU::op_movzx_r32_rmW(){
    eip++;
    ModRM rm(this);
    regs32[rm.GetExtra()] = rm.ReadWord();
}




};



