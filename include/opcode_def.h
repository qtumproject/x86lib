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
uint32_t And32(uint32_t,uint32_t);
uint8_t Or8(uint8_t,uint8_t);
uint16_t Or16(uint16_t,uint16_t);
uint32_t Or32(uint32_t,uint32_t);
uint8_t Xor8(uint8_t,uint8_t);
uint16_t Xor16(uint16_t,uint16_t);
uint32_t Xor32(uint32_t,uint32_t);
uint8_t ShiftLogicalRight8(uint8_t,uint8_t);
uint16_t ShiftLogicalRight16(uint16_t,uint8_t);
uint32_t ShiftLogicalRight32(uint32_t,uint8_t);
uint8_t ShiftArithmeticRight8(uint8_t,uint8_t);
uint16_t ShiftArithmeticRight16(uint16_t,uint8_t);
uint32_t ShiftArithmeticRight32(uint32_t,uint8_t);
uint8_t ShiftLogicalLeft8(uint8_t,uint8_t);
uint16_t ShiftLogicalLeft16(uint16_t,uint8_t);
uint32_t ShiftLogicalLeft32(uint32_t,uint8_t);
uint8_t RotateRight8(uint8_t,uint8_t);
uint16_t RotateRight16(uint16_t,uint8_t);
uint32_t RotateRight32(uint32_t,uint8_t);
uint8_t RotateLeft8(uint8_t,uint8_t);
uint16_t RotateLeft16(uint16_t,uint8_t);
uint32_t RotateLeft32(uint32_t,uint8_t);
uint8_t RotateCarryRight8(uint8_t,uint8_t);
uint16_t RotateCarryRight16(uint16_t,uint8_t);
uint32_t RotateCarryRight32(uint32_t,uint8_t);
uint8_t RotateCarryLeft8(uint8_t,uint8_t);
uint16_t RotateCarryLeft16(uint16_t,uint8_t);
uint32_t RotateCarryLeft32(uint32_t,uint8_t);


void InstallOp(uint8_t num,opcode op,opcode *table=NULL);
void InitOpcodes();


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
void op16_add_ax_imm16(); //Tested, pass #1;
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
void op16_adc_ax_imm16();
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
void op32_mov_rm32_r32();
void op32_mov_r32_rm32();
void op32_call_rel32();
void op32_retn();
void op32_loop_rel8();
void op32_sub_eax_imm32();
void op32_out_imm8_eax();
void op32_in_eax_imm8();
void op32_in_eax_dx();
void op32_out_dx_eax();
void op32_movsd();
void op32_rep();
void op32_add_eax_imm32();
void op32_mov_m32_imm32();
void op32_add_r32_rm32();
void op32_group_83();
void op32_lodsd();
void op32_scasd();
void op32_stosd();
void op32_sub_rm32_r32();
void op32_sub_r32_rm32();
void op32_group_81();
void op32_mov_eax_off32();
void op32_mov_off32_eax();
void op32_add_rm32_r32();
void op32_cmp_rm32_r32();
void op32_cmp_r32_rm32();
void op32_cmp_eax_imm32();
void op32_jmp_rel32();
void op32_push_imm8();
void op32_cwq();
void op32_and_rm32_r32();
void op32_and_r32_rm32();
void op32_and_eax_imm32();
void op32_or_rm32_r32();
void op32_or_r32_rm32();
void op32_or_eax_imm32();
void op32_cmpsd();
void op32_jcxz_rel8();
void op32_adc_eax_imm32();
void op32_adc_rm32_r32();
void op32_adc_r32_rm32();
void op32_loope_rel8();
void op32_loopne_rel8();
void op32_lea();
void op32_xor_rm32_r32();
void op32_xor_r32_rm32();
void op32_xor_eax_imm32();
void op32_sbb_eax_imm32();
void op32_sbb_rm32_r32();
void op32_sbb_r32_rm32();
void op32_test_rm32_r32();
void op32_test_eax_imm32();
void op32_xchg_rm32_r32();
void op32_group_FF();
void op32_group_8F();
void op32_group_D1();
void op32_group_F7();


void op_jcc_imm8();
void op16_jcc_imm16();
void op32_jcc_imm32();

//Oh God how I hate prototyping and adding the opcodes to the master InstallOp list...


/**Group Include Functions(not direct opcodes)**/
void op16_sub_rm8_imm8(ModRM&); //group 0x80 /5
void op16_sub_rm16_imm16(ModRM&);
void op16_add_rm8_imm8(ModRM&);
void op16_add_rm16_imm16(ModRM&);
void op16_cmp_rm8_imm8(ModRM&); //Tested, pass #1
void op16_cmp_rm16_imm16(ModRM&);
void op16_sub_rm16_imm8(ModRM&);
void op16_add_rm16_imm8(ModRM&);
void op16_cmp_rm16_imm8(ModRM&); //Tested, pass #1
void op16_jmp_rm16(ModRM&); //Tested, pass #1
void op16_jmp_m16_m16(ModRM&); //Tested, pass #1
void op16_push_m16(ModRM&);
void op16_pop_m16(ModRM&);
void op16_inc_rm8(ModRM&);
void op16_inc_rm16(ModRM&);
void op16_dec_rm8(ModRM&);
void op16_dec_rm16(ModRM&);
void op16_div_rm8(ModRM &rm);//Tested, pass #1
void op16_div_rm16(ModRM &rm);//Tested, pass #1
void op16_idiv_rm8(ModRM &rm);//Tested, pass #1
void op16_idiv_rm16(ModRM &rm);//Tested, pass #1
void op16_mul_rm8(ModRM &rm);//Tested, pass #1
void op16_mul_rm16(ModRM &rm);//Tested, pass #1
void op16_imul_rm8(ModRM &rm);
void op16_imul_rm16(ModRM &rm);
void op16_and_rm8_imm8(ModRM &rm);
void op16_and_rm16_imm16(ModRM &rm);
void op16_and_rm16_imm8(ModRM &rm);
void op16_or_rm8_imm8(ModRM& rm);
void op16_or_rm16_imm16(ModRM &rm);
void op16_or_rm16_imm8(ModRM &rm);
void op16_adc_rm8_imm8(ModRM&);
void op16_adc_rm16_imm16(ModRM&);
void op16_adc_rm16_imm8(ModRM&);
void op16_neg_rm16(ModRM&);
void op16_neg_rm8(ModRM&);
void op16_xor_rm8_imm8(ModRM& rm);
void op16_xor_rm16_imm16(ModRM &rm);
void op16_xor_rm16_imm8(ModRM &rm);
void op16_sbb_rm8_imm8(ModRM &rm);
void op16_sbb_rm16_imm16(ModRM &rm);
void op16_sbb_rm16_imm8(ModRM &rm);
void op16_test_rm8_imm8(ModRM &rm);
void op16_test_rm16_imm16(ModRM &rm);
void op16_test_rm16_imm8(ModRM &rm); /**This needs to be added**/
void op16_shr_rm8_cl(ModRM &rm);
void op16_shr_rm16_cl(ModRM &rm);
void op16_sar_rm8_cl(ModRM &rm);
void op16_sar_rm16_cl(ModRM &rm);
void op16_shl_rm8_cl(ModRM &rm);
void op16_shl_rm16_cl(ModRM &rm);
void op16_rol_rm8_cl(ModRM &rm);
void op16_rol_rm16_cl(ModRM &rm);
void op16_ror_rm8_cl(ModRM &rm);
void op16_ror_rm16_cl(ModRM &rm);
void op16_rcl_rm8_cl(ModRM &rm);
void op16_rcl_rm16_cl(ModRM &rm);
void op16_rcr_rm8_cl(ModRM &rm);
void op16_rcr_rm16_cl(ModRM &rm);
void op16_shr_rm8_1(ModRM &rm);
void op16_shr_rm16_1(ModRM &rm);
void op16_sar_rm8_1(ModRM &rm);
void op16_sar_rm16_1(ModRM &rm);
void op16_shl_rm8_1(ModRM &rm);
void op16_shl_rm16_1(ModRM &rm);
void op16_rol_rm8_1(ModRM &rm);
void op16_rol_rm16_1(ModRM &rm);
void op16_ror_rm8_1(ModRM &rm);
void op16_ror_rm16_1(ModRM &rm);
void op16_rcl_rm8_1(ModRM &rm);
void op16_rcl_rm16_1(ModRM &rm);
void op16_rcr_rm8_1(ModRM &rm);
void op16_rcr_rm16_1(ModRM &rm);
void op16_not_rm8(ModRM &rm);
void op16_not_rm16(ModRM &rm);
void op16_call_rm16(ModRM &rm);
void op16_call_rm16_rm16(ModRM &rm);

//32bit groups
void op32_add_rm32_imm8(ModRM &rm);
void op32_adc_rm32_imm8(ModRM &rm);
void op32_and_rm32_imm8(ModRM &rm);
void op32_or_rm32_imm8(ModRM &rm);
void op32_xor_rm32_imm8(ModRM &rm);
void op32_test_rm32_imm8(ModRM &rm);
void op32_sbb_rm32_imm8(ModRM &rm);
void op32_cmp_rm32_imm8(ModRM &rm);
void op32_sub_rm32_imm8(ModRM &rm);
void op32_cmp_rm32_imm32(ModRM &rm);
void op32_add_rm32_imm32(ModRM &rm);
void op32_adc_rm32_imm32(ModRM &rm);
void op32_and_rm32_imm32(ModRM &rm);
void op32_or_rm32_imm32(ModRM &rm);
void op32_xor_rm32_imm32(ModRM &rm);
void op32_test_rm32_imm32(ModRM &rm);
void op32_sub_rm32_imm32(ModRM &rm);
void op32_sbb_rm32_imm32(ModRM &rm);
void op32_jmp_rm32(ModRM &rm);
void op32_push_m32(ModRM &rm);
void op32_inc_rm32(ModRM &rm);
void op32_dec_rm32(ModRM &rm);
void op32_call_rm32(ModRM &rm);
void op32_pop_m32(ModRM &rm);
void op32_sar_rm32_1(ModRM &rm);
void op32_shl_rm32_1(ModRM &rm);
void op32_shr_rm32_1(ModRM &rm);
void op32_rol_rm32_1(ModRM &rm);
void op32_ror_rm32_1(ModRM &rm);
void op32_rcr_rm32_1(ModRM &rm);
void op32_rcl_rm32_1(ModRM &rm);
void op32_div_rm32(ModRM &rm);
void op32_idiv_rm32(ModRM &rm);
void op32_mul_rm32(ModRM &rm);
void op32_imul_rm32(ModRM &rm);
void op32_neg_rm32(ModRM &rm);
void op32_not_rm32(ModRM &rm);



//2-byte opcodes
void op16_movzx_r16_rm8();
void op32_movzx_r32_rm8();
void op32_movzx_r32_rm16();


//helpers
void Push16(uint16_t val);
uint16_t Pop16();
void Push32(uint32_t val);
uint32_t Pop32();
void SetIndex8();
void SetIndex16();
void SetIndex32();
void CalculatePF8(uint8_t val);
void CalculatePF16(uint16_t val);
void CalculatePF32(uint32_t val);
void CalculateSF8(uint8_t val);
void CalculateSF16(uint16_t val);
void CalculateSF32(uint32_t val);
void Jmp32_near32(uint32_t off);
void Jmp16_near16(uint16_t off);
void Jmp16_near8(uint8_t off);
void Int16(uint8_t num);
void ResetSegments();
void SetSegments(uint8_t segm);
uint8_t ReadByte(uint8_t segm,uint32_t off);
uint16_t ReadWord(uint8_t segm,uint32_t off);
uint32_t ReadDword(uint8_t segm,uint32_t off);
uint64_t ReadQword(uint8_t segm,uint32_t off);
void WriteByte(uint8_t segm,uint32_t off,uint8_t val);
void WriteWord(uint8_t segm,uint32_t off,uint16_t val);
void WriteDword(uint8_t segm,uint32_t off,uint32_t val);

