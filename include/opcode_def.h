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

/**This file contains the opcode function definitions and prototypes for x86CPU**/

/**NOTE! this is included INSIDE of a class, so this file is somewhat limited...**/
uint8_t Add8(uint8_t,uint8_t);
uint16_t Add16(uint16_t,uint16_t);
uint32_t Add32(uint32_t,uint32_t);
uint8_t Sub8(uint8_t,uint8_t);
uint16_t Sub16(uint16_t,uint16_t);
uint32_t Sub32(uint32_t,uint32_t);
uint8_t And8(uint8_t,uint8_t);
uint16_t And16(uint16_t,uint16_t);
uint8_t Or8(uint8_t,uint8_t);
uint16_t Or16(uint16_t,uint16_t);
uint8_t Xor8(uint8_t,uint8_t);
uint16_t Xor16(uint16_t,uint16_t);
uint8_t ShiftLogicalRight8(uint8_t,uint8_t);
uint16_t ShiftLogicalRight16(uint16_t,uint8_t);
uint8_t ShiftArithmeticRight8(uint8_t,uint8_t);
uint16_t ShiftArithmeticRight16(uint16_t,uint8_t);
uint8_t ShiftLogicalLeft8(uint8_t,uint8_t);
uint16_t ShiftLogicalLeft16(uint16_t,uint8_t);
uint8_t RotateRight8(uint8_t,uint8_t);
uint16_t RotateRight16(uint16_t,uint8_t);
uint8_t RotateLeft8(uint8_t,uint8_t);
uint16_t RotateLeft16(uint16_t,uint8_t);
uint8_t RotateCarryRight8(uint8_t,uint8_t);
uint16_t RotateCarryRight16(uint16_t,uint8_t);
uint8_t RotateCarryLeft8(uint8_t,uint8_t);
uint16_t RotateCarryLeft16(uint16_t,uint8_t);


void InstallOp(uint8_t,opcode,uint32_t=CPU086);
void InitOpcodes();
/*
uint8_t ReadByte(uint8_t seg,uint32_t offset);
uint16_t ReadWord(uint8_t seg,uint32_t offset);
uint32_t ReadDword(uint8_t seg,uint32_t offset);

void WriteByte(uint8_t seg,uint32_t offset,uint8_t data);
void WriteWord(uint8_t seg,uint32_t offset,uint16_t data);
void WriteDword(uint8_t seg,uint32_t offset,uint32_t data);
*/





void op16_unknown();
void op16_mov_r8_imm8(); //Tested, pass #1;
void op16_hlt(); //Tested, pass #1;
void op16_nop(); //Tested, pass #1;
void op16_mov_r16_imm16(); //Tested, pass #1;
void op16_jmp_rel8(); //Tested, pass #1;
void op16_sub_al_imm8(); //Tested, pass #1;
void op16_sub_ax_imm16(); //Tested, pass #1;
void op16_ja_rel8();
void op16_jnc_rel8();
void op16_jbe_rel8();
void op16_jc_rel8(); //Tested, pass #1;
void op16_jz_rel8(); //Tested, pass #1;
void op16_jnz_rel8(); //Tested, pass #1;
void op16_jp_rel8(); //Tested, pass #1;
void op16_jnp_rel8(); //Tested, pass #1;
void op16_jg_rel8(); //Tested, pass #1;
void op16_jge_rel8();
void op16_jle_rel8();
void op16_jl_rel8(); //Tested, pass #1;
void op16_jo_rel8();
void op16_jno_rel8();
void op16_js_rel8();
void op16_jns_rel8();
void op16_mov_sr_rm16(); //Tested, pass #1;
void op16_mov_rm16_sr(); //Tested, pass #1;
void op16_pop_r16(); //Tested, pass #1;
void op16_push_imm16(); //Tested, pass #1;
void op16_push_r16(); //Tested, pass #1;
void op16_push_es(); //Tested, pass #1;
void op16_push_cs(); //Tested, pass #1;
void op16_push_ss(); //Tested, pass #1;
void op16_push_ds(); //Tested, pass #1;
void op16_pop_es(); //Tested, pass #1;
void op16_pop_ss(); //Tested, pass #1;
void op16_pop_ds(); //Tested, pass #1;
void op16_mov_r16_rm16(); //Tested, pass #1;
void op16_mov_rm16_r16(); //Tested, pass #1;
void op16_call_rel16(); //Tested, pass #1;
void op16_retn(); //Tested, pass #1;
void op16_loop_rel8(); //Tested, pass #1;
void op16_pre_cs_override(); //Tested, pass #1;
void op16_pre_ds_override(); //Tested, pass #1;
void op16_pre_es_override(); //Tested, pass #1;
void op16_pre_ss_override(); //Tested, pass #1;
void op16_movsw(); //Tested, pass #1;
void op16_movsb(); //Tested, pass #1;
void op16_clc();
void op16_cld(); //Tested, pass #1;
void op16_cli(); //Tested, pass #1;
void op16_stc();
void op16_std(); //Tested, pass #1;
void op16_sti(); //Tested, pass #1;
void op16_rep(); //Tested, pass #1;(only rep, not conditionals)
void op16_out_imm8_al(); //Tested, pass #1;
void op16_out_imm8_ax(); //Tested, pass #1;
void op16_call_imm16_imm16(); //Tested, pass #1;
void op16_retf(); //Tested, pass #1;
void op16_int_imm8(); //Tested, pass #1;
void op16_iret(); //Tested, pass #1;
void op16_into();
void op16_int3();
void op16_in_al_imm8();
void op16_in_ax_imm8();
void op16_add_al_imm8(); //Tested, pass #1;
void op16_add_ax_imm8(); //Tested, pass #1;
void op16_sub_rm8_r8(); //Tested, pass #1;
void op16_group_80(); //Tested, pass #1;
void op16_sub_r8_rm8();
void op16_sub_rm16_r16();
void op16_sub_r16_rm16();
void op16_group_81(); //Tested, pass #1;
void op16_add_rm8_r8();
void op16_add_rm16_r16();
void op16_add_r8_rm8();
void op16_add_r16_rm16();
void op16_mov_al_off8();
void op16_mov_ax_off16();
void op16_mov_rm8_r8();
void op16_mov_r8_rm8();
void op16_mov_off8_al(); //Tested, pass #1;
void op16_mov_off16_ax(); //Tested, pass #1;
void op16_mov_m8_imm8(); //Tested, pass #1;
void op16_mov_m16_imm16();  //Tested, pass #1;//currently have 85 instructions or prefixes implemented(actually more, not counting group instructions)
void op16_cmp_rm8_r8();
void op16_cmp_rm16_r16();
void op16_cmp_r8_rm8();
void op16_cmp_r16_rm16();
void op16_cmp_al_imm8(); //Tested, pass #1;
void op16_cmp_ax_imm16(); //Tested, pass #1;
void op16_group_83(); //Tested, pass #1;
void op16_jmp_rel16(); //Tested, pass #1
void op16_jmp_imm16_imm16(); //Tested, pass #1
void op16_group_FF();
void op16_push_imm8();
void op16_group_8F();
void op16_salc(); //Undocumented -- Set AL on Carry
void op16_cmc();
void op16_cbw();
void op16_aaa();
void op16_daa();
void op16_das();
void op16_aas();
void op16_aad();
void op16_aam();
void op16_inc_r16();
void op16_dec_r16();
void op16_group_FE();
void op16_group_F6();
void op16_group_F7();
void op16_cwd();
void op16_and_rm8_r8();
void op16_and_rm16_r16();
void op16_and_r8_rm8();
void op16_and_r16_rm16();
void op16_and_al_imm8();
void op16_and_ax_imm16();
void op16_or_rm8_r8();
void op16_or_rm16_r16();
void op16_or_r8_rm8();
void op16_or_r16_rm16();
void op16_or_al_imm8();
void op16_or_ax_imm16();
void op16_escape();
void op16_cmpsb(); //Tested, pass #1, full
void op16_cmpsw(); //tested, pass #1, full
void op16_jcxz_rel8();
void op16_adc_al_imm8();
void op16_adc_ax_imm8();
void op16_adc_rm8_r8();
void op16_adc_rm16_r16();
void op16_adc_r8_rm8();
void op16_adc_r16_rm16();
void op16_lahf();
void op16_sahf();
void op16_loope_rel8();
void op16_loopne_rel8();
void op16_lds();
void op16_les();
void op16_lea();
void op16_lock(); //funcitonally does nothing...
void op16_xor_rm8_r8();
void op16_xor_rm16_r16();
void op16_xor_r8_rm8();
void op16_xor_r16_rm16();
void op16_xor_al_imm8();
void op16_xor_ax_imm16();
void op16_sbb_rm8_r8();
void op16_sbb_rm16_r16();
void op16_sbb_r8_rm8();
void op16_sbb_r16_rm16();
void op16_sbb_al_imm8();
void op16_sbb_ax_imm16();
void op16_test_al_imm8();
void op16_test_ax_imm16();
void op16_test_rm8_r8();
void op16_test_rm16_r16();
void op16_xchg_rm8_r8();
void op16_xchg_rm16_r16();
void op16_xchg_ax_r16();
void op16_group_D2();
void op16_group_D3();
void op16_group_D0();
void op16_group_D1();
void op16_lodsb();
void op16_lodsw();
void op16_scasb();
void op16_scasw();
void op16_stosb();
void op16_stosw();
void op16_wait();
void op16_xlatb();

void op16_in_al_dx();
void op16_in_ax_dx();
void op16_out_dx_al();
void op16_out_dx_ax();


//32bit

void op32_size16();

void op32_pop_r32();
void op32_push_r32();
void op32_inc_r32();
void op32_xchg_eax_r32();
void op32_dec_r32();
void op32_mov_r32_imm32();
void op32_push_imm32();


//Oh God how I hate prototyping and adding the opcodes to the master InstallOp list...




/**Group Include Functions(not direct opcodes)**/
void op16_sub_rm8_imm8(ModRM16&); //group 0x80 /5
void op16_sub_rm16_imm16(ModRM16&);
void op16_add_rm8_imm8(ModRM16&);
void op16_add_rm16_imm16(ModRM16&);
void op16_cmp_rm8_imm8(ModRM16&); //Tested, pass #1
void op16_cmp_rm16_imm16(ModRM16&);
void op16_sub_rm16_imm8(ModRM16&);
void op16_add_rm16_imm8(ModRM16&);
void op16_cmp_rm16_imm8(ModRM16&); //Tested, pass #1
void op16_jmp_rm16(ModRM16&); //Tested, pass #1
void op16_jmp_m16_m16(ModRM16&); //Tested, pass #1
void op16_push_m16(ModRM16&);
void op16_pop_m16(ModRM16&);
void op16_inc_rm8(ModRM16&);
void op16_inc_rm16(ModRM16&);
void op16_dec_rm8(ModRM16&);
void op16_dec_rm16(ModRM16&);
void op16_div_rm8(ModRM16 &rm);//Tested, pass #1
void op16_div_rm16(ModRM16 &rm);//Tested, pass #1
void op16_idiv_rm8(ModRM16 &rm);//Tested, pass #1
void op16_idiv_rm16(ModRM16 &rm);//Tested, pass #1
void op16_mul_rm8(ModRM16 &rm);//Tested, pass #1
void op16_mul_rm16(ModRM16 &rm);//Tested, pass #1
void op16_imul_rm8(ModRM16 &rm);
void op16_imul_rm16(ModRM16 &rm);
void op16_and_rm8_imm8(ModRM16 &rm);
void op16_and_rm16_imm16(ModRM16 &rm);
void op16_and_rm16_imm8(ModRM16 &rm);
void op16_or_rm8_imm8(ModRM16& rm);
void op16_or_rm16_imm16(ModRM16 &rm);
void op16_or_rm16_imm8(ModRM16 &rm);
void op16_adc_rm8_imm8(ModRM16&);
void op16_adc_rm16_imm16(ModRM16&);
void op16_adc_rm16_imm8(ModRM16&);
void op16_neg_rm16(ModRM16&);
void op16_neg_rm8(ModRM16&);
void op16_xor_rm8_imm8(ModRM16& rm);
void op16_xor_rm16_imm16(ModRM16 &rm);
void op16_xor_rm16_imm8(ModRM16 &rm);
void op16_sbb_rm8_imm8(ModRM16 &rm);
void op16_sbb_rm16_imm16(ModRM16 &rm);
void op16_sbb_rm16_imm8(ModRM16 &rm);
void op16_test_rm8_imm8(ModRM16 &rm);
void op16_test_rm16_imm16(ModRM16 &rm);
void op16_test_rm16_imm8(ModRM16 &rm); /**This needs to be added**/
void op16_shr_rm8_cl(ModRM16 &rm);
void op16_shr_rm16_cl(ModRM16 &rm);
void op16_sar_rm8_cl(ModRM16 &rm);
void op16_sar_rm16_cl(ModRM16 &rm);
void op16_shl_rm8_cl(ModRM16 &rm);
void op16_shl_rm16_cl(ModRM16 &rm);
void op16_rol_rm8_cl(ModRM16 &rm);
void op16_rol_rm16_cl(ModRM16 &rm);
void op16_ror_rm8_cl(ModRM16 &rm);
void op16_ror_rm16_cl(ModRM16 &rm);
void op16_rcl_rm8_cl(ModRM16 &rm);
void op16_rcl_rm16_cl(ModRM16 &rm);
void op16_rcr_rm8_cl(ModRM16 &rm);
void op16_rcr_rm16_cl(ModRM16 &rm);
void op16_shr_rm8_1(ModRM16 &rm);
void op16_shr_rm16_1(ModRM16 &rm);
void op16_sar_rm8_1(ModRM16 &rm);
void op16_sar_rm16_1(ModRM16 &rm);
void op16_shl_rm8_1(ModRM16 &rm);
void op16_shl_rm16_1(ModRM16 &rm);
void op16_rol_rm8_1(ModRM16 &rm);
void op16_rol_rm16_1(ModRM16 &rm);
void op16_ror_rm8_1(ModRM16 &rm);
void op16_ror_rm16_1(ModRM16 &rm);
void op16_rcl_rm8_1(ModRM16 &rm);
void op16_rcl_rm16_1(ModRM16 &rm);
void op16_rcr_rm8_1(ModRM16 &rm);
void op16_rcr_rm16_1(ModRM16 &rm);
void op16_not_rm8(ModRM16 &rm);
void op16_not_rm16(ModRM16 &rm);
void op16_call_rm16(ModRM16 &rm);
void op16_call_rm16_rm16(ModRM16 &rm);



void Push16(uint16_t val){
	*regs16[SP]-=2;
	WriteWord(cSS,*regs16[SP],val);
}
uint16_t Pop16(){
	uint16_t register tmp;
	tmp=ReadWord(cSS,*regs16[SP]);
	*regs16[SP]+=2;
	return tmp;
}

void Push32(uint32_t val){
    regs32[ESP]-=4;
    WriteDword(cSS,regs32[ESP],val);
}
uint32_t Pop32(){
    uint32_t register tmp;
    tmp=ReadDword(cSS,regs32[ESP]);
    regs32[ESP]+=4;
    return tmp;
}

inline void SetIndex8(){ //this just makes my code look better...
	if(freg.df==0){
		(*regs16[SI])++;
		(*regs16[DI])++;
	}else{
		(*regs16[SI])--;
		(*regs16[DI])--;
	}
}

inline void SetIndex16(){
	if(freg.df==0){
		(*regs16[SI])+=2;
		(*regs16[DI])+=2;
	}else{
		(*regs16[SI])-=2;
		(*regs16[DI])-=2;
	}
}

inline void SetIndex32(){
    if(freg.df==0){
        (regs32[ESI])+=4;
        (regs32[EDI])+=4;
    }else{
        (regs32[ESI])-=4;
        (regs32[EDI])-=4;
    }
}
inline void CalculatePF8(uint8_t val){
    unsigned int i;
    unsigned int count=0;
    for(i=0;i<=7;i++){
        if((val&((1<<i)))!=0){count++;}
    }
    if((count%2)==0){freg.pf=1;}else{freg.pf=0;}




}

inline void CalculatePF16(uint16_t val){
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

inline void CalculatePF32(uint32_t val){
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
inline void CalculateSF8(uint8_t val){
    if((val&0x80)==0){freg.sf=0;}else{freg.sf=1;}
}

inline void CalculateSF16(uint16_t val){
    if((val&0x8000)==0){freg.sf=0;}else{freg.sf=1;}
}

inline void CalculateSF32(uint32_t val){
    if((val&0x80000000)==0){freg.sf=0;}else{freg.sf=1;}
}


void Jmp32_near32(uint32_t off){
    //I thought there would be a good way to do this, but I suppose this works..
    if((off&0x80000000)==0){ //if unsigned
        eip=eip+off;
    }else{
        eip=eip-((uint16_t)-off);
    }
}

void Jmp16_near16(uint16_t off){
	//I thought there would be a good way to do this, but I suppose this works..
	if((off&0x8000)==0){ //if unsigned
		eip=eip+off;
	}else{
		eip=eip-((uint16_t)-off);
	}
}

void Jmp16_near8(uint8_t off){
	//I thought there would be a good way to do this, but I suppose this works..
	if((off&0x80)==0){ //if unsigned
		eip=eip+off;
	}else{
		eip=eip-((uint8_t)-off);
	}
	//eip++;
}

void Int16(uint8_t num){
	Push16(*(uint16_t*)&freg);
	freg._if=0;
	Push16(seg[cCS]);
	Push16(eip);
	seg[cIS]=0;
	eip=ReadWord(cIS,num*4);
	seg[cCS]=ReadWord(cIS,num*4+2);
	
//	cout << "0x" << hex << eip << "0x" << seg[cCS] << endl;
	eip--;
}

inline void ResetSegments(){
	ES=cES;
	CS=cCS;
	SS=cSS;
	DS=cDS;
	FS=cFS;
	GS=cGS;
}

inline void SetSegments(uint8_t segm){
	ES=segm;
	CS=segm;
	SS=segm;
	DS=segm;
	FS=segm;
	GS=segm;
}



inline uint8_t ReadByte(uint8_t segm,uint32_t off){
	Memory->WaitLock(busmaster);
	uint8_t res=0;
	Memory->Read((seg[segm]<<4)|off,1,&res);
	return res;
}

inline uint16_t ReadWord(uint8_t segm,uint32_t off){
	Memory->WaitLock(busmaster);
	uint16_t res=0;
	if(Opcodes==opcodes_16bit && off>=0xFFFE){

		uint16_t offset=off; //for ease of rounding
		uint32_t tmp=0;
		Memory->Read((seg[segm]<<4)|offset,1,&tmp);
		res=tmp;
		Memory->Read((seg[segm]<<4)|(uint16_t)(offset+1),1,&tmp);
		res|=tmp<<8;
		return res;
	}else{
		Memory->Read((seg[segm]<<4)|off,2,&res);
		return res;
	}
}

inline uint32_t ReadDword(uint8_t segm,uint32_t off){
	Memory->WaitLock(busmaster);
	uint32_t res=0;
	if(Opcodes==opcodes_16bit && off>=0xFFFC){

		uint16_t offset=off; //for ease of rounding
		uint32_t tmp=0;
		Memory->Read((seg[segm]<<4)|offset,1,&tmp);
		res=tmp;
		Memory->Read((seg[segm]<<4)|(uint16_t)(offset+1),1,&tmp);
		res|=tmp<<8;
		Memory->Read((seg[segm]<<4)|(uint16_t)(offset+2),1,&tmp);
		res|=tmp<<16;
		Memory->Read((seg[segm]<<4)|(uint16_t)(offset+3),1,&tmp);
		res|=tmp<<24;
		return res;
	}else{
		Memory->Read((seg[segm]<<4)|off,4,&res);
		return res;
	}
}

inline void WriteByte(uint8_t segm,uint32_t off,uint8_t val){
		Memory->WaitLock(busmaster);
		Memory->Write((seg[segm]<<4)|off,1,&val);
}

inline void WriteWord(uint8_t segm,uint32_t off,uint16_t val){
	Memory->WaitLock(busmaster);
	if(Opcodes==opcodes_16bit && off>=0xFFFE){
		uint16_t offset=off; //for ease of rounding
		uint16_t tmp=val&0x00FF;
		Memory->Write((seg[segm]<<4)|offset,1,&tmp);
		tmp=(val&0xFF00)>>8;
		Memory->Write((seg[segm]<<4)|(uint16_t)(offset+1),1,&tmp);
	}else{
		Memory->Write((seg[segm]<<4)|off,2,&val);
	}
}

inline void WriteDword(uint8_t segm,uint32_t off,uint32_t val){
	Memory->WaitLock(busmaster);
	if(Opcodes==opcodes_16bit && off>=0xFFFC){
		uint16_t offset=off; //for ease of rounding
		uint32_t tmp=val&0x00FF;
		Memory->Write((seg[segm]<<4)|offset,1,&tmp);
		tmp=(val&0xFF00)>>8;
		Memory->Write((seg[segm]<<4)|(uint16_t)(offset+1),1,&tmp);
		tmp=(val&0xFF0000)>>16;
		Memory->Write((seg[segm]<<4)|(uint16_t)(offset+2),1,&tmp);
		tmp=(val&0xFF000000)>>24;
		Memory->Write((seg[segm]<<4)|(uint16_t)(offset+3),1,&tmp);
	}else{
		Memory->Write((seg[segm]<<4)|off,4,&val);
	}
}




