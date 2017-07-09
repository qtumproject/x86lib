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
#include <iostream>
#include <x86Lib.h>
#include <sstream>


namespace x86Lib{
//The lack of indentation for namespaces is intentional...
using namespace std;


x86CPU::x86CPU(uint32_t cpu_level_,uint32_t flags){
	if(cpu_level_==0){
		cpu_level=CPU286_REAL; //The default CPU right now..
	}else{
		cpu_level=cpu_level_;
	}
	Init();
}
x86CPU::x86CPU(x86SaveData &load_data,uint32_t flags){
	Init();
	LoadState(load_data);
}

void x86CPU::Init(){
	//ports=new x86Ports();
#ifdef ENABLE_OPCODE_CALLBACK
	EachOpcodeCallback=NULL;
#endif
	Reset();
}


void x86CPU::Reset(){
	/**Initialize register pointers**/
	regs16[0]=&((volatile uint16_t*)regs32)[0];
	regs16[1]=&((volatile uint16_t*)regs32)[2];
	regs16[2]=&((volatile uint16_t*)regs32)[4];
	regs16[3]=&((volatile uint16_t*)regs32)[6];
	regs16[4]=&((volatile uint16_t*)regs32)[8];
	regs16[5]=&((volatile uint16_t*)regs32)[10];
	regs16[6]=&((volatile uint16_t*)regs32)[12];
	regs16[7]=&((volatile uint16_t*)regs32)[14];
	regs8[0]=&((volatile uint8_t*)regs32)[0];
	regs8[1]=&((volatile uint8_t*)regs32)[4];
	regs8[2]=&((volatile uint8_t*)regs32)[8];
	regs8[3]=&((volatile uint8_t*)regs32)[12], //now do all the highs
	regs8[4]=&((volatile uint8_t*)regs32)[1];
	regs8[5]=&((volatile uint8_t*)regs32)[5];
	regs8[6]=&((volatile uint8_t*)regs32)[9];
	regs8[7]=&((volatile uint8_t*)regs32)[13];
	busmaster=0;
	//assumes pmem and ports are still the same...
	InitOpcodes();
    Opcodes = opcodes_32bit; //for smart contracts, don't use this as a full VM, just straight to 32bit mode
    Opcodes_ext = opcodes_32bit_ext;
	uint32_t i;
	for(i=0;i<8;i++){
		regs32[i]=0;
	}
	for(i=0;i<7;i++){
		seg[i]=0;
	}
	ResetSegments();
	//eip=0xFFFE;
	eip=0;
	seg[cCS]=0x0000;
	*(uint16_t*)&freg=0;
    regs32[ESP] = 0xEFFF0; //set stack to reasonable address for Qtum
	string_compares=0;
	int_pending=0;
	cli_count=0;
    OperandSize16=false;
    AddressSize16=false;
    DoStop=false;
}


void x86CPU::SaveState(x86SaveData *save){
	uint32_t i;
	for(i=0;i<8;i++){
		save->reg32[i]=regs32[i];
	}
	for(i=0;i<7;i++){
		save->seg[i]=seg[i];
	}
	save->freg=*(uint16_t*)&freg;
	save->eip=eip;
	save->seg_route[cES]=ES;
	save->seg_route[cCS]=CS;
	save->seg_route[cDS]=DS;
	save->seg_route[cSS]=SS;
	save->seg_route[cFS]=FS;
	save->seg_route[cGS]=GS;
	save->cpu_level=cpu_level;
	if(Opcodes == opcodes_16bit) {
        save->opcode_mode = OPCODE_REAL_16;
    }else if(Opcodes == opcodes_32bit){
        save->opcode_mode = OPCODE_HOSTED_32;
	}else{
		throw Default_excp(__FILE__,__FUNCTION__,__LINE__);
	}
}

void x86CPU::LoadState(x86SaveData &load){
	uint32_t i;
	cpu_level=load.cpu_level;
	Reset();
	for(i=0;i<8;i++){
		regs32[i]=load.reg32[i];
	}
	for(i=0;i<7;i++){
		seg[i]=load.seg[i];
	}
	*(uint16_t*)&freg=load.freg;
	eip=load.eip;
	ES=load.seg_route[cES];
	CS=load.seg_route[cCS];
	DS=load.seg_route[cDS];
	SS=load.seg_route[cSS];
	GS=load.seg_route[cGS];
	FS=load.seg_route[cFS];
	if(load.opcode_mode==OPCODE_REAL_16) {
        Opcodes = opcodes_16bit;
    }else if(load.opcode_mode == OPCODE_HOSTED_32){
        Opcodes = opcodes_32bit;
	}else{
		throw Default_excp(__FILE__,__FUNCTION__,__LINE__);
	}
}




void x86CPU::DumpState(ostream &output){
	output << "EAX: "<< hex << regs32[EAX] <<endl;
	output << "ECX: "<< hex << regs32[ECX] <<endl;
	output << "EDX: "<< hex << regs32[EDX] <<endl;
	output << "EBX: "<< hex << regs32[EBX] <<endl;
	output << "ESP: "<< hex << regs32[ESP] <<endl;
	output << "EBP: "<< hex << regs32[EBP] <<endl;
	output << "ESI: "<< hex << regs32[ESI] <<endl;
	output << "EDI: "<< hex << regs32[EDI] <<endl;

	output << "CS: " << hex << seg[cCS] << endl;
	output << "SS: " << hex << seg[cSS] << endl;
	output << "DS: " << hex << seg[cDS] << endl;
	output << "ES: " << hex << seg[cES] << endl;
	output << "FS: " << hex << seg[cFS] << endl;
	output << "GS: " << hex << seg[cGS] << endl;
	output << "EIP: " << hex << eip << endl;

	output << "--Flags:" <<hex << *(uint16_t*)&freg<< endl;
	output << "CF: " << (int)freg.cf << endl;
	output << "PF: " << (int)freg.pf << endl;
	output << "AF: " << (int)freg.af << endl;
	output << "ZF: " << (int)freg.zf << endl;
	output << "SF: " << (int)freg.sf << endl;
	output << "TF: " << (int)freg.tf << endl;
	output << "IF: " << (int)freg._if << endl;
	output << "DF: " << (int)freg.df << endl;
	output << "OF: " << (int)freg.of << endl;
}

void x86CPU::Int(uint8_t num){ //external interrupt
	int_pending=1;
	int_number=num;
}

bool x86CPU::IntPending(){
	return int_pending;
}



int x86CPU::CheckInterrupts(){
	//possible bug here. What if we have a pop SS without an interrupt waiting? Won't interrupts be forever ignored?
	if(int_pending==0){return 0;} //quickly get out of this, this is in the main Cycle loop, so needs to be very speedy...
	if((int_pending==1) && (int_number==2)){ //NMI
		eip--;
		Int16(2);
		eip++;
		int_pending=0;
		return 1;
	}
	if(freg._if==0){
		if(cli_count>1){
			cli_count--;
		}else{
			if(cli_count!=0){
				freg._if=1;
			}
		}
	}else{
		if(int_pending==1){
			eip--;
			Int16(int_number);
			eip++;
			int_pending=0;
			return 1;
		}
	}
	return 0;
}

void x86CPU::Exec(int cyclecount){
	int i=0;
	bool done=false;
	while(!done){
		try{
			for(;i<cyclecount;i++){
				Cycle();
                if(DoStop){
                    DoStop=false;
                    return;
                }
			}
		}
		catch(CpuInt_excp err){
			err.code&=0x00FF;
			switch(err.code){
				case 0: //division by zero
				case 1: //debug exception
				case 2: //NMI
				case 3: //breakpoint
				case 4: //overflow
				
				case 7: //device unavailable
				Int16(err.code);
				eip++; //undo the decrement by Int
				break;
                case 6: //unknown opcode
                    throw CpuPanic_excp("Unknown opcode",(err.code|0xF000)|TRIPLE_FAULT_EXCP);
				case 5: //(186+ bounds check)
				if(cpu_level >= CPU186){
					Int16(err.code);
					eip++;
					break;
				}
				default:
				throw CpuPanic_excp("16bit Faults",(err.code|0xF000)|TRIPLE_FAULT_EXCP);
			}
		}
		catch(Mem_excp err){
            std::ostringstream oss;
            oss << "Memory error at 0x" << std::hex << err.address;
			throw CpuPanic_excp(oss.str(),TRIPLE_FAULT_EXCP);
		}
		if(i>=cyclecount){
			done=true;
		}
	}
}


void x86CPU::Cycle(){
#ifdef ENABLE_OPCODE_CALLBACK
	if(EachOpcodeCallback!=NULL){
		(*EachOpcodeCallback)(this);
	}
#endif
	CheckInterrupts();
	*(uint64_t*)&op_cache=ReadQword(cCS,eip);
    //note this bit for 0x0F checking could probably be better in this very critical loop
    if(op_cache[0] == 0x0F){
        //two byte opcode
        eip++;
        *(uint64_t*)&op_cache=ReadQword(cCS,eip);
        (this->*Opcodes_ext[op_cache[0]])();
    }else {
        //operate on the this class with the opcode functions in this class
        (this->*Opcodes[op_cache[0]])();
    }
	eip=eip+1;
}







void x86CPU::InstallOp(uint8_t num,opcode func, opcode *opcode_table){
    if(opcode_table){
        opcode_table[num]=func;
    }else {
        Opcodes[num] = func;
    }
}


void x86CPU::InitOpcodes(){
	Opcodes=opcodes_16bit;
	int i;
	for(i=0;i<256;i++){
		InstallOp(i,&x86CPU::op16_unknown);
	}
	for(i=0;i<=7;i++){
		InstallOp(0xB0+i,&x86CPU::op16_mov_r8_imm8);
		InstallOp(0x58+i,&x86CPU::op16_pop_r16);
		InstallOp(0x50+i,&x86CPU::op16_push_r16);
		InstallOp(0x40+i,&x86CPU::op16_inc_r16);
		InstallOp(0x48+i,&x86CPU::op16_dec_r16);
		InstallOp(0xD8+i,&x86CPU::op16_escape);
		InstallOp(0x90+i,&x86CPU::op16_xchg_ax_r16);
		InstallOp(0xB8+i,&x86CPU::op16_mov_r16_imm16);
	}
	InstallOp(0xF4,&x86CPU::op16_hlt);
	InstallOp(0x90,&x86CPU::op16_nop);
	InstallOp(0xEB,&x86CPU::op16_jmp_rel8);
	InstallOp(0x2C,&x86CPU::op16_sub_al_imm8);
	InstallOp(0x2D,&x86CPU::op16_sub_ax_imm16);
	InstallOp(0x77,&x86CPU::op16_ja_rel8);
	InstallOp(0x70,&x86CPU::op16_jo_rel8);
	InstallOp(0x71,&x86CPU::op16_jno_rel8);
	InstallOp(0x72,&x86CPU::op16_jc_rel8);
	InstallOp(0x73,&x86CPU::op16_jnc_rel8);
	InstallOp(0x74,&x86CPU::op16_jz_rel8);
	InstallOp(0x75,&x86CPU::op16_jnz_rel8);
	InstallOp(0x77,&x86CPU::op16_jbe_rel8);
	InstallOp(0x78,&x86CPU::op16_js_rel8);
	InstallOp(0x79,&x86CPU::op16_jns_rel8);
	InstallOp(0x7A,&x86CPU::op16_jp_rel8);
	InstallOp(0x7B,&x86CPU::op16_jnp_rel8);
	InstallOp(0x7C,&x86CPU::op16_jl_rel8);
	InstallOp(0x7D,&x86CPU::op16_jge_rel8);
	InstallOp(0x7E,&x86CPU::op16_jle_rel8);
	InstallOp(0x7F,&x86CPU::op16_jg_rel8);
	InstallOp(0x8E,&x86CPU::op16_mov_sr_rm16);
	InstallOp(0x8C,&x86CPU::op16_mov_rm16_sr);
	InstallOp(0x68,&x86CPU::op16_push_imm16);
	InstallOp(0x07,&x86CPU::op16_pop_es);
	InstallOp(0x17,&x86CPU::op16_pop_ss);
	InstallOp(0x1F,&x86CPU::op16_pop_ds);
	InstallOp(0x06,&x86CPU::op16_push_es);
	InstallOp(0x0E,&x86CPU::op16_push_cs);
	InstallOp(0x16,&x86CPU::op16_push_ss);
	InstallOp(0x1E,&x86CPU::op16_push_ds);
	InstallOp(0x89,&x86CPU::op16_mov_rm16_r16);
	InstallOp(0x8B,&x86CPU::op16_mov_r16_rm16);
	InstallOp(0xE8,&x86CPU::op16_call_rel16);
	InstallOp(0xC3,&x86CPU::op16_retn);
	InstallOp(0xE2,&x86CPU::op16_loop_rel8);
	InstallOp(0x26,&x86CPU::op16_pre_es_override);
	InstallOp(0x3E,&x86CPU::op16_pre_ds_override);
	InstallOp(0x36,&x86CPU::op16_pre_ss_override);
	InstallOp(0x2E,&x86CPU::op16_pre_cs_override);
	InstallOp(0xA5,&x86CPU::op16_movsw);
	InstallOp(0xA4,&x86CPU::op16_movsb);
	InstallOp(0xF8,&x86CPU::op16_clc);
	InstallOp(0xFC,&x86CPU::op16_cld);
	InstallOp(0xFA,&x86CPU::op16_cli);
	InstallOp(0xF9,&x86CPU::op16_stc);
	InstallOp(0xFD,&x86CPU::op16_std);
	InstallOp(0xFB,&x86CPU::op16_sti);
	InstallOp(0xF2,&x86CPU::op16_rep);
	InstallOp(0xF3,&x86CPU::op16_rep); //different, but handled by the same function...
	InstallOp(0xE6,&x86CPU::op16_out_imm8_al);
	InstallOp(0xE7,&x86CPU::op16_out_imm8_ax);
	InstallOp(0x9A,&x86CPU::op16_call_imm16_imm16);
	InstallOp(0xCB,&x86CPU::op16_retf); //lol...CB...<inside joke>
	InstallOp(0xCD,&x86CPU::op16_int_imm8);
	InstallOp(0xCF,&x86CPU::op16_iret);
	InstallOp(0xCC,&x86CPU::op16_int3);
	InstallOp(0xCE,&x86CPU::op16_into);
	InstallOp(0xE4,&x86CPU::op16_in_al_imm8);
	InstallOp(0xE5,&x86CPU::op16_in_ax_imm8);
	InstallOp(0x04,&x86CPU::op16_add_al_imm8);
	InstallOp(0x05,&x86CPU::op16_add_ax_imm16);
	InstallOp(0x28,&x86CPU::op16_sub_rm8_r8);
	InstallOp(0x80,&x86CPU::op16_group_80);
	InstallOp(0x29,&x86CPU::op16_sub_rm16_r16);
	InstallOp(0x2A,&x86CPU::op16_sub_r8_rm8);
	InstallOp(0x2B,&x86CPU::op16_sub_r16_rm16);
	InstallOp(0x81,&x86CPU::op16_group_81);
	InstallOp(0x00,&x86CPU::op16_add_rm8_r8);
	InstallOp(0x01,&x86CPU::op16_add_rm16_r16);
	InstallOp(0x02,&x86CPU::op16_add_r8_rm8);
	InstallOp(0x03,&x86CPU::op16_add_r16_rm16);
	InstallOp(0xA0,&x86CPU::op16_mov_al_off8);
	InstallOp(0xA1,&x86CPU::op16_mov_ax_off16);
	InstallOp(0x88,&x86CPU::op16_mov_rm8_r8);
	InstallOp(0x8A,&x86CPU::op16_mov_r8_rm8);
	InstallOp(0xA2,&x86CPU::op16_mov_off8_al);
	InstallOp(0xA3,&x86CPU::op16_mov_off16_ax);
	InstallOp(0xC6,&x86CPU::op16_mov_m8_imm8);
	InstallOp(0xC7,&x86CPU::op16_mov_m16_imm16);
	InstallOp(0x38,&x86CPU::op16_cmp_rm8_r8);
	InstallOp(0x39,&x86CPU::op16_cmp_rm16_r16);
	InstallOp(0x3A,&x86CPU::op16_cmp_r8_rm8);
	InstallOp(0x3B,&x86CPU::op16_cmp_r16_rm16);
	InstallOp(0x3C,&x86CPU::op16_cmp_al_imm8);
	InstallOp(0x3D,&x86CPU::op16_cmp_ax_imm16); //3D TimD Tim Vision TV!!! Yay!!!
	InstallOp(0x83,&x86CPU::op16_group_83); //83 is my lucky number btw...
	InstallOp(0xFF,&x86CPU::op16_group_FF);
	InstallOp(0xE9,&x86CPU::op16_jmp_rel16);
	InstallOp(0xEA,&x86CPU::op16_jmp_imm16_imm16);
	InstallOp(0x6A,&x86CPU::op16_push_imm8);
	InstallOp(0x8F,&x86CPU::op16_group_8F);
	InstallOp(0xD6,&x86CPU::op16_salc);
	InstallOp(0xF5,&x86CPU::op16_cmc);
	InstallOp(0x98,&x86CPU::op16_cbw);
	InstallOp(0x37,&x86CPU::op16_aaa);
	InstallOp(0x27,&x86CPU::op16_daa);
	InstallOp(0x2F,&x86CPU::op16_das);
	InstallOp(0x3F,&x86CPU::op16_aas);
	InstallOp(0xD5,&x86CPU::op16_aad);
	InstallOp(0xD4,&x86CPU::op16_aam);
	InstallOp(0xFE,&x86CPU::op16_group_FE);
	InstallOp(0xF6,&x86CPU::op16_group_F6);
	InstallOp(0xF7,&x86CPU::op16_group_F7);
	InstallOp(0x99,&x86CPU::op16_cwd);
	InstallOp(0x20,&x86CPU::op16_and_rm8_r8);
	InstallOp(0x21,&x86CPU::op16_and_rm16_r16);
	InstallOp(0x22,&x86CPU::op16_and_r8_rm8);
	InstallOp(0x23,&x86CPU::op16_and_r16_rm16);
	InstallOp(0x24,&x86CPU::op16_and_al_imm8);
	InstallOp(0x25,&x86CPU::op16_and_ax_imm16);
	InstallOp(0x08,&x86CPU::op16_or_rm8_r8);
	InstallOp(0x09,&x86CPU::op16_or_rm16_r16);
	InstallOp(0x0A,&x86CPU::op16_or_r8_rm8);
	InstallOp(0x0B,&x86CPU::op16_or_r16_rm16);
	InstallOp(0x0C,&x86CPU::op16_or_al_imm8);
	InstallOp(0x0D,&x86CPU::op16_or_ax_imm16);
	InstallOp(0xA6,&x86CPU::op16_cmpsb);
	InstallOp(0xA7,&x86CPU::op16_cmpsw);
	InstallOp(0xE3,&x86CPU::op16_jcxz_rel8);
	InstallOp(0x14,&x86CPU::op16_adc_al_imm8);
	InstallOp(0x15,&x86CPU::op16_adc_ax_imm16);
	InstallOp(0x10,&x86CPU::op16_adc_rm8_r8);
	InstallOp(0x11,&x86CPU::op16_adc_rm16_r16);
	InstallOp(0x12,&x86CPU::op16_adc_r8_rm8);
	InstallOp(0x13,&x86CPU::op16_adc_r16_rm16);
	InstallOp(0x9E,&x86CPU::op16_sahf);
	InstallOp(0x9F,&x86CPU::op16_lahf);
	InstallOp(0xE1,&x86CPU::op16_loope_rel8);
	InstallOp(0xE0,&x86CPU::op16_loopne_rel8);
	InstallOp(0xC5,&x86CPU::op16_lds);
	InstallOp(0xC4,&x86CPU::op16_les);
	InstallOp(0x8D,&x86CPU::op16_lea);
	InstallOp(0xF0,&x86CPU::op16_lock);
	InstallOp(0x30,&x86CPU::op16_xor_rm8_r8);
	InstallOp(0x31,&x86CPU::op16_xor_rm16_r16);
	InstallOp(0x32,&x86CPU::op16_xor_r8_rm8);
	InstallOp(0x33,&x86CPU::op16_xor_r16_rm16);
	InstallOp(0x34,&x86CPU::op16_xor_al_imm8);
	InstallOp(0x35,&x86CPU::op16_xor_ax_imm16);
	InstallOp(0x1C,&x86CPU::op16_sbb_al_imm8);
	InstallOp(0x1D,&x86CPU::op16_sbb_ax_imm16);
	InstallOp(0x19,&x86CPU::op16_sbb_rm16_r16);
	InstallOp(0x1A,&x86CPU::op16_sbb_r8_rm8);
	InstallOp(0x1B,&x86CPU::op16_sbb_r16_rm16);
	InstallOp(0x18,&x86CPU::op16_sub_rm8_r8);
	InstallOp(0x84,&x86CPU::op16_test_rm8_r8);
	InstallOp(0x85,&x86CPU::op16_test_rm16_r16);
	InstallOp(0xA8,&x86CPU::op16_test_al_imm8);
	InstallOp(0xA9,&x86CPU::op16_test_ax_imm16);
	InstallOp(0x86,&x86CPU::op16_xchg_rm8_r8);
	InstallOp(0x87,&x86CPU::op16_xchg_rm16_r16);
	InstallOp(0xD2,&x86CPU::op16_group_D2);
	InstallOp(0xD3,&x86CPU::op16_group_D3);
	InstallOp(0xD0,&x86CPU::op16_group_D0);
	InstallOp(0xD1,&x86CPU::op16_group_D1);
	InstallOp(0xAC,&x86CPU::op16_lodsb);
	InstallOp(0xAD,&x86CPU::op16_lodsw);
	InstallOp(0xAE,&x86CPU::op16_scasb);
	InstallOp(0xAF,&x86CPU::op16_scasw);
	InstallOp(0x9B,&x86CPU::op16_wait);
	InstallOp(0xD7,&x86CPU::op16_xlatb);
	InstallOp(0xEC,&x86CPU::op16_in_al_dx);
	InstallOp(0xED,&x86CPU::op16_in_ax_dx);
	InstallOp(0xEE,&x86CPU::op16_out_dx_al);
	InstallOp(0xEF,&x86CPU::op16_out_dx_ax);
	InstallOp(0xAA,&x86CPU::op16_stosb);
	InstallOp(0xAB,&x86CPU::op16_stosw);

    Opcodes=opcodes_32bit;
    InstallOp(0x66, &x86CPU::op32_size16);
    for(i=0;i<256;i++){
        InstallOp(i,&x86CPU::op16_unknown);
    }
    for(i=0;i<=7;i++){
        InstallOp(0xB0+i,&x86CPU::op16_mov_r8_imm8);
        InstallOp(0x58+i,&x86CPU::op32_pop_r32);
        InstallOp(0x50+i,&x86CPU::op32_push_r32);
        InstallOp(0x90+i,&x86CPU::op32_xchg_eax_r32);
        InstallOp(0x40+i,&x86CPU::op32_inc_r32);
        InstallOp(0x48+i,&x86CPU::op32_dec_r32);
        InstallOp(0xD8+i,&x86CPU::op16_escape);
        InstallOp(0xB8+i,&x86CPU::op32_mov_r32_imm32);
    }
    //unchanged opcodes from 16bit:
    InstallOp(0xE6,&x86CPU::op16_out_imm8_al);
    InstallOp(0xF4,&x86CPU::op16_hlt);
    InstallOp(0x90,&x86CPU::op16_nop);
    InstallOp(0xEB,&x86CPU::op16_jmp_rel8);
    InstallOp(0x2C,&x86CPU::op16_sub_al_imm8);
    InstallOp(0x77,&x86CPU::op16_ja_rel8);
    InstallOp(0x70,&x86CPU::op16_jo_rel8);
    InstallOp(0x71,&x86CPU::op16_jno_rel8);
    InstallOp(0x72,&x86CPU::op16_jc_rel8);
    InstallOp(0x73,&x86CPU::op16_jnc_rel8);
    InstallOp(0x74,&x86CPU::op16_jz_rel8);
    InstallOp(0x75,&x86CPU::op16_jnz_rel8);
    InstallOp(0x77,&x86CPU::op16_jbe_rel8);
    InstallOp(0x78,&x86CPU::op16_js_rel8);
    InstallOp(0x79,&x86CPU::op16_jns_rel8);
    InstallOp(0x7A,&x86CPU::op16_jp_rel8);
    InstallOp(0x7B,&x86CPU::op16_jnp_rel8);
    InstallOp(0x7C,&x86CPU::op16_jl_rel8);
    InstallOp(0x7D,&x86CPU::op16_jge_rel8);
    InstallOp(0x7E,&x86CPU::op16_jle_rel8);
    InstallOp(0x7F,&x86CPU::op16_jg_rel8);
    InstallOp(0xF8,&x86CPU::op16_clc); //technically for 32bit, some of these should have protection, but doesn't matter for smart contracts
    InstallOp(0xFC,&x86CPU::op16_cld);
    InstallOp(0xFA,&x86CPU::op16_cli);
    InstallOp(0xF9,&x86CPU::op16_stc);
    InstallOp(0xFD,&x86CPU::op16_std);
    InstallOp(0xFB,&x86CPU::op16_sti);
    InstallOp(0xE4,&x86CPU::op16_in_al_imm8);
    InstallOp(0xF0,&x86CPU::op16_lock);
    InstallOp(0x9B,&x86CPU::op16_wait);
    InstallOp(0xEC,&x86CPU::op16_in_al_dx);
    InstallOp(0xEE,&x86CPU::op16_out_dx_al);
    InstallOp(0x04,&x86CPU::op16_add_al_imm8);
    InstallOp(0x28,&x86CPU::op16_sub_rm8_r8);
    InstallOp(0x2A,&x86CPU::op16_sub_r8_rm8);
    InstallOp(0x00,&x86CPU::op16_add_rm8_r8);
    InstallOp(0x02,&x86CPU::op16_add_r8_rm8);
    InstallOp(0x88,&x86CPU::op16_mov_rm8_r8);
    InstallOp(0x8A,&x86CPU::op16_mov_r8_rm8);
    InstallOp(0x38,&x86CPU::op16_cmp_rm8_r8);
    InstallOp(0x3A,&x86CPU::op16_cmp_r8_rm8);
    InstallOp(0x20,&x86CPU::op16_and_rm8_r8);
    InstallOp(0x22,&x86CPU::op16_and_r8_rm8);
    InstallOp(0x08,&x86CPU::op16_or_rm8_r8);
    InstallOp(0x0A,&x86CPU::op16_or_r8_rm8);
    InstallOp(0x0C,&x86CPU::op16_or_al_imm8);
    InstallOp(0x14,&x86CPU::op16_adc_al_imm8);
    InstallOp(0x10,&x86CPU::op16_adc_rm8_r8);
    InstallOp(0x12,&x86CPU::op16_adc_r8_rm8);
    InstallOp(0x18,&x86CPU::op16_sub_rm8_r8);
    InstallOp(0x30,&x86CPU::op16_xor_rm8_r8);
    InstallOp(0x32,&x86CPU::op16_xor_r8_rm8);
    InstallOp(0x34,&x86CPU::op16_xor_al_imm8);
    InstallOp(0x1C,&x86CPU::op16_sbb_al_imm8);
    InstallOp(0x1A,&x86CPU::op16_sbb_r8_rm8);
    InstallOp(0x84,&x86CPU::op16_test_rm8_r8);
    InstallOp(0x86,&x86CPU::op16_xchg_rm8_r8);
    InstallOp(0x98,&x86CPU::op16_cbw);
    InstallOp(0x37,&x86CPU::op16_aaa);
    InstallOp(0x27,&x86CPU::op16_daa);
    InstallOp(0x2F,&x86CPU::op16_das);
    InstallOp(0x3F,&x86CPU::op16_aas);
    InstallOp(0xD5,&x86CPU::op16_aad);
    InstallOp(0xD4,&x86CPU::op16_aam);
    InstallOp(0x80,&x86CPU::op16_group_80);
    InstallOp(0xD0,&x86CPU::op16_group_D0);
    InstallOp(0xD2,&x86CPU::op16_group_D2);
    InstallOp(0xD6,&x86CPU::op16_salc);
    InstallOp(0xF5,&x86CPU::op16_cmc);
	InstallOp(0x9E,&x86CPU::op16_sahf);
	InstallOp(0x9F,&x86CPU::op16_lahf);
    InstallOp(0xA8,&x86CPU::op16_test_al_imm8);
    InstallOp(0xAC,&x86CPU::op16_lodsb);
    InstallOp(0xAE,&x86CPU::op16_scasb);
    InstallOp(0xD7,&x86CPU::op16_xlatb);
    InstallOp(0xAA,&x86CPU::op16_stosb);
    InstallOp(0xA0,&x86CPU::op16_mov_al_off8);
    InstallOp(0xA2,&x86CPU::op16_mov_off8_al);
    InstallOp(0xC6,&x86CPU::op16_mov_m8_imm8);
    InstallOp(0x3C,&x86CPU::op16_cmp_al_imm8);
    InstallOp(0x24,&x86CPU::op16_and_al_imm8);
    InstallOp(0xA6,&x86CPU::op16_cmpsb);
    InstallOp(0xA4,&x86CPU::op16_movsb);
    InstallOp(0xFE,&x86CPU::op16_group_FE);
    InstallOp(0xF6,&x86CPU::op16_group_F6);

    //unsupported opcodes
    InstallOp(0x8E,&x86CPU::op16_mov_sr_rm16);
    InstallOp(0x8C,&x86CPU::op16_mov_rm16_sr);
    InstallOp(0x07,&x86CPU::op16_pop_es);
    InstallOp(0x17,&x86CPU::op16_pop_ss);
    InstallOp(0x1F,&x86CPU::op16_pop_ds);
    InstallOp(0x06,&x86CPU::op16_push_es);
    InstallOp(0x0E,&x86CPU::op16_push_cs);
    InstallOp(0x16,&x86CPU::op16_push_ss);
    InstallOp(0x1E,&x86CPU::op16_push_ds);
    InstallOp(0x26,&x86CPU::op16_pre_es_override); //supported, but are effectively no-ops
    InstallOp(0x3E,&x86CPU::op16_pre_ds_override);
    InstallOp(0x36,&x86CPU::op16_pre_ss_override);
    InstallOp(0x2E,&x86CPU::op16_pre_cs_override);
    InstallOp(0x9A,&x86CPU::op16_call_imm16_imm16);
    InstallOp(0xCB,&x86CPU::op16_retf);
    InstallOp(0xCD,&x86CPU::op16_int_imm8);
    InstallOp(0xCF,&x86CPU::op16_iret);
    InstallOp(0xCC,&x86CPU::op16_int3);
    InstallOp(0xCE,&x86CPU::op16_into);
    InstallOp(0xC5,&x86CPU::op16_lds);
    InstallOp(0xC4,&x86CPU::op16_les);
    InstallOp(0xEA,&x86CPU::op16_jmp_imm16_imm16);


    //changed opcodes to 32bit:
    InstallOp(0x68,&x86CPU::op32_push_imm32);
    InstallOp(0x89,&x86CPU::op32_mov_rm32_r32);
    InstallOp(0x8B,&x86CPU::op32_mov_r32_rm32);
    InstallOp(0xE8,&x86CPU::op32_call_rel32);
    InstallOp(0xC3,&x86CPU::op32_retn);
    InstallOp(0xE2,&x86CPU::op32_loop_rel8);
    InstallOp(0x2D,&x86CPU::op32_sub_eax_imm32);
    InstallOp(0xE7,&x86CPU::op32_out_imm8_eax);
    InstallOp(0xE5,&x86CPU::op32_in_eax_imm8);
    InstallOp(0xED,&x86CPU::op32_in_eax_dx);
    InstallOp(0xEF,&x86CPU::op32_out_dx_eax);
    InstallOp(0xA5,&x86CPU::op32_movsd);
    InstallOp(0xF2,&x86CPU::op32_rep);
    InstallOp(0xF3,&x86CPU::op32_rep); //different, but handled by the same function...
    InstallOp(0x05,&x86CPU::op32_add_eax_imm32);
    InstallOp(0xC7,&x86CPU::op32_mov_m32_imm32);
    InstallOp(0x03,&x86CPU::op32_add_r32_rm32);
    InstallOp(0x83,&x86CPU::op32_group_83);
    InstallOp(0xAD,&x86CPU::op32_lodsd);
    InstallOp(0xAF,&x86CPU::op32_scasd);
    InstallOp(0xAB,&x86CPU::op32_stosd);
    InstallOp(0x29,&x86CPU::op32_sub_rm32_r32);
    InstallOp(0x2B,&x86CPU::op32_sub_r32_rm32);
    InstallOp(0x81,&x86CPU::op32_group_81);
    InstallOp(0xA1,&x86CPU::op32_mov_eax_off32);
    InstallOp(0xA3,&x86CPU::op32_mov_off32_eax);
    InstallOp(0x01,&x86CPU::op32_add_rm32_r32);
    InstallOp(0x39,&x86CPU::op32_cmp_rm32_r32);
    InstallOp(0x3B,&x86CPU::op32_cmp_r32_rm32);
    InstallOp(0x3D,&x86CPU::op32_cmp_eax_imm32);
    InstallOp(0xE9,&x86CPU::op32_jmp_rel32);
    InstallOp(0x6A,&x86CPU::op32_push_imm8);
    InstallOp(0x99,&x86CPU::op32_cwq);
    InstallOp(0x21,&x86CPU::op32_and_rm32_r32);
    InstallOp(0x23,&x86CPU::op32_and_r32_rm32);
    InstallOp(0x25,&x86CPU::op32_and_eax_imm32);
    InstallOp(0x09,&x86CPU::op32_or_rm32_r32);
    InstallOp(0x0B,&x86CPU::op32_or_r32_rm32);
    InstallOp(0x0D,&x86CPU::op32_or_eax_imm32);
    InstallOp(0xA7,&x86CPU::op32_cmpsd);
    InstallOp(0xE3,&x86CPU::op32_jcxz_rel8);
    InstallOp(0x15,&x86CPU::op32_adc_eax_imm32);
    InstallOp(0x11,&x86CPU::op32_adc_rm32_r32);
    InstallOp(0x13,&x86CPU::op32_adc_r32_rm32);
    InstallOp(0xE1,&x86CPU::op32_loope_rel8);
    InstallOp(0xE0,&x86CPU::op32_loopne_rel8);
    InstallOp(0x8D,&x86CPU::op32_lea);
    InstallOp(0x31,&x86CPU::op32_xor_rm32_r32);
    InstallOp(0x33,&x86CPU::op32_xor_r32_rm32);
    InstallOp(0x35,&x86CPU::op32_xor_eax_imm32);
    InstallOp(0x1D,&x86CPU::op32_sbb_eax_imm32);
    InstallOp(0x19,&x86CPU::op32_sbb_rm32_r32);
    InstallOp(0x1B,&x86CPU::op32_sbb_r32_rm32);
    InstallOp(0x85,&x86CPU::op32_test_rm32_r32);
    InstallOp(0xA9,&x86CPU::op32_test_eax_imm32);
    InstallOp(0x87,&x86CPU::op32_xchg_rm32_r32);
    InstallOp(0xFF,&x86CPU::op32_group_FF);
    InstallOp(0x8F,&x86CPU::op32_group_8F);

    InstallOp(0xD1,&x86CPU::op32_group_D1);
    InstallOp(0xF7,&x86CPU::op32_group_F7);



    //two byte opcodes (new as of i286)
    InstallOp(0xB6,&x86CPU::op16_movzx_r16_rm8, opcodes_16bit_ext);
    InstallOp(0xB6,&x86CPU::op32_movzx_r32_rm8, opcodes_32bit_ext);
    InstallOp(0xB7,&x86CPU::op32_movzx_r32_rm16, opcodes_32bit_ext);





}


bool x86CPU::IsLocked(){
	return Memory->IsLocked();
}

void x86CPU::Lock(){
	//nothing...
	Memory->Lock();
	busmaster=1;
}

void x86CPU::Unlock(){
	//still nothing...
	Memory->Unlock();
	busmaster=0;
}


void x86CPU::ReadMemory(uint32_t address, uint32_t size, void* buffer){
    Memory->WaitLock(busmaster);
    Memory->Read(address, size, buffer);
}
void x86CPU::WriteMemory(uint32_t address, uint32_t size, void* buffer){
    Memory->WaitLock(busmaster);
    Memory->Write(address, size, buffer);
}


};






