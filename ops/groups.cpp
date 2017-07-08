/**
Copyright (c) 2007 - 2009 Jordan "Earlz/hckr83" Earls  <http://www.Earlz.biz.tm>
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


void x86CPU::op16_group_80(){
	eip++;
	ModRM rm(this);
	switch(rm.GetExtra()){
		case 5:
		op16_sub_rm8_imm8(rm);
		break;
		case 0:
		op16_add_rm8_imm8(rm);
		break;
		case 2:
		op16_adc_rm8_imm8(rm);
		break;
		case 7:
		op16_cmp_rm8_imm8(rm);
		break;
		case 1:
		op16_or_rm8_imm8(rm);
		break;
		case 6:
		op16_xor_rm8_imm8(rm);
		break;
		case 3:
		op16_sbb_rm8_imm8(rm);
		break;
		case 4:
		op16_and_rm8_imm8(rm);
		break;

		default:
		eip--; //to get actual opcode
		throw CpuInt_excp(UNK_IEXCP);
		break;
	}
	eip++; //for imm8

}

void x86CPU::op16_group_81(){
	eip++;
	ModRM rm(this);
	switch(rm.GetExtra()){
		case 5:
		op16_sub_rm16_imm16(rm);
		break;
		case 0:
		op16_add_rm16_imm16(rm);
		break;
		case 2:
		op16_adc_rm16_imm16(rm);
		break;
		case 7:
		op16_cmp_rm16_imm16(rm);
		break;
		case 1:
		op16_or_rm16_imm16(rm);
		break;
		case 6:
		op16_xor_rm16_imm16(rm);
		break;
		case 3:
		op16_sbb_rm16_imm16(rm);
		break;
		case 4:
		op16_and_rm16_imm16(rm);
		break;

		default:
		eip--;
		throw CpuInt_excp(UNK_IEXCP);
		break;
	}
	eip+=2; //these each have imm16
}

void x86CPU::op32_group_81(){
    eip++;
    ModRM rm(this);
    switch(rm.GetExtra()){
        case 5:
            op32_sub_rm32_imm32(rm);
            break;
        case 0:
            op32_add_rm32_imm32(rm);
            break;
        case 2:
            op32_adc_rm32_imm32(rm);
            break;
        case 7:
            op32_cmp_rm32_imm32(rm);
            break;
        case 1:
            op32_or_rm32_imm32(rm);
            break;
        case 6:
            op32_xor_rm32_imm32(rm);
            break;
        case 3:
            op32_sbb_rm32_imm32(rm);
            break;
        case 4:
            op32_and_rm32_imm32(rm);
            break;

        default:
            eip--;
            throw CpuInt_excp(UNK_IEXCP);
            break;
    }
    eip+=4; //these each have imm32
}

void x86CPU::op16_group_83() {
    eip++;
    ModRM rm(this);
    switch (rm.GetExtra()) {
        case 0:
            op16_add_rm16_imm8(rm);
            break;
        case 2:
            op16_adc_rm16_imm8(rm);
            break;
        case 5:
            op16_sub_rm16_imm8(rm);
            break;
        case 7:
            op16_cmp_rm16_imm8(rm);
            break;
        case 1:
            op16_or_rm16_imm8(rm);
            break;
        case 6:
            op16_xor_rm16_imm8(rm);
            break;
        case 3:
            op16_sbb_rm16_imm8(rm);
            break;
        case 4:
            op16_and_rm16_imm8(rm);
            break;

        default:
            eip--;
            throw CpuInt_excp(UNK_IEXCP);
            break;
    }
    eip++;
}

void x86CPU::op32_group_83() {
    eip++;
    ModRM rm(this);
    switch (rm.GetExtra()) {
        case 0:
            op32_add_rm32_imm8(rm);
            break;
        case 2:
            op32_adc_rm32_imm8(rm);
            break;
        case 5:
            op32_sub_rm32_imm8(rm);
            break;
        case 7:
            op32_cmp_rm32_imm8(rm);
            break;
        case 1:
            op32_or_rm32_imm8(rm);
            break;
        case 6:
            op32_xor_rm32_imm8(rm);
            break;
        case 3:
            op32_sbb_rm32_imm8(rm);
            break;
        case 4:
            op32_and_rm32_imm8(rm);
            break;

        default:
            eip--;
            throw CpuInt_excp(UNK_IEXCP);
            break;
    }
    eip++;
}

void x86CPU::op16_group_8F(){
	eip++;
	ModRM rm(this);
	switch(rm.GetExtra()){
		case 0:
		op16_pop_m16(rm);
		break;

		default:
		eip--;
		throw CpuInt_excp(UNK_IEXCP);
		break;
	}
}

void x86CPU::op16_group_F6(){
	eip++;
	ModRM rm(this);
	switch(rm.GetExtra()){
		case 6:
		op16_div_rm8(rm);
		break;
		case 7:
		op16_idiv_rm8(rm);
		break;
		case 4:
		op16_mul_rm8(rm);
		break;
		case 5:
		op16_imul_rm8(rm);
		break;
		case 3:
		op16_neg_rm8(rm);
		break;
		case 0:
		op16_test_rm8_imm8(rm);
		eip++;
		break;
		case 2:
		op16_not_rm8(rm);
		break;
		
		default:
		eip--;
		throw CpuInt_excp(UNK_IEXCP);
		break;
	}
}

void x86CPU::op16_group_F7(){
	eip++;
	ModRM rm(this);
	switch(rm.GetExtra()){
		case 6:
		op16_div_rm16(rm);
		break;
		case 7:
		op16_idiv_rm16(rm);
		break;
		case 4:
		op16_mul_rm16(rm);
		break;
		case 5:
		op16_imul_rm16(rm);
		break;
		case 3:
		op16_neg_rm16(rm);
		break;
		case 0:
		op16_test_rm16_imm16(rm);
		eip+=2;
		break;
		case 2:
		op16_not_rm16(rm);
		break;

		default:
		eip--;
		throw CpuInt_excp(UNK_IEXCP);
		break;
	}
}


void x86CPU::op16_group_FF(){
	eip++;
	ModRM rm(this);
	switch(rm.GetExtra()){
		case 4:
		op16_jmp_rm16(rm);
		break;
		case 5:
		op16_jmp_m16_m16(rm);
		break;
		case 6:
		op16_push_m16(rm);
		break;
		case 0:
		op16_inc_rm16(rm);
		break;
		case 1:
		op16_dec_rm16(rm);
		break;
		case 2:
		op16_call_rm16(rm);
		break;
		case 3:
		op16_call_rm16_rm16(rm);
		break;

		default:
		eip--;
		throw CpuInt_excp(UNK_IEXCP);
		break;
	}
}

void x86CPU::op32_group_FF(){
    eip++;
    ModRM rm(this);
    switch(rm.GetExtra()){
        case 4:
            op32_jmp_rm32(rm);
            break;
        case 5:
            op16_jmp_m16_m16(rm); //unsupported
            break;
        case 6:
            op32_push_m32(rm);
            break;
        case 0:
            op32_inc_rm32(rm);
            break;
        case 1:
            op32_dec_rm32(rm);
            break;
        case 2:
            op32_call_rm32(rm);
            break;
        case 3:
            op16_call_rm16_rm16(rm); //unsupported
            break;

        default:
            eip--;
            throw CpuInt_excp(UNK_IEXCP);
            break;
    }
}

void x86CPU::op16_group_FE(){
	eip++;
	ModRM rm(this);
	switch(rm.GetExtra()){
		case 0:
		op16_inc_rm8(rm);
		break;
		case 1:
		op16_dec_rm8(rm);
		break;

		default:
		eip--;
		throw CpuInt_excp(UNK_IEXCP);
		break;
	}
}

void x86CPU::op16_group_D0(){
	eip++;
	ModRM rm(this);
	switch(rm.GetExtra()){
		case 7:
		op16_sar_rm8_1(rm);
		break;
		case 4:
		op16_shl_rm8_1(rm);
		break;
		case 5:
		op16_shr_rm8_1(rm);
		break;
		case 0:
		op16_rol_rm8_1(rm);
		break;
		case 1:
		op16_ror_rm8_1(rm);
		break;
		case 3:
		op16_rcr_rm8_1(rm);
		break;
		case 2:
		op16_rcl_rm8_1(rm);
		break;
		default:
		eip--;
		throw CpuInt_excp(UNK_IEXCP);
		break;
	}
}

void x86CPU::op16_group_D1(){
	eip++;
	ModRM rm(this);
	switch(rm.GetExtra()){
		case 7:
		op16_sar_rm16_1(rm);
		break;
		case 4:
		op16_shl_rm16_1(rm);
		break;
		case 5:
		op16_shr_rm16_1(rm);
		break;
		case 0:
		op16_rol_rm16_1(rm);
		break;
		case 1:
		op16_ror_rm16_1(rm);
		break;
		case 3:
		op16_rcr_rm16_1(rm);
		break;
		case 2:
		op16_rcl_rm16_1(rm);
		break;


		default:
		eip--;
		throw CpuInt_excp(UNK_IEXCP);
		break;
	}
}

void x86CPU::op16_group_D2(){
	eip++;
	ModRM rm(this);
	switch(rm.GetExtra()){
		case 7:
		op16_sar_rm8_cl(rm);
		break;
		case 4:
		op16_shl_rm8_cl(rm);
		break;
		case 5:
		op16_shr_rm8_cl(rm);
		break;
		case 0:
		op16_rol_rm8_cl(rm);
		break;
		case 1:
		op16_ror_rm8_cl(rm);
		break;
		case 3:
		op16_rcr_rm8_cl(rm);
		break;
		case 2:
		op16_rcl_rm8_cl(rm);
		break;


		default:
		eip--;
		throw CpuInt_excp(UNK_IEXCP);
		break;
	}
}

void x86CPU::op16_group_D3(){
	eip++;
	ModRM rm(this);
	switch(rm.GetExtra()){
		case 7:
		op16_sar_rm16_cl(rm);
		break;
		case 4:
		op16_shl_rm16_cl(rm);
		break;
		case 5:
		op16_shr_rm16_cl(rm);
		break;
		case 0:
		op16_rol_rm16_cl(rm);
		break;
		case 1:
		op16_ror_rm16_cl(rm);
		break;
		case 3:
		op16_rcr_rm16_cl(rm);
		break;
		case 2:
		op16_rcl_rm16_cl(rm);
		break;


		default:
		eip--;
		throw CpuInt_excp(UNK_IEXCP);
		break;
	}
}






};



