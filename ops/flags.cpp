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


static bool jcc(int condition, volatile FLAGS &f){
    switch(condition){ //see http://www.ousob.com/ng/iapx86/ng10482.php for a good reference
        case 0:
            return f.of;
        case 1:
            return !f.of;
        case 2:
            return f.cf;
        case 3:
            return !f.cf;
        case 4:
            return f.zf;
        case 5:
            return !f.zf;
        case 6:
            return f.cf | f.zf;
        case 7:
            return !f.cf & !f.zf;
        case 8:
            return f.sf;
        case 9:
            return !f.sf;
        case 10:
            return f.pf;
        case 11:
            return !f.pf;
        case 12:
            return f.sf!=f.of;
        case 13:
            return f.sf==f.of;
        case 14:
            return (f.sf!=f.of) | f.zf;
        case 15:
            return (f.sf==f.of) & f.zf;
        default:
            throw new CpuPanic_excp("This code should not be reached", 0xFFFF);
    }
}

void x86CPU::op_jcc_rel8(){
    int cc = op_cache[0]-0x70;
    eip+=2;
    if(jcc(cc, freg)){
        Jmp_near8(op_cache[1]);
    }
}

void x86CPU::op_jcc_relW(){
    int cc = op_cache[0]-0x80;
    eip+=OperandSize() + 1; //1 to move past current opcode
    if(jcc(cc, freg)){
        Jmp_nearW(*(uint32_t*)&op_cache[1]);
    }
    eip--;
}


void x86CPU::op_clc(){
	freg.cf=0;
}

void x86CPU::op_cld(){
	freg.df=0;
}

void x86CPU::op_cli(){
	freg._if=0;
}

void x86CPU::op_stc(){
	freg.cf=1;
}

void x86CPU::op_std(){
	freg.df=1;
}

void x86CPU::op_sti(){
	freg._if=1;
}


void x86CPU::op_cmc(){
    freg.cf=freg.cf^1;
}

void x86CPU::op_lahf(){
	*regs8[AL]=*(uint8_t*)&freg;
}
void x86CPU::op_sahf(){
	*(uint8_t*)&freg=*regs8[AL];
}











};



