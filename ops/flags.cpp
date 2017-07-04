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

//jmp rel8 jcc stuff
void x86CPU::op16_ja_rel8(){ //is also jnbe
	eip++;
    if(freg.cf==0 && freg.zf==0){
		Jmp16_near8(op_cache[1]);
    }
}

void x86CPU::op16_jnc_rel8(){ //is also jae and jnb
	eip++;
    if(freg.cf==0){
        Jmp16_near8(op_cache[1]);
    }
}


void x86CPU::op16_jbe_rel8(){ //is also jna
	eip++;
    if(freg.cf==1 || freg.zf==1){
        Jmp16_near8(op_cache[1]);
    }else{
    	eip++;
    }
}

void x86CPU::op16_jc_rel8(){ //is also jb and jnae
	eip++;
    if(freg.cf==1){
        Jmp16_near8(op_cache[1]);
    }
}

void x86CPU::op16_jz_rel8(){ //is also je
	eip++;
    if(freg.zf==1){
        Jmp16_near8(op_cache[1]);
    }
}

void x86CPU::op16_jnz_rel8(){ //is also jne
	eip++;
    if(freg.zf==0){
        Jmp16_near8(op_cache[1]);
    }
}


void x86CPU::op16_jp_rel8(){ //also jpe
	eip++;
    if(freg.pf==1){
        Jmp16_near8(op_cache[1]);
    }
}

void x86CPU::op16_jnp_rel8(){ //is also jpo
	eip++;
    if(freg.pf==0){
        Jmp16_near8(op_cache[1]);
    }
}

void x86CPU::op16_jg_rel8(){ //is also jnle
	eip++;
    if(freg.sf==freg.of && freg.zf==0){
        Jmp16_near8(op_cache[1]);
    }
}


void x86CPU::op16_jge_rel8(){ //is also jnl
	eip++;
    if(freg.sf==freg.of){
        Jmp16_near8(op_cache[1]);
    }
}

void x86CPU::op16_jle_rel8(){ //is also jng
	eip++;
    if(freg.sf!=freg.of || freg.zf==1){
        Jmp16_near8(op_cache[1]);
    }
}

void x86CPU::op16_jl_rel8(){ //is also jnge
	eip++;
    if(freg.sf!=freg.of){
        Jmp16_near8(op_cache[1]);
    }
}


void x86CPU::op16_jo_rel8(){
    eip++;
	if(freg.of==1){
       Jmp16_near8(op_cache[1]);
    }
}

void x86CPU::op16_jno_rel8(){
	eip++;
    if(freg.of==0){
        Jmp16_near8(op_cache[1]);
    }
}

void x86CPU::op16_js_rel8(){ //is negative
	eip++;
    if(freg.sf==1){
        Jmp16_near8(op_cache[1]);
    }
}

void x86CPU::op16_jns_rel8(){ //is  positive
	eip++;
    if(freg.sf==0){
        Jmp16_near8(op_cache[1]);
    }
}



void x86CPU::op16_clc(){
	freg.cf=0;
}

void x86CPU::op16_cld(){
	freg.df=0;
}

void x86CPU::op16_cli(){
	freg._if=0;
}

void x86CPU::op16_stc(){
	freg.cf=1;
}

void x86CPU::op16_std(){
	freg.df=1;
}

void x86CPU::op16_sti(){
	freg._if=1;
}


void x86CPU::op16_cmc(){
    freg.cf=freg.cf^1;
}

void x86CPU::op16_lahf(){
	*regs8[AL]=*(uint8_t*)&freg;
}
void x86CPU::op16_sahf(){
	*(uint8_t*)&freg=*regs8[AL];
}











};



