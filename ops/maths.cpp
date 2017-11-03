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


uint8_t x86CPU::Sub8(uint8_t base,uint8_t subt){
    int8_t result;
    if(subt>base){freg.cf=1;}else{freg.cf=0;}
    if((int16_t)base-subt>127 || (int16_t)base-subt<-127){
            freg.of=1;
        }else{
            freg.of=0;
        }
    result=base-subt;
    if(result==0){freg.zf=1;}else{freg.zf=0;}
    CalculatePF8(result); //do pf
    CalculateSF8(result); //do sf
	base&=0xF;
	subt&=0xF; 
    freg.af = (((base-subt) & ~0xf) != 0); //thank you http://stackoverflow.com/questions/4513746/explain-how-the-af-flag-works-in-an-x86-instructions
    return result;
}

uint16_t x86CPU::Sub16(uint16_t base,uint16_t subt){
    int16_t result;
    uint16_t mirror;
    if(subt>base){freg.cf=1;}else{freg.cf=0;}
    if((int32_t)base-subt>32767 || (int32_t)base-subt<-32767){
            freg.of=1;
        }else{
            freg.of=0;
        }
    mirror=base-subt;
    result=mirror;
    if(result==0){freg.zf=1;}else{freg.zf=0;}
    CalculatePF16(result); //do pf
    CalculateSF16(result); //do sf
	base&=0xF;
	subt&=0xF; 
    freg.af = (((base-subt) & ~0xf) != 0); 
    return mirror;
}

uint32_t x86CPU::Sub32(uint32_t base,uint32_t subt){
    int32_t result;
    uint32_t mirror;
    int64_t result64 = (int64_t) base - (int64_t) subt;
    mirror=base-subt;
    result=mirror;
    if(subt>base){freg.cf=1;}else{freg.cf=0;}
    if(result64 > INT32_MAX || result64 < INT32_MIN){
        freg.of=1;
    }else{
        freg.of=0;
    }
    if(result==0){freg.zf=1;}else{freg.zf=0;}
    CalculatePF32(result); //do pf
    CalculateSF32(result); //do sf
    base&=0xF;
    subt&=0xF;
    freg.af = (((base-subt) & ~0xf) != 0);
    return mirror;
}
uint32_t x86CPU::SubW(uint32_t base,uint32_t subt){
	if(OperandSize16){
		return Sub16(base, subt);
	}else{
		return Sub32(base, subt);
	}
}

uint8_t x86CPU::Add8(uint8_t base,uint8_t adder){
    int8_t result;
    if(adder+base>255){freg.cf=1;}else{freg.cf=0;}
    if((int16_t)base+adder>127 || (int16_t)base+adder<-127){
            freg.of=1;
        }else{
            freg.of=0;
        }
    result=base+adder;
    if(result==0){freg.zf=1;}else{freg.zf=0;}
    CalculatePF8(result); //do pf
    CalculateSF8(result); //do sf
	base&=0x0F;
	adder&=0x0F;
    freg.af = ((int16_t)base+adder > 15);
    return result;
}

uint16_t x86CPU::Add16(uint16_t base,uint16_t adder){
    int16_t result;
    if((uint32_t)adder+base>(uint32_t)65535){freg.cf=1;}else{freg.cf=0;}
    if((int32_t)base+adder>32767 || (int32_t)base+adder<-32767){
            freg.of=1;
        }else{
            freg.of=0;
        }
    result=base+adder;
    if(result==0){freg.zf=1;}else{freg.zf=0;}
    CalculatePF16(result); //do pf
    CalculateSF16(result); //do sf
	base&=0x0F;
	adder&=0x0F;
    freg.af = ((int16_t)base+adder > 15);
    return result;
}

uint32_t x86CPU::Add32(uint32_t base,uint32_t adder){
    int32_t result;
    result=(int32_t)(base+adder);
    int64_t result64 = (int64_t) base + (int64_t) adder;
    if((uint64_t)adder+base>(uint64_t)UINT32_MAX){freg.cf=1;}else{freg.cf=0;}
    if(result64 > INT32_MAX || result64 < INT32_MIN){
        freg.of=1;
    }else{
        freg.of=0;
    }
    if(result==0){freg.zf=1;}else{freg.zf=0;}
    CalculatePF32(result); //do pf
    CalculateSF32(result); //do sf
    base&=0x0F;
    adder&=0x0F;
    freg.af = result > 15;
    return result;
}

uint32_t x86CPU::AddW(uint32_t base,uint32_t adder){
	if(OperandSize16){
		return Add16(base, adder);
	}else{
		return Add32(base, adder);
	}
}



uint8_t x86CPU::And8(uint8_t base,uint8_t mask){
	freg.of=0;
	freg.cf=0;
	base=base&mask;
	CalculatePF8(base);
	CalculateSF8(base);
	if(base==0){
		freg.zf=1;
	}else{
		freg.zf=0;
	}
	return base;
}

uint16_t x86CPU::And16(uint16_t base,uint16_t mask){
	freg.of=0;
	freg.cf=0;
	base=base&mask;
	CalculatePF16(base);
	CalculateSF16(base);
	if(base==0){
		freg.zf=1;
	}else{
		freg.zf=0;
	}
	return base;
}

uint32_t x86CPU::And32(uint32_t base,uint32_t mask){
    freg.of=0;
    freg.cf=0;
    base=base&mask;
    CalculatePF32(base);
    CalculateSF32(base);
    if(base==0){
        freg.zf=1;
    }else{
        freg.zf=0;
    }
    return base;
}

uint32_t x86CPU::AndW(uint32_t base,uint32_t mask){
	if(OperandSize16){
		return And16(base, mask);
	}else{
		return And32(base, mask);
	}
}

//Not affects no flags, so just use ~

uint8_t x86CPU::Or8(uint8_t base,uint8_t mask){
	freg.of=0;
	freg.cf=0;
	base=base|mask;
	CalculatePF8(base);
	CalculateSF8(base);
	if(base==0){
		freg.zf=1;
	}else{
		freg.zf=0;
	}
	return base;
}

uint16_t x86CPU::Or16(uint16_t base,uint16_t mask){
	freg.of=0;
	freg.cf=0;
	base=base|mask;
	CalculatePF16(base);
	CalculateSF16(base);
	if(base==0){
		freg.zf=1;
	}else{
		freg.zf=0;
	}
	return base;
}

uint32_t x86CPU::Or32(uint32_t base,uint32_t mask){
    freg.of=0;
    freg.cf=0;
    base=base|mask;
    CalculatePF32(base);
    CalculateSF32(base);
    if(base==0){
        freg.zf=1;
    }else{
        freg.zf=0;
    }
    return base;
}
uint32_t x86CPU::OrW(uint32_t base,uint32_t arg){
	if(OperandSize16){
		return Or16(base, arg);
	}else{
		return Or32(base, arg);
	}
}

uint8_t x86CPU::Xor8(uint8_t base,uint8_t mask){
	freg.of=0;
	freg.cf=0;
	base=base^mask;
	CalculatePF8(base);
	CalculateSF8(base);
	if(base==0){
		freg.zf=1;
	}else{
		freg.zf=0;
	}
	return base;
}

uint16_t x86CPU::Xor16(uint16_t base,uint16_t mask){
	freg.of=0;
	freg.cf=0;
	base=base^mask;
	CalculatePF16(base);
	CalculateSF16(base);
	if(base==0){
		freg.zf=1;
	}else{
		freg.zf=0;
	}
	return base;
}
uint32_t x86CPU::Xor32(uint32_t base,uint32_t mask){
    freg.of=0;
    freg.cf=0;
    base=base^mask;
    CalculatePF32(base);
    CalculateSF32(base);
    if(base==0){
        freg.zf=1;
    }else{
        freg.zf=0;
    }
    return base;
}
uint32_t x86CPU::XorW(uint32_t base,uint32_t arg){
	if(OperandSize16){
		return Xor16(base, arg);
	}else{
		return Xor32(base, arg);
	}
}

uint8_t x86CPU::ShiftLogicalRight8(uint8_t base,uint8_t count){
	count&=0x1F; //only use bottom 5 bits
	if(count==0){
		CalculatePF8(base);
		CalculateSF8(base);
		freg.zf=1;
		return base;
	}
	freg.of=(base&0x80)>>7;
	freg.cf=(base>>(count-1))&1;
	base=base>>count;
	freg.of=freg.of^((base&0x80)>>7); //if the sign bit changed, then set it to 1
	CalculatePF8(base);
	CalculateSF8(base);
	if(base==0){
		freg.zf=1;
	}else{
		freg.zf=0;
	}
	freg.of=0;
	return base;
}

uint16_t x86CPU::ShiftLogicalRight16(uint16_t base,uint8_t count){
	count&=0x1F; //only use bottom 5 bits
	if(count==0){
		CalculatePF16(base);
		CalculateSF16(base);
		freg.zf=1;
		return base;
	}
	freg.of=(base&0x8000)>>15;
	freg.cf=(base>>(count-1))&1;
	base=base>>count;
	freg.of=freg.of^((base&0x8000)>>15); //if the sign bit changed, then set it to 1
	CalculatePF16(base);
	CalculateSF16(base);
	if(base==0){
		freg.zf=1;
	}else{
		freg.zf=0;
	}
	freg.of=0;
	return base;
}

uint32_t x86CPU::ShiftLogicalRight32(uint32_t base,uint8_t count){
    count&=0x1F; //only use bottom 5 bits
    if(count==0){
        CalculatePF32(base);
        CalculateSF32(base);
        freg.zf=1;
        return base;
    }
    freg.of=(base&0x80000000)>>31;
    freg.cf=(base>>(count-1))&1;
    base=base>>count;
    freg.of=freg.of^((base&0x80000000)>>31); //if the sign bit changed, then set it to 1
    CalculatePF32(base);
    CalculateSF32(base);
    if(base==0){
        freg.zf=1;
    }else{
        freg.zf=0;
    }
    freg.of=0;
    return base;
}

uint32_t x86CPU::ShiftLogicalRightW(uint32_t base,uint8_t arg){
	if(OperandSize16){
		return ShiftLogicalRight16(base, arg);
	}else{
		return ShiftLogicalRight32(base, arg);
	}
}

uint8_t x86CPU::ShiftArithmeticRight8(uint8_t base,uint8_t count){
	count&=0x1F; //only use bottom 5 bits
	if(count==0){
		CalculatePF8(base);
		CalculateSF8(base);
		freg.zf=1;
		return base;
	}
	freg.cf=(base>>(count-1))&1;
	if((base&0x80)!=0){
		base=(base>>count)|(~(0xFF>>count)); //this replaces displaced zero bits with the sign bit
	}else{
		base=(base>>count);
	}
	freg.of=0;
	CalculatePF8(base);
	CalculateSF8(base);
	if(base==0){
		freg.zf=1;
	}else{
		freg.zf=0;
	}
	freg.of=0;
	return base;
}
uint16_t x86CPU::ShiftArithmeticRight16(uint16_t base,uint8_t count){
	count&=0x1F; //only use bottom 5 bits
	if(count==0){
		CalculatePF16(base);
		CalculateSF16(base);
		freg.zf=1;
		return base;
	}
	freg.cf=(base>>(count-1))&1;
	if((base&0x8000)!=0){
		base=(base>>count)|(~(0xFFFF>>count)); //this replaces displaced zero bits with the sign bit
	}else{
		base=(base>>count);
	}
	freg.of=0;
	CalculatePF16(base);
	CalculateSF16(base);
	if(base==0){
		freg.zf=1;
	}else{
		freg.zf=0;
	}
	freg.of=0;
	return base;
}

uint32_t x86CPU::ShiftArithmeticRight32(uint32_t base,uint8_t count){
    count&=0x1F; //only use bottom 5 bits
    if(count==0){
        CalculatePF32(base);
        CalculateSF32(base);
        freg.zf=1;
        return base;
    }
    freg.cf=(base>>(count-1))&1;
    if((base&0x80000000)!=0){
        base=(base>>count)|(~(0xFFFFFFFF>>count)); //this replaces displaced zero bits with the sign bit
    }else{
        base=(base>>count);
    }
    freg.of=0;
    CalculatePF32(base);
    CalculateSF32(base);
    if(base==0){
        freg.zf=1;
    }else{
        freg.zf=0;
    }
    freg.of=0;
    return base;
}

uint32_t x86CPU::ShiftArithmeticRightW(uint32_t base,uint8_t arg){
	if(OperandSize16){
		return ShiftArithmeticRight16(base, arg);
	}else{
		return ShiftArithmeticRight32(base, arg);
	}
}

uint8_t x86CPU::ShiftLogicalLeft8(uint8_t base,uint8_t count){
	count&=0x1F; //only use bottom 5 bits
	if(count==0){
		CalculatePF8(base);
		CalculateSF8(base);
		freg.zf=1;
		return base;
	}
	freg.of=(base&0x80)>>7;
	freg.cf=((base<<(count-1))&0x80)>>7;
	base=base<<count;
	freg.of=freg.of^((base&0x80)>>7); //if the sign bit changed, then set it to 1
	CalculatePF8(base);
	CalculateSF8(base);
	if(base==0){
		freg.zf=1;
	}else{
		freg.zf=0;
	}
	freg.of=0;
	return base;
}

uint16_t x86CPU::ShiftLogicalLeft16(uint16_t base,uint8_t count){
	count&=0x1F; //only use bottom 5 bits
	if(count==0){
		CalculatePF16(base);
		CalculateSF16(base);
		freg.zf=1;
		return base;
	}
	freg.of=(base&0x8000)>>15;
	freg.cf=((base<<(count-1))&0x8000)>>15;
	base=base<<count;
	freg.of=freg.of^((base&0x8000)>>15); //if the sign bit changed, then set it to 1
	CalculatePF16(base);
	CalculateSF16(base);
	if(base==0){
		freg.zf=1;
	}else{
		freg.zf=0;
	}
	return base;
}

uint32_t x86CPU::ShiftLogicalLeft32(uint32_t base,uint8_t count){
    count&=0x1F; //only use bottom 5 bits
    if(count==0){
        CalculatePF32(base);
        CalculateSF32(base);
        freg.zf=1;
        return base;
    }
    freg.of=(base&0x80000000)>>31;
    freg.cf=((base<<(count-1))&0x80000000)>>31;
    base=base<<count;
    freg.of=freg.of^((base&0x80000000)>>31); //if the sign bit changed, then set it to 1
    CalculatePF32(base);
    CalculateSF32(base);
    if(base==0){
        freg.zf=1;
    }else{
        freg.zf=0;
    }
    return base;
}

uint32_t x86CPU::ShiftLogicalLeftW(uint32_t base,uint8_t arg){
	if(OperandSize16){
		return ShiftLogicalLeft16(base, arg);
	}else{
		return ShiftLogicalLeft32(base, arg);
	}
}

/**ToDo: Possibly adapt BOCHS source so that we avoid this loop crap...**/
uint8_t x86CPU::RotateRight8(uint8_t base,uint8_t count){
    count&=0x1F; //only use bottom 5 bits
	freg.of=(base&0x80)>>7;
	while(count>0){
		freg.cf=(base&0x01);
		base=(freg.cf<<7)|(base>>1);
		count--;
	}
	freg.of=freg.of^((base&0x80)>>7);
	return base;
}

uint16_t x86CPU::RotateRight16(uint16_t base,uint8_t count){
    count&=0x1F; //only use bottom 5 bits
	freg.of=(base&0x8000)>>15;
	while(count>0){
		freg.cf=(base&0x01);
		base=(freg.cf<<15)|(base>>1);
		count--;
	}
	freg.of=freg.of^((base&0x80)>>15);
	return base;
}
uint32_t x86CPU::RotateRight32(uint32_t base,uint8_t count){
    count&=0x1F; //only use bottom 5 bits
    freg.of=(base&0x80000000)>>31;
    while(count>0){
        freg.cf=(base&0x01);
        base=(freg.cf<<31)|(base>>1);
        count--;
    }
    freg.of=freg.of^((base&0x80)>>31);
    return base;
}

uint32_t x86CPU::RotateRightW(uint32_t base,uint8_t arg){
	if(OperandSize16){
		return RotateRight16(base, arg);
	}else{
		return RotateRight32(base, arg);
	}
}

uint8_t x86CPU::RotateLeft8(uint8_t base,uint8_t count){
    count&=0x1F; //only use bottom 5 bits
	freg.of=(base&0x80)>>7;
	while(count>0){
		freg.cf=(base&0x80)>>7;
		base=(freg.cf)|(base<<1);
		count--;
	}
	freg.of=freg.of^((base&0x80)>>7);
	return base;
}

uint16_t x86CPU::RotateLeft16(uint16_t base,uint8_t count){
    count&=0x1F; //only use bottom 5 bits
	freg.of=(base&0x8000)>>15;
	while(count>0){
		freg.cf=(base&0x8000)>>15;
		base=(freg.cf)|(base<<1);
		count--;
	}
	freg.of=freg.of^((base&0x8000)>>15);
	return base;
}

uint32_t x86CPU::RotateLeft32(uint32_t base,uint8_t count){
    count&=0x1F; //only use bottom 5 bits
    freg.of=(base&0x80000000)>>31;
    while(count>0){
        freg.cf=(base&0x80000000)>>31;
        base=(freg.cf)|(base<<1);
        count--;
    }
    freg.of=freg.of^((base&0x80000000)>>31);
    return base;
}

uint32_t x86CPU::RotateLeftW(uint32_t base,uint8_t arg){
	if(OperandSize16){
		return RotateLeft16(base, arg);
	}else{
		return RotateLeft32(base, arg);
	}
}

uint8_t x86CPU::RotateCarryLeft8(uint8_t base,uint8_t count){
    count&=0x1F; //only use bottom 5 bits
   freg.of=(base&0x80)>>7;
   while(count>0){
      freg.r0=freg.cf; //reserved bit as a temp variable...
      freg.cf=(base&0x80)>>7;
      base=(base<<1);
      base|=freg.r0; //puts (old)CF in it's place
      count--;
   }
   freg.of=freg.of^((base&0x80)>>7);
   return base;
}

uint16_t x86CPU::RotateCarryLeft16(uint16_t base,uint8_t count){
    count&=0x1F; //only use bottom 5 bits
	freg.of=(base&0x8000)>>15;
	while(count>0){
		freg.r0=freg.cf; //reserved bit as a temp variable...
		freg.cf=(base&0x8000)>>15;
		base=(base<<1);
		base=(base&0xFFFE)|freg.r0; //zeros the 0 bit and puts (old)CF in it's place
		count--;
	}
	freg.of=freg.of^((base&0x8000)>>15);
	return base;
}

uint32_t x86CPU::RotateCarryLeft32(uint32_t base,uint8_t count){
    count&=0x1F; //only use bottom 5 bits
    freg.of=(base&0x80000000)>>31;
    while(count>0){
        freg.r0=freg.cf; //reserved bit as a temp variable...
        freg.cf=(base&0x80000000)>>31;
        base=(base<<1);
        base=(base&0xFFFFFFFE)|freg.r0; //zeros the 0 bit and puts (old)CF in it's place
        count--;
    }
    freg.of=freg.of^((base&0x80000000)>>31);
    return base;
}

uint32_t x86CPU::RotateCarryLeftW(uint32_t base,uint8_t arg){
	if(OperandSize16){
		return RotateCarryLeft16(base, arg);
	}else{
		return RotateCarryLeft32(base, arg);
	}
}

uint8_t x86CPU::RotateCarryRight8(uint8_t base,uint8_t count){
    count&=0x1F; //only use bottom 5 bits
	freg.of=(base&0x80)>>7;
	while(count>0){
		freg.r0=freg.cf;
		freg.cf=(base&0x01);
		base=(freg.r0<<7)|(base>>1);
		count--;
	}
	freg.of=freg.of^((base&0x80)>>7);
	return base;
}

uint16_t x86CPU::RotateCarryRight16(uint16_t base,uint8_t count){
    count&=0x1F; //only use bottom 5 bits
	freg.of=(base&0x8000)>>15;
	while(count>0){
		freg.r0=freg.cf;
		freg.cf=(base&0x01);
		base=(freg.r0<<15)|(base>>1);
		count--;
	}
	freg.of=freg.of^((base&0x8000)>>15);
	return base;
}

uint32_t x86CPU::RotateCarryRight32(uint32_t base,uint8_t count){
    count&=0x1F; //only use bottom 5 bits
    freg.of=(base&0x80000000)>>31;
    while(count>0){
        freg.r0=freg.cf;
        freg.cf=(base&0x01);
        base=(freg.r0<<31)|(base>>1);
        count--;
    }
    freg.of=freg.of^((base&0x80000000)>>31);
    return base;
}

uint32_t x86CPU::RotateCarryRightW(uint32_t base,uint8_t arg){
	if(OperandSize16){
		return RotateCarryRight16(base, arg);
	}else{
		return RotateCarryRight32(base, arg);
	}
}







void x86CPU::op_sub_al_imm8(){ //0x2C
	*regs8[AL]=Sub8(*regs8[AL],op_cache[1]);
	eip++;
}

void x86CPU::op_sub_axW_immW(){ //0x2D
	WriteReg(AX, Sub(Reg(AX),ImmW()));
}


void x86CPU::op_sub_rm8_r8(){
	eip++;
	ModRM rm8(this);
	rm8.WriteByter(Sub8(rm8.ReadByter(),*regs8[rm8.GetExtra()]));
}

void x86CPU::op_sub_rmW_rW(){
	eip++;
	ModRM rm(this);
	rm.Write(Sub(rm.Read(),Reg(rm.GetExtra()])));
}
void x86CPU::op16_sub_r8_rm8(){
	eip++;
	ModRM rm8(this);
	*regs8[rm8.GetExtra()]=Sub8(*regs8[rm8.GetExtra()],rm8.ReadByter());
}

void x86CPU::op_sub_rW_rmW(){
	eip++;
	ModRM rm(this);
	WriteReg(rm.GetExtra(), Sub(Reg(rm.GetExtra()), rm.Read());
}

void x86CPU::op16_sub_rm8_imm8(ModRM &rm8){ //group 0x80 /5
	rm8.WriteByter(Sub8(rm8.ReadByter(),ReadByte(cCS,eip+rm8.GetLength())));
}

void x86CPU::op_sub_rmW_immW(ModRM &rm){ //Group 0x81 /5
	rm.Write(Sub(rm.Read(), Read(cCS,eip+rm.GetLength())));
}

void x86CPU::op_sub_rmW_imm8(ModRM &rm){ //group 0x83 /5
	rm.Write(Sub(rm.Read(), SignExtend8to32(ReadByte(cCS,eip+rm.GetLength()))));
}

/****/
void x86CPU::op_sbb_al_imm8(){
	*regs8[AL]=Sub8(*regs8[AL],op_cache[1]-freg.cf);
	eip++;
}

void x86CPU::op_sbb_axW_immW(){
	WriteReg(AX, Sub(Reg(AX), ImmW()-freg.cf));
}

void x86CPU::op_sbb_rmW_rW(){
	eip++;
	ModRM rm(this);
	rm.Write(Sub(rm.Read(), Reg(rm.GetExtra())-freg.cf)); //is this correct with CF subtracting as a 32bit integer? 
}
void x86CPU::op_sbb_r8_rm8(){
	eip++;
	ModRM rm8(this);
	*regs8[rm8.GetExtra()]=Sub8(*regs8[rm8.GetExtra()],rm8.ReadByte()-freg.cf);
}

void x86CPU::op_sbb_rW_rmW(){
	eip++;
	ModRM rm(this);
	WriteReg(rm.GetExtra(), Sub(Reg(rm.GetExtra()), rm.Read()-freg.cf));
}

void x86CPU::op16_sbb_rm8_imm8(ModRM &rm8){ //group 0x80
	rm8.WriteByte(Sub8(rm8.ReadByte(),ReadByte(cCS,eip+rm8.GetLength())-freg.cf));
}

void x86CPU::op_sbb_rmW_immW(ModRM &rm){ //Group 0x81
	rm.Write(Sub(rm.Read(), ReadW(cCS,eip+rm.GetLength())-freg.cf));
}

void x86CPU::op_sbb_rmW_imm8(ModRM &rm){ //group 0x83
	rm.Write(Sub(rm.Read(), SignExtend8to32(ReadByte(cCS,eip+rm.GetLength())-freg.cf)));
}


void x86CPU::op_dec_rW(){ //0x48+r
	freg.r0=freg.cf;
	WriteReg(op_cache[0]-0x48, Sub(Reg(op_cache[0]-0x48), 1));
	freg.cf=freg.r0;
}

void x86CPU::op_dec_rm8(ModRM& rm){
	freg.r0=freg.cf;
	rm.WriteByte(Sub8(rm.ReadByte(),1));
	freg.cf=freg.r0;
}

void x86CPU::op_dec_rmW(ModRM& rm){
	freg.r0=freg.cf;
	rm.Write(Sub(rm.Read(),1));
	freg.cf=freg.r0;
}


//cmp and sub are so similar, that they are both going in here...
void x86CPU::op_cmp_al_imm8(){
	Sub8(*regs8[AL],op_cache[1]);
	eip++;
}

void x86CPU::op_cmp_axW_immW(){
	Sub(Reg(AX), ImmW());
}

void x86CPU::op_cmp_rm8_r8(){
	eip++;
	ModRM rm(this);
	Sub8(rm.ReadByter(),*regs8[rm.GetExtra()]);
}

void x86CPU::op_cmp_rmW_rW(){
	eip++;
	ModRM rm(this);
	Sub(rm.Read(),Reg(rm.GetExtra()));
}

void x86CPU::op_cmp_r8_rm8(){
	eip++;
	ModRM rm(this);
	Sub8(*regs8[rm.GetExtra()],rm.ReadByter());
}

void x86CPU::op_cmp_rW_rmW(){
	eip++;
	ModRM rm(this);
	Sub(Reg(rm.GetExtra()),rm.Read());
}

void x86CPU::op16_cmp_rm8_imm8(ModRM &rm){ //group 80 /7
	Sub8(rm.ReadByter(),ReadByte(cCS,eip+rm.GetLength()));
}

void x86CPU::op_cmp_rmW_immW(ModRM &rm){ //group 81 /7
	Sub(rm.ReadW(), ReadW(cCS, eip+rm.GetLength()));
}

void x86CPU::op_cmp_rmW_imm8(ModRM &rm){ //group 83 /7
	Sub(rm.ReadW(),SignExtend8to32(ReadByte(cCS, eip+rm.GetLength())));
}


void x86CPU::op_add_al_imm8(){
	*regs8[AL]=Add8(*regs8[AL],op_cache[1]);
	eip++;
}

void x86CPU::op_add_axW_immW(){
	WriteReg(AX, Add(Reg(AX), ImmW()));
}

void x86CPU::op_add_rm8_r8(){
	eip++;
	ModRM rm8(this);
	rm8.WriteByte(Add8(rm8.ReadByte(),*regs8[rm8.GetExtra()]));
}

void x86CPU::op_add_rmW_rW(){
	eip++;
	ModRM rm(this);
	rm.WriteW(Add(rm.ReadW(),Reg(rm.GetExtra())));
}


void x86CPU::op_add_r8_rm8(){
	eip++;
	ModRM rm(this);
	*regs8[rm.GetExtra()]=Add8(*regs8[rm.GetExtra()],rm.ReadByter());
}

void x86CPU::op_add_rW_rmW(){
	eip++;
	ModRM rm(this);
	WriteReg(rm.GetExtra(), Add(Reg(rm.GetExtra(), rm.ReadW())));
}

void x86CPU::op_add_rm8_imm8(ModRM &rm){ //Group 0x80 /0
	rm.WriteByte(Add8(rm.ReadWord(),ReadByte(cCS,eip+rm.GetLength())));
}

void x86CPU::op_add_rmW_immW(ModRM &rm){ //Group 0x81 /0
	rm.WriteW(Add(rm.ReadW(), ReadW(cCS, eip+rm.GetLength())));
}

void x86CPU::op_add_rmW_imm8(ModRM &rm){ //group 0x83 /0
	rm.WriteW(Add(rm.ReadW(),SignExtend8to32(ReadByte(cCS, eip+rm.GetLength()))));
}


/****/
void x86CPU::op_adc_al_imm8(){
	*regs8[AL]=Add8(*regs8[AL],op_cache[1]+freg.cf);
	eip++;
}

void x86CPU::op_adc_axW_immW(){
	WriteReg(AX, Add(Reg(AX), ImmW() + freg.cf));
}

void x86CPU::op_adc_rm8_r8(){
	eip++;
	ModRM rm8(this);
	rm8.WriteByter(Add8(rm8.ReadByter(),*regs8[rm8.GetExtra()]+freg.cf));
}

void x86CPU::op_adc_rmW_rW(){
	eip++;
	ModRM rm(this);
	rm.WriteW(Add(rm.ReadW(),Reg(rm.GetExtra())+freg.cf));
}

void x86CPU::op16_adc_r8_rm8(){
	eip++;
	ModRM rm(this);
	*regs8[rm.GetExtra()]=Add8(*regs8[rm.GetExtra()],rm.ReadByte()+freg.cf);
}

void x86CPU::op_adc_rW_rmW(){
	eip++;
	ModRM rm(this);
	WriteReg(rm.GetExtra(), Add(Reg(rm.GetExtra()), rm.ReadW() + freg.cf));
}

void x86CPU::op16_adc_rm8_imm8(ModRM &rm){ //Group 0x80 /2
	rm.WriteByter(Add8(rm.ReadWordr(),ReadByte(cCS,eip+rm.GetLength())+freg.cf));
}

void x86CPU::op_adc_rmW_immW(ModRM &rm){ //Group 0x81 /2
	rm.WriteW(Add(rm.ReadW(),ReadW(cCS, eip + rm.GetLength()) + freg.cf));
}

void x86CPU::op_adc_rmW_imm8(ModRM &rm){ //group 0x83 /2
	rm.WriteW(Add(rm.ReadW(),SignExtend8to32(ReadByte(cCS, eip + rm.GetLength())) + freg.cf));
}


void x86CPU::op16_inc_rW(){ //0x40+r
	freg.r0=freg.cf;
	WriteReg(op_cache[0]-0x40), Add(Reg(op_cache[0] - 0x40), 1);
	freg.cf=freg.r0;
}

void x86CPU::op_inc_rm8(ModRM &rm){
	freg.r0=freg.cf; //yay for reserved flags! TODO check for qtum
	rm.WriteByte(Add8(rm.ReadByte(),1));
	freg.cf=freg.r0;
}

void x86CPU::op_inc_rmW(ModRM &rm){
	freg.r0=freg.cf;
	rm.WriteW(Add(rm.ReadW(), 1));
	freg.cf=freg.r0;
}

void x86CPU::op_neg_rm8(ModRM &rm){
	uint8_t tmp=rm.ReadByter();
	if(tmp==0xFF){
		freg.of=1;
		return;
	}
	rm.WriteByte(Sub8(0,tmp));
	if(tmp==0){
		freg.cf=0;
	}else{
		freg.cf=1;
	}
}
void x86CPU::op_neg_rmW(ModRM &rm){
	uint32_t tmp=rm.ReadW();
	rm.WriteW(SubW(0, tmp));
	freg.cf = tmp != 0;
}

void x86CPU::op_div_rm8(ModRM &rm){
	if(rm.ReadByter()==0){
		throw CpuInt_excp(DIV0_IEXCP);
	}
	if(((*regs16[AX])/rm.ReadByter())>0xFF){
		throw CpuInt_excp(DIV0_IEXCP); 
	}
	*regs8[AL]=(*regs16[AX])/rm.ReadByter();
	*regs8[AH]=(*regs16[AX])%rm.ReadByter();
}

void x86CPU::op16_div_rm16(ModRM &rm){
	if(rm.ReadWord()==0){
		throw CpuInt_excp(DIV0_IEXCP);
	}
	if((((*regs16[DX]<<16)|(*regs16[AX]))/rm.ReadWord())>0xFFFF){
		throw CpuInt_excp(DIV0_IEXCP);
	}
	*regs16[AX]=((*regs16[DX]<<16)|(*regs16[AX]))/rm.ReadWord();
	*regs16[DX]=((*regs16[DX]<<16)|(*regs16[AX]))%rm.ReadWord();
}

void x86CPU::op32_div_rm32(ModRM &rm){
    if(rm.ReadDwordr()==0){
        throw CpuInt_excp(DIV0_IEXCP);
    }
    if((((uint64_t)regs32[EDX]<<32)|((uint64_t)regs32[EAX])/(uint64_t)rm.ReadDword()) > 0xFFFFFFFF){
        throw CpuInt_excp(DIV0_IEXCP);
    }
    regs32[EAX]=(((uint64_t)regs32[EDX]<<32)|(regs32[EAX]))/rm.ReadDword();
    regs32[EDX]=(((uint64_t)regs32[EDX]<<32)|(regs32[EAX]))%rm.ReadDword();
}


void x86CPU::op_idiv_rm8(ModRM &rm){
	if(rm.ReadByte()==0){
		throw CpuInt_excp(DIV0_IEXCP);
	}
	uint8_t tmp=rm.ReadByte();
	bool store1,store2;

	tmp=Unsign8(tmp,store1);
	*regs16[AX]=Unsign16(*regs16[AX],store2);
	if(((*regs16[AX])/tmp)>0xFF){
		throw CpuInt_excp(DIV0_IEXCP);
	}

	*regs8[AL]=(*regs16[AX])/tmp;
	*regs8[AH]=(*regs16[AX])%tmp;

	*regs8[AL]=Resign8(*regs8[AL],store1^store2);
}

void x86CPU::op16_idiv_rm16(ModRM &rm){
	if(rm.ReadWord()==0){
		throw CpuInt_excp(DIV0_IEXCP);
	}
	uint16_t tmp=rm.ReadWord();
	bool store1,store2;
	tmp=Unsign16(tmp,store1);
	uint32_t tmp2=Unsign32(((*regs16[DX])<<16)|(*regs16[AX]),store2);

	if((tmp2/tmp)>0xFFFF){
		throw CpuInt_excp(DIV0_IEXCP);
	}
	*regs16[AX]=tmp2/tmp;
	*regs16[DX]=tmp2%tmp;

	*regs16[AX]=Resign16(*regs16[AX],store1^store2);
}

void x86CPU::op32_idiv_rm32(ModRM &rm){
    uint32_t tmp= rm.ReadDword();
    if(tmp == 0){
        throw CpuInt_excp(DIV0_IEXCP);
    }
    bool store1,store2;
    tmp=Unsign32(tmp,store1);
    uint64_t tmp2=Unsign64((((uint64_t)regs32[EDX])<<32)|(regs32[EAX]),store2);

    if((((uint64_t)tmp2)/((uint64_t)tmp))>0xFFFFFFFF){
        throw CpuInt_excp(DIV0_IEXCP);
    }
    regs32[EAX]=tmp2/tmp;
    regs32[EDX]=tmp2%tmp;

    regs32[EAX]=Resign32(regs32[EAX],store1^store2);
}



void x86CPU::op_mul_rm8(ModRM &rm){
    *regs16[AX]=(*regs8[AL])*rm.ReadByte();
    if((*regs8[AH])>0){ //if tophalf of result has anything in it
        freg.cf=1;
        freg.of=1;
    }else{
        freg.cf=0;
        freg.of=0;
    }
}


void x86CPU::op16_mul_rm16(ModRM &rm){
	uint32_t result;
    result=(*regs16[AX])*rm.ReadWord();
    *regs16[AX]=result&0x0000FFFF;
    *regs16[DX]=(result&0xFFFF0000)>>16;
    if((*regs16[DX])>0){ //if tophalf of result has anything in it
        freg.cf=1;
        freg.of=1;
    }else{
        freg.cf=0;
        freg.of=0;
    }
}

void x86CPU::op32_mul_rm32(ModRM &rm){
    uint64_t result;
    result=(regs32[EAX])*(uint64_t)rm.ReadDword();
    regs32[EAX]=result&0x00000000FFFFFFFF;
    regs32[EDX]=(result&0xFFFFFFFF00000000)>>32;
    if((regs32[EDX])>0){ //if tophalf of result has anything in it
        freg.cf=1;
        freg.of=1;
    }else{
        freg.cf=0;
        freg.of=0;
    }
}

void x86CPU::op_imul_rm8(ModRM &rm){
	bool store1,store2;
	*regs16[AX]=Unsign8(*regs8[AL],store1)*Unsign8(rm.ReadByte(),store2);
	if(*regs8[AX]>0){
		freg.of=1;
		freg.cf=1;
	}else{
		freg.of=0;
		freg.cf=0;
	}
	*regs16[AX]=Resign16(*regs16[AX],store1^store2);
}

void x86CPU::op16_imul_rm16(ModRM &rm){
	bool store1,store2;
	uint32_t result=Unsign16(*regs16[AX],store1)*Unsign16(rm.ReadWordr(),store2);
	if((result&0xFFFF0000)>0){
		freg.of=1;
		freg.cf=1;
	}else{
		freg.of=0;
		freg.cf=0;
	}
	result=Resign32(result,store1^store2);
	*regs16[DX]=(result&0xFFFF0000)>>16;
	*regs16[AX]=(result&0xFFFF);
}

void x86CPU::op32_imul_rm32(ModRM &rm){
    bool store1,store2;
    uint64_t result=Unsign32(regs32[EAX],store1) * (uint64_t)Unsign32(rm.ReadDwordr(),store2);
    if((result&0xFFFFFFFF00000000)>0){
        freg.of=1;
        freg.cf=1;
    }else{
        freg.of=0;
        freg.cf=0;
    }
    result=Resign64(result,store1^store2);
    regs32[EDX]=(result&0xFFFFFFFF00000000)>>32;
    regs32[EAX]=(result&0xFFFFFFFF);
}



void x86CPU::op_and_rm8_r8(){
	eip++;
	ModRM rm(this);
	rm.WriteByte(And8(rm.ReadByte(),*regs8[rm.GetExtra()]));
}

void x86CPU::op_and_rmW_rW(){
	eip++;
	ModRM rm(this);
	rm.WriteW(And(rm.ReadW(),Reg(rm.GetExtra())));
}

void x86CPU::op16_and_r8_rm8(){
	eip++;
	ModRM rm(this);
	*regs8[rm.GetExtra()]=And8(*regs8[rm.GetExtra()],rm.ReadByter());
}

void x86CPU::op_and_r_rmW(){
	eip++;
	ModRM rm(this);
	WriteReg(rm.GetExtra(), And(Reg(rm.GetExtra()), rm.ReadW()));
}

void x86CPU::op_and_al_imm8(){
	eip++;
	*regs8[AL]=And8(*regs8[AL],op_cache[1]);
}

void x86CPU::op_and_axW_immW(){
	WriteReg(AX, And(Reg(AX), ImmW()));
}

void x86CPU::op_and_rm8_imm8(ModRM& rm){
	rm.WriteByter(And8(rm.ReadByter(),ReadByte(cCS,eip+rm.GetLength())));
}

void x86CPU::op_and_rmW_immW(ModRM& rm){
	rm.WriteW(And(rm.ReadW(), ReadW(cCS, eip + rm.GetLength())));
}

void x86CPU::op_and_rmW_imm8(ModRM& rm){
	rm.WriteW(And(rm.ReadW(), SignExtend8to32(ReadByte(cCS, eip + rm.GetLength()))));
}

void x86CPU::op_or_rm8_r8(){
	eip++;
	ModRM rm(this);
	rm.WriteByter(Or8(rm.ReadByter(),*regs8[rm.GetExtra()]));
}

void x86CPU::op_or_rmW_rW(){
	eip++;
	ModRM rm(this);
	rm.WriteW(Or(rm.ReadW(),Reg(rm.GetExtra())));
}

void x86CPU::op_or_r8_rm8(){
	eip++;
	ModRM rm(this);
	*regs8[rm.GetExtra()]=Or8(*regs8[rm.GetExtra()],rm.ReadByter());
}

void x86CPU::op_or_rW_rmW(){
	eip++;
	ModRM rm(this);
	WriteReg(rm.GetExtra(), Or(Reg(rm.GetExtra(), rm.ReadW())));
}

void x86CPU::op_or_al_imm8(){
	eip++;
	*regs8[AL]=Or8(*regs8[AL],op_cache[1]);
}

void x86CPU::op_or_axW_immW(){
	WriteReg(AX, Or(Reg(AX), ImmW()));
}

void x86CPU::op_or_rm8_imm8(ModRM& rm){
	rm.WriteByter(Or8(rm.ReadByter(),ReadByte(cCS,eip+rm.GetLength())));
}

void x86CPU::op_or_rmW_immW(ModRM& rm){
	rm.WriteW(Or(rm.ReadW(), ReadW(cCS, eip + rm.GetLength())));
}

void x86CPU::op_or_rmW_imm8(ModRM& rm){
	rm.WriteW(Or(rm.ReadW(), SignExtend8to32(ReadByte(cCS, eip + rm.GetLength()))));
}


void x86CPU::op_xor_rm8_r8(){
	eip++;
	ModRM rm(this);
	rm.WriteByter(Xor8(rm.ReadByter(),*regs8[rm.GetExtra()]));
}

void x86CPU::op_xor_rmW_rW(){
	eip++;
	ModRM rm(this);
	rm.WriteW(Xor(rm.ReadW(), Reg(rm.GetExtra())));
}

void x86CPU::op_xor_r8_rm8(){
	eip++;
	ModRM rm(this);
	*regs8[rm.GetExtra()]=Xor8(*regs8[rm.GetExtra()],rm.ReadByter());
}

void x86CPU::op_xor_rW_rmW(){
	eip++;
	ModRM rm(this);
	WriteReg(rm.GetExtra(), XorW(Reg(rm.GetExtra()), rm.ReadW()));
}

void x86CPU::op16_xor_al_imm8(){
	eip++;
	*regs8[AL]=Xor8(*regs8[AL],op_cache[1]);
}

void x86CPU::op_xor_axW_immW(){
	WriteReg(AX, XorW(Reg(AX), ImmW()));
}

void x86CPU::op_xor_rm8_imm8(ModRM& rm){
	rm.WriteByter(Xor8(rm.ReadByter(),ReadByte(cCS,eip+rm.GetLength())));
}

void x86CPU::op_xor_rmW_immW(ModRM& rm){
	rm.WriteW(Xor(rm.ReadW(), ReadW(cCS, eip + rm.GetLength())));
}

void x86CPU::op_xor_rmW_imm8(ModRM& rm){
	rm.WriteW(Xor(rm.ReadW(),SignExtend8to32(ReadByte(cCS, eip + rm.GetLength()))));
}

void x86CPU::op16_test_rm8_r8(){
	eip++;
	ModRM rm(this);
	And8(rm.ReadByter(),*regs8[rm.GetExtra()]);
}

void x86CPU::op_test_rmW_rW(){
	eip++;
	ModRM rm(this);
	And(rm.ReadW(),Reg(rm.GetExtra()));
}

void x86CPU::op_test_al_imm8(){
	eip++;
	And8(*regs8[AL],op_cache[1]);
}
void x86CPU::op_test_axW_immW(){
	And(Reg(AX), ImmW());
}

void x86CPU::op16_test_rm8_imm8(ModRM& rm){
	And8(rm.ReadByter(),ReadByte(cCS,eip+rm.GetLength()));
}

void x86CPU::op_test_rmW_immW(ModRM& rm){
	And(rm.ReadW(),ReadW(cCS, eip + rm.GetLength()));
}

void x86CPU::op_test_rmW_imm8(ModRM& rm){
	And(rm.ReadW(),SignExtend8to32(ReadByte(cCS,eip+rm.GetLength())));
}

void x86CPU::op_shr_rm8_cl(ModRM &rm){
	rm.WriteByter(ShiftLogicalRight8(rm.ReadByter(),*regs8[CL]));
}
void x86CPU::op_shr_rmW_cl(ModRM &rm){
	rm.WriteW(ShiftLogicalRight(rm.ReadW(),*regs8[CL]));
}

void x86CPU::op_shl_rm8_cl(ModRM &rm){
	rm.WriteByter(ShiftLogicalLeft8(rm.ReadByter(),*regs8[CL]));
}
void x86CPU::op_shl_rmW_cl(ModRM &rm){
	rm.WriteW(ShiftLogicalLeft(rm.ReadW(),*regs8[CL]));
}

void x86CPU::op_sar_rm8_cl(ModRM &rm){
	rm.WriteByter(ShiftArithmeticRight8(rm.ReadByter(),*regs8[CL]));
}
void x86CPU::op_sar_rmW_cl(ModRM &rm){
	rm.WriteW(ShiftArithmeticRightW(rm.ReadW(),*regs8[CL]));
}

void x86CPU::op_rol_rm8_cl(ModRM &rm){
	rm.WriteByter(RotateLeft8(rm.ReadByter(),*regs8[CL]));
}
void x86CPU::op_rol_rmW_cl(ModRM &rm){
	rm.WriteW(RotateLeftW(rm.ReadW(),*regs8[CL]));
}

void x86CPU::op_ror_rm8_cl(ModRM &rm){
	rm.WriteByter(RotateRight8(rm.ReadByter(),*regs8[CL]));
}
void x86CPU::opW_ror_rmW_cl(ModRM &rm){
	rm.WriteW(RotateRightW(rm.ReadW(),*regs8[CL]));
}


void x86CPU::op_rcl_rm8_cl(ModRM &rm){
	rm.WriteByter(RotateCarryLeft8(rm.ReadByter(),*regs8[CL]));
}
void x86CPU::op_rcl_rmW_cl(ModRM &rm){
	rm.WriteW(RotateCarryLeftW(rm.ReadW(),*regs8[CL]));
}

void x86CPU::op_rcr_rm8_cl(ModRM &rm){
	rm.WriteByter(RotateCarryRight8(rm.ReadByter(),*regs8[CL]));
}
void x86CPU::op_rcr_rmW_cl(ModRM &rm){
	rm.WriteW(RotateCarryRightW(rm.ReadW(),*regs8[CL]));
}


/****/
void x86CPU::op_shr_rm8_1(ModRM &rm){
	rm.WriteByter(ShiftLogicalRight8(rm.ReadByter(),1));
}
void x86CPU::op_shr_rmW_1(ModRM &rm){
	rm.WriteW(ShiftLogicalRight(rm.ReadW(),1));
}

void x86CPU::op_shl_rm8_1(ModRM &rm){
	rm.WriteByter(ShiftLogicalLeft8(rm.ReadByter(),1));
}
void x86CPU::op_shl_rmW_1(ModRM &rm){
	rm.WriteW(ShiftLogicalLeftW(rm.ReadW(),1));
}

void x86CPU::op_sar_rm8_1(ModRM &rm){
	rm.WriteByter(ShiftArithmeticRight8(rm.ReadByter(),1));
}
void x86CPU::op_sar_rmW_1(ModRM &rm){
	rm.WriteW(ShiftArithmeticRightW(rm.ReadW(),1));
}

void x86CPU::op_rol_rm8_1(ModRM &rm){
	rm.WriteByter(RotateLeft8(rm.ReadByter(),1));
}
void x86CPU::op_rol_rmW_1(ModRM &rm){
	rm.WriteW(RotateLeftW(rm.ReadW(),1));
}

void x86CPU::op_ror_rm8_1(ModRM &rm){
	rm.WriteByter(RotateRight8(rm.ReadByter(),1));
}
void x86CPU::op_ror_rmW_1(ModRM &rm){
	rm.WriteW(RotateRightW(rm.ReadW(),1));
}

void x86CPU::op_rcl_rm8_1(ModRM &rm){
	rm.WriteByter(RotateCarryLeft8(rm.ReadByter(),1));
}
void x86CPU::op_rcl_rmW_1(ModRM &rm){
	rm.WriteW(RotateCarryLeftW(rm.ReadW(),1));
}

void x86CPU::op_rcr_rm8_1(ModRM &rm){
	rm.WriteByter(RotateCarryRight8(rm.ReadByter(),1));
}
void x86CPU::op_rcr_rmW_1(ModRM &rm){
	rm.WriteW(RotateCarryRightW(rm.ReadW(),1));
}

void x86CPU::op_not_rm8(ModRM &rm){
	rm.WriteByter(~(rm.ReadByter()));
}

void x86CPU::op_not_rmW(ModRM &rm){
	rm.WriteW(~(rm.ReadW()));
}



/**
BCD Opcodes for Open86
By: Alboin, 3-16-07
Modified to work with x86Lib by Jordan(hckr83)
**/


/* Opcode: 0x37 */
void x86CPU::op_aaa() {
	if((*regs8[AL] & 0x0f) > 9 || freg.af == 1) {
		*regs8[AL] += 6;
		*regs8[AH]++;
		freg.af = 1;
		freg.cf = 1;
	}
	else {
		freg.af = 0;
		freg.cf = 0;
	}
	*regs8[AL] = *regs8[AL] & 0x0f;
}


/* Opcode: 0x27 */
void x86CPU::op_daa() {
	if((*regs8[AL] & 0x0f) > 9 || freg.af == 1) {
		*regs8[AL] += 6;
		*regs8[AH]++;
	}
	else
		freg.af = 0;

	if(*regs8[AL] > 0x9f || freg.cf == 1) {
			*regs8[AL] = *regs8[AL] + 0x60;
			freg.cf = 1;
	}
	else
		freg.cf = 0;
}

/* Opcode: 0x2F */
void x86CPU::op_das() {
	if((*regs8[AL] & 0x0f) > 9 || freg.af == 1) {
		*regs8[AL] -= 6;
		freg.af = 1;
	}
	else
		freg.af = 0;

	if(*regs8[AL] > 0x9f || freg.cf == 1) {
			*regs8[AL] -= 0x60;
			freg.cf = 1;
	}
	else
		freg.cf = 0;
}

/* Opcode: 0x3F */
void x86CPU::op_aas() {
	if((*regs8[AL] & 0x0f) > 9 || freg.af == 1) {
		*regs8[AL] -= 6;
		*regs8[AH]--;
		freg.af = 1;
		freg.cf = 1;
	}
	else {
		freg.af = 0;
		freg.cf = 0;
	}
	*regs8[AL] = *regs8[AL] & 0x0f;
}



void x86CPU::op_aad_imm8(){
	//AAD is a weird opcode because it is two bytes
	//for "apparently" no reason. But really, it is just
	//undocumented... the 0x0A in the second byte is for
	//multiplecation...but it can be changed...
	*regs8[AL]=(*regs8[AH])*(op_cache[1])+*regs8[AL];
	*regs8[AH]=0;
}

void x86CPU::op_aam_imm8(){
	//same wiht the 0x0A operand as above..
	if(op_cache[1]==0){
		throw CpuInt_excp(DIV0_IEXCP);
	}
	*regs8[AH]=(*regs8[AL])/op_cache[1];
	*regs8[AL]=(*regs8[AL])%op_cache[1];
}










};



