/**
Copyright (c) 2018 - 2018 Yauhen Klimiankou  <Evgeny.Klimenkov@gmail.com>
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
**/

// ----------------------------------------------------------- IA-32  Disassembler -----------------------------------------------------------
#include "Disassembler.h"

static const uint32_t ONE       = 0x00000001U;
static const  int32_t TEN       = 0x0000000AU;
static const uint32_t TYPE_MASK = 0xFF000000U;

static const char* const COMMAND_NAMES[] =
{
	"aaa"     ,
    "aad"     ,
    "aam"     ,
    "aas"     ,
    "adc"     ,
    "add"     ,
    "and"     ,
    "arpl"    ,
    "bound"   ,
    "bswap"   ,
    "call"    ,
    "cdq"     ,
    "clc"     ,
    "cld"     ,
    "cli"     ,
    "clts"    ,
    "cmc"     ,
    "cmp"     ,
    "cmpsb"   ,
    "cmpsd"   ,
    "cpuid"   ,
    "cwde"    ,
    "daa"     ,
    "das"     ,
    "dec"     ,
    "div"     ,
    "emms"    ,
    "enter"   ,
    "f2xm1"   ,
    "fabs"    ,
    "fadd"    ,
    "faddp"   ,
    "fbld"    ,
    "fbstp"   ,
    "fchs"    ,
    "fcmovb"  ,
    "fcmovbe" ,
    "fcmove"  ,
    "fcmovnb" ,
    "fcmovnbe",
    "fcmovne" ,
    "fcmovnu" ,
    "fcmovu"  ,
    "fcom"    ,
    "fcomi"   ,
    "fcomip"  ,
    "fcomp"   ,
    "fcomp5"  ,
    "fcompp"  ,
    "fcos"    ,
    "fdecstp" ,
    "fdiv"    ,
    "fdivp"   ,
    "fdivr"   ,
    "fdivrp"  ,
    "femms"   ,
    "ffree"   ,
    "ffreep"  ,
    "fiadd"   ,
    "ficom"   ,
    "ficomp"  ,
    "fidiv"   ,
    "fidivr"  ,
    "fild"    ,
    "fimul"   ,
    "fincstp" ,
    "fist"    ,
    "fistp"   ,
    "fisttp"  ,
    "fisub"   ,
    "fisubr"  ,
    "fld"     ,
    "fld1"    ,
    "fldcw"   ,
    "fldenv"  ,
    "fldl2e"  ,
    "fldl2t"  ,
    "fldlg2"  ,
    "fldln2"  ,
    "fldpi"   ,
    "fldz"    ,
    "fmul"    ,
    "fmulp"   ,
    "fnclex"  ,
    "fndisi"  ,
    "fneni"   ,
    "fninit"  ,
    "fnsave"  ,
    "fnstcw"  ,
    "fnstenv" ,
    "fnstsw"  ,
    "fnop"    ,
    "fpatan"  ,
    "fprem"   ,
    "fprem1"  ,
    "fptan"   ,
    "frndint" ,
    "frstor"  ,
    "fscale"  ,
    "fsetpm"  ,
    "fsin"    ,
    "fsincos" ,
    "fsqrt"   ,
    "fst"     ,
    "fstp"    ,
    "fstp1"   ,
    "fstp8"   ,
    "fstp9"   ,
    "fsub"    ,
    "fsubr"   ,
    "fsubrp"  ,
    "fsubp"   ,
    "ftst"    ,
    "fucom"   ,
    "fucomi"  ,
    "fucomip" ,
    "fucomp"  ,
    "fucompp" ,
    "fxam"    ,
    "fxch"    ,
    "fxch4"   ,
    "fxch7"   ,
    "fxtract" ,
    "fyl2x"   ,
    "fyl2xp1" ,
    "getsec"  ,
    "hlt"     ,
    "icebp"   ,
    "idiv"    ,
    "imul"    ,
    "in"      ,
    "inc"     ,
    "insb"    ,
    "insd"    ,
    "int"     ,
    "into"    ,
    "iret"    ,
    "invd"    ,
    "ja"      ,
    "jb"      ,
    "jecxz"   ,
    "jbe"     ,
    "jg"      ,
    "jge"     ,
    "jl"      ,
    "jmp"     ,
    "jle"     ,
    "jnb"     ,
    "jno"     ,
    "jnp"     ,
    "jns"     ,
    "jnz"     ,
    "jo"      ,
    "jp"      ,
    "js"      ,
    "jz"      ,
    "lahf"    ,
    "lds"     ,
    "lea"     ,
    "leave"   ,
    "les"     ,
    "lock"    ,
    "lodsb"   ,
    "lodsd"   ,
    "loop"    ,
    "loope"   ,
    "loopne"  ,
    "mov"     ,
    "movsb"   ,
    "movsd"   ,
    "mul"     ,
    "neg"     ,
    "nop"     ,
    "not"     ,
    "or"      ,
    "out"     ,
    "outsb"   ,
    "outsd"   ,
    "pause"   ,
    "pop"     ,
    "popa"    ,
    "popf"    ,
    "push"    ,
    "pusha"   ,
    "pushf"   ,
    "rcl"     ,
    "rcr"     ,
    "rdmsr"   ,
    "rdpmc"   ,
    "rdtsc"   ,
    "repne"   ,
    "retf"    ,
    "retn"    ,
    "rms"     ,
    "rol"     ,
    "ror"     ,
    "sahf"    ,
    "sal"     ,
    "sar"     ,
    "sbb"     ,
    "scasb"   ,
    "scasd"   ,
    "setalc"  ,
    "shl"     ,
    "shr"     ,
    "stc"     ,
    "std"     ,
    "sti"     ,
    "stosb"   ,
    "stosd"   ,
    "sub"     ,
    "syscall" ,
    "sysenter",
    "sysexit" ,
    "sysret"  ,
    "test"    ,
    "ud2"     ,
    "wait"    ,
    "wbinvd"  ,
    "wrmsr"   ,
    "xabort"  ,
    "xbegin"  ,
    "xchg"    ,
    "xlat"    ,
	"xor"     ,

    "prefix db 0x26",
    "prefix db 0x2E",
    "prefix db 0x36",
    "prefix db 0x3E",
    "prefix db 0x64",
    "prefix db 0x65",
    "prefix db 0x66",
    "prefix db 0x67",
    "prefix db 0xF0",
    "prefix db 0xF2",
	"prefix db 0xF3",
};

static const char* const INDEX_SCALE  [] = { "", "*2", "*4", "*8" };

static const char* const OPERAND_NAMES[] =
{
            "al" ,            "cl" ,            "dl" ,            "bl" ,            "ah" ,            "ch" ,            "dh" ,            "bh" ,
            "ax" ,            "cx" ,            "dx" ,            "bx" ,            "sp" ,            "bp" ,            "si" ,            "di" ,
           "eax" ,           "ecx" ,           "edx" ,           "ebx" ,           "esp" ,           "ebp" ,           "esi" ,           "edi" ,
          "[eax]",          "[ecx]",          "[edx]",          "[ebx]",          "[esp]",          "[ebp]",          "[esi]",          "[edi]",
 "byte ptr [eax]", "byte ptr [ecx]", "byte ptr [edx]", "byte ptr [ebx]", "byte ptr [esp]", "byte ptr [ebp]", "byte ptr [esi]", "byte ptr [edi]",
 "word ptr [eax]", "word ptr [ecx]", "word ptr [edx]", "word ptr [ebx]", "word ptr [esp]", "word ptr [ebp]", "word ptr [esi]", "word ptr [edi]",
"dword ptr [eax]","dword ptr [ecx]","dword ptr [edx]","dword ptr [ebx]","dword ptr [esp]","dword ptr [ebp]","dword ptr [esi]","dword ptr [edi]",
"fword ptr [eax]","fword ptr [ecx]","fword ptr [edx]","fword ptr [ebx]","fword ptr [esp]","fword ptr [ebp]","fword ptr [esi]","fword ptr [edi]",
"qword ptr [eax]","qword ptr [ecx]","qword ptr [edx]","qword ptr [ebx]","qword ptr [esp]","qword ptr [ebp]","qword ptr [esi]","qword ptr [edi]",
"tbyte ptr [eax]","tbyte ptr [ecx]","tbyte ptr [edx]","tbyte ptr [ebx]","tbyte ptr [esp]","tbyte ptr [ebp]","tbyte ptr [esi]","tbyte ptr [edi]",
            "st" ,        "st (1)" ,        "st (2)" ,        "st (3)" ,        "st (4)" ,        "st (5)" ,        "st (6)" ,        "st (7)" ,
            "es" ,            "cs" ,            "ss" ,            "ds" ,            "fs" ,            "gs" ,
};
// -------------------------------- HELPERS ---------------------------------------------------------------------------------------------------
inline uint8_t  LoUint8 (const uint16_t VALUE) { return static_cast<uint8_t >(VALUE &   UINT8_MAX     ); }
inline uint8_t  HiUint8 (const uint16_t VALUE) { return static_cast<uint8_t >(VALUE / ( UINT8_MAX + 1)); }
inline uint16_t LoUint16(const uint32_t VALUE) { return static_cast<uint16_t>(VALUE &  UINT16_MAX     ); }
inline uint16_t HiUint16(const uint32_t VALUE) { return static_cast<uint16_t>(VALUE / (UINT16_MAX + 1)); }

static const char* ExtractPrefix(const uint8_t BASE)
{
    switch (BASE / 8)
    {
        case  4: return  "byte ptr ";
        case  5: return  "word ptr ";
        case  6: return "dword ptr ";
        case  7: return "fword ptr ";
        case  8: return "qword ptr ";
        case  9: return "tbyte ptr ";
        default: return           "";
    }
}

// -------------------------------- ARGUMENT CONSTRUCTORS -------------------------------------------------------------------------------------
inline char* AddChr(char* ptr, const char CHR)
{
	*ptr = CHR;

	return ptr + ONE;
}

inline char* AddDig(char* ptr, const char DIG)
{
	*ptr = DIG + ((DIG < TEN) ? ('0') : ('A' - TEN));

	return ptr + ONE;
}

inline char* AddStr(char* ptr, const char* str)
{
	for(; *str != '\0'; ptr++, str++)
		*ptr = *str;

	return ptr;
}

static char* AddInt(char* ptr, uint32_t value)
{
	if (value < TEN)
		ptr = AddChr(ptr, '0' + static_cast<char>(value));
	else
	{
		int nibbles = CHAR_BIT;

		for (; (value & 0xF0000000) == 0; value <<= 4)
			nibbles--;

		if ((value & 0xF0000000) >= 0xA0000000)
			ptr = AddChr(ptr, '0');

		for (char x = static_cast<char>(value >> 28); nibbles != 0; nibbles--, value <<= 4, x = static_cast<char>(value >> 28))
			ptr = AddDig(ptr, x);

		ptr = AddChr(ptr, 'h');
	}

	return ptr;
}

static char* Addint(char* ptr, uint32_t value)
{
	if (value < TEN)
		ptr = AddChr(ptr, '0' + static_cast<char>(value));
	else
	{
		int nibbles = CHAR_BIT;

		for (; (value & 0xF0000000) == 0; value <<= 4)
			nibbles--;

		for (int x = static_cast<char>(value >> 28); nibbles != 0; nibbles--, value <<= 4, x = static_cast<char>(value >> 28))
			ptr = AddDig(ptr, x);
	}

	return ptr;
}

static char* AddOffset(char* ptr, uint32_t number)
{
    if (number >= INT_MAX)
    {
		ptr    = AddChr(ptr, '-');
        number = UINT32_MAX - number + ONE;
    }
	else
	{
		ptr = AddChr(ptr, '+');
	}

    return AddInt(ptr, number);
}

static char* ArgDirectAddress(char* ptr, const uint32_t ARGUMENT, const uint32_t ADDRESS)
{
	ptr = AddStr(ptr, ExtractPrefix(LoUint8(LoUint16(ARGUMENT))));
	ptr = AddStr(ptr,                                "ds:"      );
	ptr = AddInt(ptr,                                ADDRESS    );

	return ptr;
}

static char* ArgDirectIndex(char* ptr, const uint32_t ARGUMENT, const uint32_t ADDRESS)
{
    const uint8_t BASE = LoUint8(LoUint16(ARGUMENT));

	ptr = AddStr(ptr,                  ExtractPrefix(BASE    )  );
	ptr = AddStr(ptr,                                "ds:"      );
	ptr = AddInt(ptr,                                ADDRESS    );
	ptr = AddChr(ptr,                                '['        );
	ptr = AddStr(ptr, OPERAND_NAMES[      REG_EAX + (BASE % 8) ]);
	ptr = AddStr(ptr,   INDEX_SCALE[LoUint8(HiUint16(ARGUMENT))]);
	ptr = AddChr(ptr,                                ']'        );

    return ptr;
}

static char* ArgSignedImm(char* ptr, uint32_t address)
{
	if (address > INT32_MAX)
	{
		address = ~address  +  ONE;
		ptr     = AddChr(ptr, '-');
	}

	return AddInt(ptr, address);
}

static char* ArgWithAddress(char* ptr, const uint32_t ARGUMENT, const uint32_t ADDRESS)
{
    const uint8_t BASE = LoUint8(LoUint16(ARGUMENT));

    ptr = AddStr   (ptr,                  ExtractPrefix(BASE    )  );
	ptr = AddChr   (ptr,                                '['        );
	ptr = AddStr   (ptr, OPERAND_NAMES[      REG_EAX + (BASE % 8) ]);
	ptr = AddChr   (ptr,                                '+'        );
    ptr = AddStr   (ptr, OPERAND_NAMES[HiUint8(LoUint16(ARGUMENT))]);
    ptr = AddStr   (ptr,   INDEX_SCALE[LoUint8(HiUint16(ARGUMENT))]);
    ptr = AddOffset(ptr,                                ADDRESS    );
	ptr = AddChr   (ptr,                                ']'        );

    return ptr;
}

static char* ArgWithIndex(char* ptr, const uint32_t ARGUMENT)
{
    const uint8_t BASE = LoUint8(LoUint16(ARGUMENT));

    ptr = AddStr(ptr,                  ExtractPrefix(BASE    )  );
	ptr = AddChr(ptr,                                '['        );
	ptr = AddStr(ptr, OPERAND_NAMES[      REG_EAX + (BASE % 8) ]);
	ptr = AddChr(ptr,                                '+'        );
    ptr = AddStr(ptr, OPERAND_NAMES[HiUint8(LoUint16(ARGUMENT))]);
    ptr = AddStr(ptr,   INDEX_SCALE[LoUint8(HiUint16(ARGUMENT))]);
	ptr = AddChr(ptr,                                ']'        );

    return ptr;
}

static char* ArgWithOffset(char* ptr, const uint32_t ARGUMENT, const uint32_t ADDRESS)
{
    const uint8_t BASE = LoUint8(LoUint16(ARGUMENT));

    ptr = AddStr   (ptr,                  ExtractPrefix(BASE    )  );
	ptr = AddChr   (ptr,                                '['        );
    ptr = AddStr   (ptr, OPERAND_NAMES[REG_EAX +       (BASE % 8) ]);
    ptr = AddStr   (ptr,   INDEX_SCALE[LoUint8(HiUint16(ARGUMENT))]);
    ptr = AddOffset(ptr,                                ADDRESS    );
    ptr = AddChr   (ptr,                                ']'        );

    return ptr;
}

static char* BuildArg(char* ptr, const uint32_t ARGUMENT, const uint32_t ADDRESS)
{
    switch (HiUint8(HiUint16(ARGUMENT)))
    {
        case NULL                       : return AddStr          (ptr, OPERAND_NAMES[LoUint8(LoUint16(ARGUMENT))]         );
        case WITH_____INDEX             : return ArgWithIndex    (ptr,                                ARGUMENT            );
        case DIRECT_ADDRESS / 0x01000000: return ArgDirectAddress(ptr,                                ARGUMENT   , ADDRESS);
        case DIRECT___INDEX             : return ArgDirectIndex  (ptr,                                ARGUMENT   , ADDRESS);
        case WITH___ADDRESS             : return ArgWithAddress  (ptr,                                ARGUMENT   , ADDRESS);
        case WITH____OFFSET             : return ArgWithOffset   (ptr,                                ARGUMENT   , ADDRESS);
        case IMM_____SIGNED / 0x01000000: return ArgSignedImm    (ptr,                                             ADDRESS);
        default                         : return AddInt          (ptr,                                             ADDRESS);
    }
}

static char* MakeLabel(char* ptr, const uint32_t ADDRESS, const int32_t OFFSET)
{
    const size_t ACTUAL_ADDRESS = ADDRESS + OFFSET;

    if ((ACTUAL_ADDRESS & ONE) != 0)
		return AddStr(Addint(AddStr(ptr, "near ptr loc_"), ACTUAL_ADDRESS & ~ONE), "+1");
	else
		return        Addint(AddStr(ptr,          "loc_"), ACTUAL_ADDRESS       )       ;
}

char* Disassembler::GetUndefined(char* ptr, const uint8_t BYTE)
{
	const char HIGH = BYTE / 16;
	const char LOW  = BYTE % 16;

                                                 ptr = AddChr(ptr, 'd' );
                                                 ptr = AddChr(ptr, 'b' );
                                                 ptr = AddChr(ptr, ' ' );
	if (((HIGH == 0) ?  (LOW) : (HIGH)) >= TEN ) ptr = AddChr(ptr, '0' );
	if (  HIGH != 0                            ) ptr = AddDig(ptr, HIGH);
	                                             ptr = AddDig(ptr, LOW );
	if ( (HIGH != 0) || (LOW            >= TEN)) ptr = AddChr(ptr, 'h' );

	return ptr;
}

char* Disassembler::GetInstruction(char* code, const uint32_t OFFSET) const
{
	const uint32_t ARG_1 = decoder.GetArg1();
	const uint32_t CMD   = decoder.GetCmd ();

	char* ptr = AddStr(code, COMMAND_NAMES[CMD]);

    if (ARG_1 != UINT32_MAX)
    {
        static const uint32_t UINT8MAX = 0x000000FF;

		const uint32_t ARG_2 = decoder.GetArg2();
		const uint32_t VAL_1 = decoder.GetVal1();

		*ptr = ' '; ptr++;

		if (ARG_2 == UINT32_MAX)
        {
			if      ((CMD >= CMD_JA  ) && ( CMD                <= CMD_JZ        ))
                if  ((CMD == CMD_JMP ) && ((ARG_1 & TYPE_MASK) != IMM___UNSIGNED))
                        ptr = BuildArg(        ptr              , ARG_1  , VAL_1);
                else
                    if (((VAL_1 & (~UINT8MAX)) == (~UINT8MAX)) || ((VAL_1 & (~UINT8MAX)) == 0))
                        ptr = MakeLabel(AddStr(ptr, "short "   ), OFFSET , VAL_1);
                    else
                        ptr = AddInt(AddStr(   ptr, "near ptr "), OFFSET + VAL_1);
            else if ((CMD >= CMD_LOOP) && ( CMD                <= CMD_LOOPNE    ))
                        ptr = MakeLabel(       ptr              , OFFSET , VAL_1);
            else if ( CMD == CMD_CALL                                            )
                if ((ARG_1 & TYPE_MASK) == IMM___UNSIGNED)
                        ptr = AddInt(AddStr(   ptr, "near ptr "), OFFSET + VAL_1);
                else
                        ptr = BuildArg(        ptr              , ARG_1  , VAL_1);
            else if ( CMD == CMD_XBEGIN                                          )
                        ptr = AddInt(AddStr(   ptr, "near ptr "), OFFSET + VAL_1);
            else
                        ptr = BuildArg(        ptr              , ARG_1  , VAL_1);
        }
        else
            ptr = Get23(ptr, CMD, ARG_1, ARG_2, VAL_1);
    }

	return ptr;
}

char* Disassembler::Get23(char* ptr, const uint32_t CMD, const uint32_t ARG_1, const uint32_t ARG_2, const uint32_t VAL_1) const
{
    const uint32_t ARG_3 = decoder.GetArg3();
    const uint32_t VAL_2 = decoder.GetVal2();

    if (ARG_3 == UINT32_MAX)
    {
        if ((CMD == CMD_CALL) || (CMD == CMD_JMP))
		{
			ptr = AddStr(ptr, "far ptr ");
			ptr = AddInt(ptr, VAL_2     );
			ptr = AddChr(ptr, ':'       );
			ptr = AddInt(ptr, VAL_1     );
		}
        else
        {
			ptr = BuildArg(ptr, ARG_1, VAL_1);
            ptr =   AddStr(ptr, ", "        );
			ptr = BuildArg(ptr, ARG_2, VAL_2);
        }
    }
    else
    {
        ptr = BuildArg(ptr, ARG_3,     0);
        ptr =   AddStr(ptr, ", "        );
        ptr = BuildArg(ptr, ARG_1, VAL_1);
        ptr =   AddStr(ptr, ", "        );
        ptr = BuildArg(ptr, ARG_2, VAL_2);
    }

	return ptr;
}
