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

#include <stdint.h>

#ifndef X86LIB_INTERNAL_H
#define X86LIB_INTERNAL_H


namespace x86Lib{
	//32 bit register macros
static const int EAX=0;
static const int ECX=1;
static const int EDX=2;
static const int EBX=3;
static const int ESP=4;
static const int EBP=5;
static const int ESI=6;
static const int EDI=7;


//16 bit register macros
static const int AX=0;
static const int CX=1;
static const int DX=2;
static const int BX=3;
static const int SP=4;
static const int BP=5;
static const int SI=6;
static const int DI=7;

//8 bit register macros
static const int AL=0;
static const int CL=1;
static const int DL=2;
static const int BL=3;
static const int AH=4;
static const int CH=5;
static const int DH=6;
static const int BH=7;


//segment registers constants(the defaults)
static const int cES=0;
static const int cCS=1;
static const int cSS=2;
static const int cDS=3;
static const int cFS=4;
static const int cGS=5;
static const int cIS=6; //this is an imaginary segment only used for direct segment overrides
//for instance it would be used in mov [1000:bx],ax



typedef struct{
    unsigned char cf:1;
    unsigned char r0:1;
    unsigned char pf:1;
    unsigned char r1:1;
    unsigned char af:1;
    unsigned char r2:1;
    unsigned char zf:1;
    unsigned char sf:1;
    unsigned char tf:1;
    unsigned char _if:1;
    unsigned char df:1;
    unsigned char of:1;
    unsigned char iopl:2; //not yet used
    unsigned char nt:1;
    unsigned char r3:1;
}__attribute__((packed))FLAGS; //this is a better representation of flags(much easier to use)





typedef void (x86Lib::x86CPU::*opcode)(); /**You have no idea how hard it was to figure out how to do that!**/


static const uint32_t OPCODE_REAL_16=1;
static const uint32_t OPCODE_HOSTED_32=2;


//CPU Exceptions(interrupt handled)
static const uint32_t DIV0_IEXCP=0xF000; //Divide by zero exception
static const uint32_t DEBUG_IEXCP=0xF001; //Debug exception
static const uint32_t NMI_IEXCP=0xF002; //NMI
static const uint32_t BREAK_IEXCP=0xF003; //Breakpoint/int 3
static const uint32_t OVERFLOW_IEXCP=0xF004; //Overflow/into
static const uint32_t BOUNDS_IEXCP=0xF005; //Bounds Check
static const uint32_t UNK_IEXCP=0xF006; //unknown opcode
static const uint32_t UNDEV_IEXCP=0xF007; //Unknown device
static const uint32_t DOUBLE_FAULT_IEXCP=0xF008;
static const uint32_t SEG_OVERRUN_IEXCP=0xF009; //Co-processor segment overrun..(not used after i486
static const uint32_t ITSS_IEXCP=0xF00A; //Invalid TSS
static const uint32_t ISEG_IEXCP=0xF00B; //Invalid/non-existent segment
static const uint32_t STACK_IEXCP=0xF00C; //Stack Exception
static const uint32_t GPF_IEXCP=0xF00D; //GPF
static const uint32_t PAGE_FAULT_IEXCP=0xF00E;
static const uint32_t RESERVED_IEXCP=0xF00F; //Reserved by intel, so internal use?
static const uint32_t FLOAT_ERROR_IEXCP=0xF010; //Floating Point Error..
static const uint32_t ALIGN_IEXCP=0xF011; //Alignment Check...

static const uint32_t UNSUPPORTED_EXCP = 0xF0FF; //indicates something unsupported by the emulator

class CpuInt_excp{ //Used internally for handling interrupt exceptions...
	public:
	CpuInt_excp(uint32_t code_){
		code=code_;
	}
	uint32_t code;
};

/**Random support functions that are static inline'd**/

static inline uint16_t SignExtend8(uint8_t val){ //sign extend a byte to a word
	if((val&0x80)!=0){
		return 0xFF00|val;
	}else{
		return val;
	}
}

//convert signed integer into unsigned, and store top bit in store
static inline uint64_t Unsign64(uint64_t val,bool &store){

    if(val>=0x8000000000000000){
        store=1;
        return (~(val))+1;
    }else{
        store=0;
        return val;
    }

}
static inline uint32_t Unsign32(uint32_t val,bool &store){

	if(val>=0x80000000){
		store=1;
		return (~(val))+1;
	}else{
		store=0;
		return val;
	}

}

static inline uint16_t Unsign16(uint16_t val,bool &store){
	if(val>=0x8000){
		store=1;
		return (~(val))+1;
	}else{
		store=0;
		return val;
	}

}

static inline uint8_t Unsign8(uint8_t val,bool &store){
	if(val>=0x80){
		store=1;
		return (~(val))+1;
	}else{
		store=0;
		return val;
	}

}

/**Resign an unsigned integer using the store as the sign bit.
--Note, in order to combine two sign bits, just bitwise XOR(^) them!*/
static inline uint64_t Resign64(uint64_t val,bool store1){
    if((store1)==1){
        return (~(val))+1;
    }else{
        return val;
    }
}
static inline uint32_t Resign32(uint32_t val,bool store1){
	if((store1)==1){
		return (~(val))+1;
	}else{
		return val;
	}
}

static inline uint16_t Resign16(uint16_t val,bool store1){
	if((store1)==1){
		return (~(val))+1;
	}else{
		return val;
	}
}

static inline uint8_t Resign8(uint8_t val,bool store1){
	if((store1)==1){
		return (~(val))+1;
	}else{
		return val;
	}
}


};


#endif


#ifdef X86_POST_CPU

#ifndef X86LIB_INTERNAL_H_POST_CPU
#define X86LIB_INTERNAL_H_POST_CPU
#endif
#endif
