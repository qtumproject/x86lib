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
#include <sstream>


namespace x86Lib{

void x86CPU::op_nop(){ //0x90
	//do nothing
}

void x86CPU::op_hlt(){ //0xF4
	if(freg._if==0){
		throw CpuPanic_excp("HLT With IF=0; Nothing to do",CLIHLT_EXCP);
	}
}

void x86CPU::op_unknown(){
    std::ostringstream oss;
    oss << "Unknown opcode: 0x" << std::hex << (int)op_cache[0];
    throw CpuPanic_excp(oss.str(), UNK_IEXCP);
}
void x86CPU::op_unknownG(ModRM &rm){
    std::ostringstream oss;
    oss << "Unknown opcode: 0x" << std::hex << (int)op_cache[0];
    throw CpuPanic_excp(oss.str(), UNK_IEXCP);
}




//Segment overrides...
void x86CPU::op_pre_es_override(){ //0x26
	//nop
}

void x86CPU::op_pre_ds_override(){ //0x3E
	//nop
}

void x86CPU::op_pre_ss_override(){ //0x36
	//nop
}

void x86CPU::op_pre_cs_override(){ //0x2E
	//nop
}
void x86CPU::op_pre_fs_override(){ //0x64
    //nop
}
void x86CPU::op_pre_gs_override(){ //0x65
    //nop
}

void x86CPU::op_rep(){
	if(AddressSize16){ //why address size and not operand size? Makes no sense to me, but that's what the manual says
		op16_rep();
	}else{
		op32_rep();
	}
}

void x86CPU::op16_rep(){ //repe and repne..(different opcodes, but I make them possible to use the same function)
	//use a string_compares variable...
	if(*regs16[CX]==0){ //for this, not executing the instruction is almost as expensive...
		*(uint16_t*)&op_cache=ReadWord(cCS,eip+1); //note we only need 2 bytes, so don't read a whole Qword
		int i=0;
		//get size of opcode and prefixes....
		for(i=0;i<4;i++){
			switch(op_cache[i]){
				case 0x67:
				case 0x66:
				case 0x2E:
				case 0x36:
				case 0x3E:
				case 0x26:
				case 0x64:
				case 0x65:
				eip++;
				break;
				default:
				eip++;
				return;
				break;
			}
		}
		return;
	}else{
		uint32_t tmp=eip;
		uint8_t t2=op_cache[0];
		eip++;
		*(uint64_t*)&op_cache=ReadQword(cCS,eip);
		(this->*Opcodes[op_cache[0]])();
		(*regs16[CX])--;
		eip=tmp-1;
		if(string_compares==1){
			string_compares=0;
			if(t2==0xF2){ //repNE
				if(freg.zf==1){ //exit...
					eip+=2;
					return;
				}
			}else{
				if((volatile uint8_t)freg.zf==0){ //exit...
					eip+=2;
					return;
				}
			}
		}
	}
}

void x86CPU::op32_rep(){ //repe and repne..(different opcodes, but I make them possible to use the same function)
    //use a string_compares variable...
    if(regs32[ECX]==0){ //for this, not executing the instruction is almost as expensive...
        *(uint64_t*)&op_cache=ReadQword(cCS,eip+1);
        int i=0;
        //get size of opcode and prefixes....
        for(i=0;i<4;i++){
            switch(op_cache[i]){ //correct?
                case 0x67:
                case 0x66:
                case 0x2E:
                case 0x36:
                case 0x3E:
                case 0x26:
                case 0x64:
                case 0x65:
                    eip++;
                    break;
                default:
                    eip++;
                    return;
                    break;
            }
        }
        return;
    }else{
        uint32_t tmp=eip;
        uint8_t t2=op_cache[0];
        eip++;
        *(uint64_t*)&op_cache=ReadQword(cCS,eip);
        (this->*Opcodes[op_cache[0]])();
        regs32[ECX]--;
        eip=tmp-1;
        if(string_compares==1){
            string_compares=0;
            if(t2==0xF2){ //repNE
                if(freg.zf==1){ //exit...
                    eip+=4;
                    return;
                }
            }else{
                if((volatile uint8_t)freg.zf==0){ //exit...
                    eip+=4;
                    return;
                }
            }
        }
    }
}

void x86CPU::op_lock(){ //0xF0 prefix
	#ifndef X86_MULTITHREADING
	if(IsLocked()==1){
		eip--;
		return;
	}
	#endif
	Lock();
	eip++;
	*(uint64_t*)&op_cache=ReadQword(cCS,eip);
	//Add strict opcode testing for 386+
	(this->*Opcodes[op_cache[0]])();
	Unlock();
}



void x86CPU::op_cbW(){
    if(OperandSize16){
        *regs16[AX] = SignExtend8to16(*regs8[AL]);
    }else{
        regs32[EAX] = SignExtend16to32(*regs16[AL]);
    }
}



void x86CPU::op_cwE(){
	if(OperandSize16){
		if(*regs16[AX]>=0x8000){
			*regs16[DX]=0xFFFF;
		}else{
			*regs16[DX]=0;
		}
	}else{
	    if(regs32[EAX]>=0x80000000){
	        regs32[EDX]=0xFFFFFFFF;
	    }else{
	        regs32[EDX]=0;
	    }
	}
}

void x86CPU::op_escape(){
	/**This is for FPU escape opcodes
	this uses 0xD8 to 0xDF
	**/
	throw CpuInt_excp(UNDEV_IEXCP); //throw unknown device exception
}
void x86CPU::op_wait(){
	/**Does nothing...**/
	throw CpuInt_excp(UNDEV_IEXCP);
}



/**so-called undocumented opcodes:**/

/**salc -0xD6- this is will set al on carry -no arguments
log:  if cf=0 then al=0 else al=FF
**/
void x86CPU::op_salc(){ //set al on carry
    if(freg.cf==0){*regs8[AL]=0;
    }else{*regs8[AL]=0xFF;
    }

}

//operand size override to 16bit
void x86CPU::op_operand_override(){
    eip++; //increment past override byte
    OperandSize16 = true;
    *(uint64_t*)&op_cache=ReadQword(cCS,eip);
    if(op_cache[0] == 0x0F){
        //two byte opcode
        eip++;
        *(uint64_t*)&op_cache=ReadQword(cCS,eip);
        (this->*opcodes_hosted_ext[op_cache[0]])();
    }else {
        (this->*opcodes_hosted[op_cache[0]])();
    }
    OperandSize16 = false;
}

//operand size override to 16bit
void x86CPU::op_address_override(){
    eip++; //increment past override byte
    OperandSize16 = true;
    *(uint64_t*)&op_cache=ReadQword(cCS,eip);
    if(op_cache[0] == 0x0F){
        //two byte opcode
        eip++;
        *(uint64_t*)&op_cache=ReadQword(cCS,eip);
        (this->*opcodes_hosted_ext[op_cache[0]])();
    }else {
        (this->*opcodes_hosted[op_cache[0]])();
    }
    OperandSize16 = false;
}






};




