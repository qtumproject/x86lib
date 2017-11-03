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


/*Opcode naming key:

sizes:
W    - Universal word (32bit by default, but can be overridden to 16bits by operand size prefix 0x66)
E    - Universal Extended Word (64bit by default, but can be overridden to 32bits by operand size prefix 0x66)
F    - Universal "far" word (48bit by default, can be overridden to 32bit by operand size prefix)
A    - Universal address (32bit by default, but can be overridden to 16bits by address size prefix 0x67)


argument types:
imm  - immediate followed by size in bits, or W for universal word
m    - immediate memory address followed by size in bits of data stored there. Always uses universal address for actual address size
rel  - immediate relative jump target followed by size in bits
rm   - Mod R/M byte, followed by size in bits or W for universal word (note: rm8 means 8 bit data size, not address)
r    - Direct register
sr   - segment register

opcode types:

op   - universal opcode
op16 - 16bit only opcode
op32 - 32bit only opcode (useful for when 16bit and 32bit functions are drastically different)

For opcodes with fixed arguments that take a fixed universal word operand, it uses W instead of the typical 'd' or 'w' suffix.
For example, cbw and cbd are separate instructions but both handled by op_cbW()

Opcodes with registers in the name can have W after them to indicate that they operate on either the 32bit or 16bit register

Sizes should be specified explicitly. It should never be like `op_inc_rm`, it should be `op_inc_rmW` 

/**This file contains the opcode function definitions and prototypes for x86CPU**/

/**NOTE! this is included INSIDE of a class, so this file is somewhat limited...**/
uint8_t Add8(uint8_t,uint8_t);
uint16_t Add16(uint16_t,uint16_t);
uint32_t Add32(uint32_t,uint32_t);
uint32_t AddW(uint32_t,uint32_t);
uint8_t Sub8(uint8_t,uint8_t);
uint16_t Sub16(uint16_t,uint16_t);
uint32_t Sub32(uint32_t,uint32_t);
uint32_t SubW(uint32_t,uint32_t);
uint8_t And8(uint8_t,uint8_t);
uint16_t And16(uint16_t,uint16_t);
uint32_t And32(uint32_t,uint32_t);
uint32_t AndW(uint32_t,uint32_t);
uint8_t Or8(uint8_t,uint8_t);
uint16_t Or16(uint16_t,uint16_t);
uint32_t Or32(uint32_t,uint32_t);
uint32_t OrW(uint32_t,uint32_t);
uint8_t Xor8(uint8_t,uint8_t);
uint16_t Xor16(uint16_t,uint16_t);
uint32_t Xor32(uint32_t,uint32_t);
uint32_t XorW(uint32_t,uint32_t);
uint8_t ShiftLogicalRight8(uint8_t,uint8_t);
uint16_t ShiftLogicalRight16(uint16_t,uint8_t);
uint32_t ShiftLogicalRight32(uint32_t,uint8_t);
uint32_t ShiftLogicalRightW(uint32_t,uint8_t);
uint8_t ShiftArithmeticRight8(uint8_t,uint8_t);
uint16_t ShiftArithmeticRight16(uint16_t,uint8_t);
uint32_t ShiftArithmeticRight32(uint32_t,uint8_t);
uint32_t ShiftArithmeticRightW(uint32_t,uint8_t);
uint8_t ShiftLogicalLeft8(uint8_t,uint8_t);
uint16_t ShiftLogicalLeft16(uint16_t,uint8_t);
uint32_t ShiftLogicalLeft32(uint32_t,uint8_t);
uint32_t ShiftLogicalLeftW(uint32_t,uint8_t);
uint8_t RotateRight8(uint8_t,uint8_t);
uint16_t RotateRight16(uint16_t,uint8_t);
uint32_t RotateRight32(uint32_t,uint8_t);
uint32_t RotateRightW(uint32_t,uint8_t);
uint8_t RotateLeft8(uint8_t,uint8_t);
uint16_t RotateLeft16(uint16_t,uint8_t);
uint32_t RotateLeft32(uint32_t,uint8_t);
uint32_t RotateLeftW(uint32_t,uint8_t);
uint8_t RotateCarryRight8(uint8_t,uint8_t);
uint16_t RotateCarryRight16(uint16_t,uint8_t);
uint32_t RotateCarryRight32(uint32_t,uint8_t);
uint32_t RotateCarryRightW(uint32_t,uint8_t);
uint8_t RotateCarryLeft8(uint8_t,uint8_t);
uint16_t RotateCarryLeft16(uint16_t,uint8_t);
uint32_t RotateCarryLeft32(uint32_t,uint8_t);
uint32_t RotateCarryLeftW(uint32_t,uint8_t);



void InstallOp(uint8_t num,opcode op,opcode *table=NULL);
void InitOpcodes();


void op_unknown();
void op_mov_r8_imm8(); //Tested, pass #1;
void op_hlt(); //Tested, pass #1;
void op_nop(); //Tested, pass #1;
void op_mov_rW_immW(); //Tested, pass #1;
void op_jmp_rel8(); //Tested, pass #1;
void op_sub_al_imm8(); //Tested, pass #1;
void op_sub_axW_immW(); //Tested, pass #1;
void op_ja_rel8();
void op_jnc_rel8();
void op_jbe_rel8();
void op_jc_rel8(); //Tested, pass #1;
void op_jz_rel8(); //Tested, pass #1;
void op_jnz_rel8(); //Tested, pass #1;
void op_jp_rel8(); //Tested, pass #1;
void op_jnp_rel8(); //Tested, pass #1;
void op_jg_rel8(); //Tested, pass #1;
void op_jge_rel8();
void op_jle_rel8();
void op_jl_rel8(); //Tested, pass #1;
void op_jo_rel8();
void op_jno_rel8();
void op_js_rel8();
void op_jns_rel8();
void op_mov_sr_rm16(); //Tested, pass #1;
void op_mov_rm16_sr(); //Tested, pass #1;
void op_pop_rW(); //Tested, pass #1;
void op_push_immW(); //Tested, pass #1;
void op_push_rW(); //Tested, pass #1;
void op_push_es(); //Tested, pass #1;
void op_push_cs(); //Tested, pass #1;
void op_push_ss(); //Tested, pass #1;
void op_push_ds(); //Tested, pass #1;
void op_pop_es(); //Tested, pass #1;
void op_pop_ss(); //Tested, pass #1;
void op_pop_ds(); //Tested, pass #1;
void op_mov_rW_rmW(); //Tested, pass #1;
void op_mov_rmW_rW(); //Tested, pass #1;
void op_call_rel16(); //Tested, pass #1;
void op_retn(); //Tested, pass #1;
void op_loop_rel8(); //Tested, pass #1;
void op_pre_cs_override(); //Tested, pass #1;
void op_pre_ds_override(); //Tested, pass #1;
void op_pre_es_override(); //Tested, pass #1;
void op_pre_ss_override(); //Tested, pass #1;
void op_movsw(); //Tested, pass #1;
void op_movsb(); //Tested, pass #1;
void op_clc();
void op_cld(); //Tested, pass #1;
void op_cli(); //Tested, pass #1;
void op_stc();
void op_std(); //Tested, pass #1;
void op_sti(); //Tested, pass #1;
void op16_rep(); //Tested, pass #1;(only rep, not conditionals)
void op_out_imm8_al(); //Tested, pass #1;
void op_out_imm8_axW(); //Tested, pass #1;
void op_call_immW_immW(); //Tested, pass #1;
void op_retf(); //Tested, pass #1;
void op_int_imm8(); //Tested, pass #1;
void op_iret(); //Tested, pass #1;
void op_into();
void op_int3();
void op_in_al_imm8();
void op_in_axW_imm8();
void op_add_al_imm8(); //Tested, pass #1;
void op_add_axW_immW(); //Tested, pass #1;
void op_sub_rm8_r8(); //Tested, pass #1;
void op_group_80(); //Tested, pass #1;
void op_sub_r8_rm8();
void op_sub_rmW_rW();
void op_sub_rW_rmW();
void op_group_81(); //Tested, pass #1;
void op_add_rm8_r8();
void op_add_rmW_rW();
void op_add_r8_rm8();
void op_add_rW_rmW();
void op_mov_al_off8();
void op_mov_axW_off16();
void op_mov_rm8_r8();
void op_mov_r8_rm8();
void op_mov_off8_al(); //Tested, pass #1;
void op_mov_off16_axW(); //Tested, pass #1;
void op_mov_m8_imm8(); //Tested, pass #1;
void op_mov_m16_immW();  //Tested, pass #1;//currently have 85 instructions or prefixes implemented(actually more, not counting group instructions)
void op_cmp_rm8_r8();
void op_cmp_rmW_rW();
void op_cmp_r8_rm8();
void op_cmp_rW_rmW();
void op_cmp_al_imm8(); //Tested, pass #1;
void op_cmp_axW_immW(); //Tested, pass #1;
void op_group_83(); //Tested, pass #1;
void op_jmp_rel16(); //Tested, pass #1
void op_jmp_immW_immW(); //Tested, pass #1
void op_group_FF();
void op_push_imm8();
void op_group_8F();
void op_salc(); //Undocumented -- Set AL on Carry
void op_cmc();
void op_cbw();
void op_aaa();
void op_daa();
void op_das();
void op_aas();
void op_aad_imm8();
void op_aam_imm8();
void op_inc_rW();
void op_dec_rW();
void op_group_FE();
void op_group_F6();
void op_group_F7();
void op_cwd();
void op_and_rm8_r8();
void op_and_rmW_rW();
void op_and_r8_rm8();
void op_and_rW_rmW();
void op_and_al_imm8();
void op_and_axW_immW();
void op_or_rm8_r8();
void op_or_rmW_rW();
void op_or_r8_rm8();
void op_or_rW_rmW();
void op_or_al_imm8();
void op_or_axW_immW();
void op_escape();
void op_cmpsb(); //Tested, pass #1, full
void op_cmpsw(); //tested, pass #1, full
void op_jcxz_rel8();
void op_adc_al_imm8();
void op_adc_axW_immW();
void op_adc_rm8_r8();
void op_adc_rmW_rW();
void op_adc_r8_rm8();
void op_adc_rW_rmW();
void op_lahf();
void op_sahf();
void op_loope_rel8();
void op_loopne_rel8();
void op_lds();
void op_les();
void op_lea();
void op_lock(); //funcitonally does nothing...
void op_xor_rm8_r8();
void op_xor_rmW_rW();
void op_xor_r8_rm8();
void op_xor_rW_rmW();
void op_xor_al_imm8();
void op_xor_axW_immW();
void op_sbb_rm8_r8();
void op_sbb_rmW_rW();
void op_sbb_r8_rm8();
void op_sbb_rW_rmW();
void op_sbb_al_imm8();
void op_sbb_axW_immW();
void op_test_al_imm8();
void op_test_axW_immW();
void op_test_rm8_r8();
void op_test_rmW_rW();
void op_xchg_rm8_r8();
void op_xchg_rmW_rW();
void op_xchg_axW_rW();
void op_group_D2();
void op_group_D3();
void op_group_D0();
void op_group_D1();
void op_lodsb();
void op_lodsw();
void op_scasb();
void op_scasw();
void op_stosb();
void op_stosw();
void op_wait();
void op_xlatb();

void op_in_al_dx();
void op_in_axW_dx();
void op_out_dx_al();
void op_out_dx_axW();


//32bit

void op32_size16();

void op32_pop_r32();
void op32_push_r32();
void op32_inc_r32();
void op32_xchg_eaxW_r32();
void op32_dec_r32();
void op32_mov_r32_imm32();
void op32_push_imm32();
void op32_mov_rm32_r32();
void op32_mov_r32_rm32();
void op32_call_rel32();
void op32_retn();
void op32_loop_rel8();
void op32_sub_eaxW_imm32();
void op32_out_imm8_eaxW();
void op32_in_eaxW_imm8();
void op32_in_eaxW_dx();
void op32_out_dx_eaxW();
void op32_movsd();
void op32_rep();
void op32_add_eaxW_imm32();
void op32_mov_m32_imm32();
void op32_add_r32_rm32();
void op32_group_83();
void op32_lodsd();
void op32_scasd();
void op32_stosd();
void op32_sub_rm32_r32();
void op32_sub_r32_rm32();
void op32_group_81();
void op32_mov_eaxW_off32();
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
void op_jcc_immW();
void op32_jcc_imm32();


/**Group Include Functions(not direct opcodes)**/
void op_sub_rm8_imm8(ModRM&); //group 0x80 /5
void op_sub_rmW_immW(ModRM&);
void op_add_rm8_imm8(ModRM&);
void op_add_rmW_immW(ModRM&);
void op_cmp_rm8_imm8(ModRM&); //Tested, pass #1
void op_cmp_rmW_immW(ModRM&);
void op_sub_rmW_imm8(ModRM&);
void op_add_rmW_imm8(ModRM&);
void op_cmp_rmW_imm8(ModRM&); //Tested, pass #1
void op_jmp_rmW(ModRM&); //Tested, pass #1
void op_jmp_m16_m16(ModRM&); //Tested, pass #1
void op_push_m16(ModRM&);
void op_pop_m16(ModRM&);
void op_inc_rm8(ModRM&);
void op_inc_rmW(ModRM&);
void op_dec_rm8(ModRM&);
void op_dec_rmW(ModRM&);
void op_div_rm8(ModRM &rm);//Tested, pass #1
void op16_div_rm16(ModRM &rm);//Tested, pass #1
void op_idiv_rm8(ModRM &rm);//Tested, pass #1
void op16_idiv_rm16(ModRM &rm);//Tested, pass #1
void op_mul_rm8(ModRM &rm);//Tested, pass #1
void op16_mul_rm16(ModRM &rm);//Tested, pass #1
void op_imul_rm8(ModRM &rm);
void op16_imul_rm16(ModRM &rm);
void op16_and_rm8_imm8(ModRM &rm);
void op_and_rmW_immW(ModRM &rm);
void op_and_rmW_imm8(ModRM &rm);
void op_and_rm8_imm8(ModRM &rm);
void op_or_rm8_imm8(ModRM& rm);
void op_or_rmW_immW(ModRM &rm);
void op_or_rmW_imm8(ModRM &rm);
void op_adc_rm8_imm8(ModRM&);
void op_adc_rmW_immW(ModRM&);
void op_adc_rmW_imm8(ModRM&);
void op_neg_rmW(ModRM&);
void op_neg_rm8(ModRM&);
void op_xor_rm8_imm8(ModRM& rm);
void op_xor_rmW_immW(ModRM &rm);
void op_xor_rmW_imm8(ModRM &rm);
void op_sbb_rm8_imm8(ModRM &rm);
void op_sbb_rmW_immW(ModRM &rm);
void op_sbb_rmW_imm8(ModRM &rm);
void op_test_rm8_imm8(ModRM &rm);
void op_test_rmW_immW(ModRM &rm);
void op_test_rmW_imm8(ModRM &rm); /**This needs to be added**/
void op_shr_rm8_cl(ModRM &rm);
void op_shr_rmW_cl(ModRM &rm);
void op_sar_rm8_cl(ModRM &rm);
void op_sar_rmW_cl(ModRM &rm);
void op_shl_rm8_cl(ModRM &rm);
void op_shl_rmW_cl(ModRM &rm);
void op_rol_rm8_cl(ModRM &rm);
void op_rol_rmW_cl(ModRM &rm);
void op_ror_rm8_cl(ModRM &rm);
void op_ror_rmW_cl(ModRM &rm);
void op_rcl_rm8_cl(ModRM &rm);
void op_rcl_rmW_cl(ModRM &rm);
void op_rcr_rm8_cl(ModRM &rm);
void op_rcr_rmW_cl(ModRM &rm);
void op_shr_rm8_1(ModRM &rm);
void op_shr_rmW_1(ModRM &rm);
void op_sar_rm8_1(ModRM &rm);
void op_sar_rmW_1(ModRM &rm);
void op_shl_rm8_1(ModRM &rm);
void op_shl_rmW_1(ModRM &rm);
void op_rol_rm8_1(ModRM &rm);
void op_rol_rmW_1(ModRM &rm);
void op_ror_rm8_1(ModRM &rm);
void op_ror_rmW_1(ModRM &rm);
void op_rcl_rm8_1(ModRM &rm);
void op_rcl_rmW_1(ModRM &rm);
void op_rcr_rm8_1(ModRM &rm);
void op_rcr_rmW_1(ModRM &rm);
void op_not_rm8(ModRM &rm);
void op_not_rmW(ModRM &rm);
void op_call_rmW(ModRM &rm);
void op_call_rmW_rmW(ModRM &rm);

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
void op_movzx_rW_rm8();
void op32_movzx_r32_rm8();
void op32_movzx_r32_rmW();


//helpers
void Push(uint32_t val);
uint32_t Pop();
void SetIndex8();
void SetIndex16();
void SetIndex32();
void SetIndex();
void CalculatePF8(uint8_t val);
void CalculatePF16(uint16_t val);
void CalculatePF32(uint32_t val);
void CalculatePF(uint32_t val);
void CalculateSF8(uint8_t val);
void CalculateSF16(uint16_t val);
void CalculateSF32(uint32_t val);
void CalculateSF(uint32_t val);
void Jmp_near(uint32_t off);
void Jmp_near32(uint32_t off);
void Jmp16_nearW(uint16_t off);
void Jmp16_near8(uint8_t off);
void Int16(uint8_t num);
void ResetSegments();
void SetSegments(uint8_t segm);
uint8_t ReadByte(uint8_t segm,uint32_t off);
uint16_t ReadWord(uint8_t segm,uint32_t off);
uint32_t ReadDword(uint8_t segm,uint32_t off);
uint32_t ReadW(uint8_t segm,uint32_t off);
uint64_t ReadQword(uint8_t segm,uint32_t off);
void WriteByte(uint8_t segm,uint32_t off,uint8_t val);
void WriteWord(uint8_t segm,uint32_t off,uint16_t val);
void WriteDword(uint8_t segm,uint32_t off,uint32_t val);
void WriteW(uint8_t segm,uint32_t off,uint32_t val);


