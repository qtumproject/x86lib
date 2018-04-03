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

// ------------------------------------------------ Efficient and fast IA-32 software decoder ------------------------------------------------
#include "Decoder.h"

#define ONE 1

static const int8_t SIZE_8S = -static_cast< int8_t >(sizeof( int8_t ));
static const int8_t SIZE_8U = +static_cast<uint8_t >(sizeof(uint8_t ));
static const int8_t SIZE_16 = +static_cast<uint16_t>(sizeof(uint16_t));
static const int8_t SIZE_32 = +static_cast<uint32_t>(sizeof(uint32_t));

static const INSTRUCTION COMMAND_TABLE[] =
{
    CMD_FADD   , CMD_FMUL   , CMD_FCOM    , CMD_FCOMP  , CMD_FSUB  , CMD_FSUBR  , CMD_FDIV  , CMD_FDIVR, // 0
    CMD_ADD    , CMD_OR     , CMD_ADC     , CMD_SBB    , CMD_AND   , CMD_SUB    , CMD_XOR   , CMD_CMP  , // 1
    CMD_ROL    , CMD_ROR    , CMD_RCL     , CMD_RCR    , CMD_SHL   , CMD_SHR    , CMD_SAL   , CMD_SAR  , // 2
    CMD_POP    , CMD_NULL   , CMD_NULL    , CMD_NULL   , CMD_NULL  , CMD_NULL   , CMD_NULL  , CMD_NULL , // 3
    CMD_MOV    , CMD_NULL   , CMD_NULL    , CMD_NULL   , CMD_NULL  , CMD_NULL   , CMD_NULL  , CMD_NULL , // 4
    CMD_MOV    , CMD_NULL   , CMD_NULL    , CMD_NULL   , CMD_NULL  , CMD_NULL   , CMD_NULL  , CMD_NULL , // 5
    CMD_TEST   , CMD_TEST   , CMD_NOT     , CMD_NEG    , CMD_MUL   , CMD_IMUL   , CMD_DIV   , CMD_IDIV , // 6
};

static const INSTRUCTION CMD_D9E[] = { CMD_FLD    , CMD_FXCH   , CMD_NULL    , CMD_FSTP1  , CMD_NULL  , CMD_NULL   , CMD_NULL  , CMD_NULL  };
static const INSTRUCTION CMD_DAE[] = { CMD_FCMOVB , CMD_FCMOVE , CMD_FCMOVBE , CMD_FCMOVU , CMD_NULL  , CMD_NULL   , CMD_NULL  , CMD_NULL  };
static const INSTRUCTION CMD_DBE[] = { CMD_FCMOVNB, CMD_FCMOVNE, CMD_FCMOVNBE, CMD_FCMOVNU, CMD_NULL  , CMD_FUCOMI , CMD_FCOMI , CMD_NULL  };
static const INSTRUCTION CMD_DDE[] = { CMD_FFREE  , CMD_FXCH4  , CMD_FST     , CMD_FSTP   , CMD_FUCOM , CMD_FUCOMP , CMD_NULL  , CMD_NULL  };
static const INSTRUCTION CMD_DEE[] = { CMD_FADDP  , CMD_FMULP  , CMD_FCOMP5  , CMD_NULL   , CMD_FSUBRP, CMD_FSUBP  , CMD_FDIVRP, CMD_FDIVP };
static const INSTRUCTION CMD_DFE[] = { CMD_FFREEP , CMD_FXCH7  , CMD_FSTP8   , CMD_FSTP9  , CMD_NULL  , CMD_FUCOMIP, CMD_FCOMIP, CMD_NULL  };
static const INSTRUCTION CMD_FFE[] = { CMD_INC    , CMD_DEC    , CMD_CALL    , CMD_NULL   , CMD_JMP   , CMD_NULL   , CMD_PUSH  , CMD_NULL  };

static const CpuInstruction OPCODE_TABLE_L0[UINT8_MAX + ONE] =
{
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 00
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 01
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 02
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 03
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 04
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 05
    CpuInstruction(CMD_PUSH  , REG_SES       , UINT32_MAX, UINT32_MAX), // 06
    CpuInstruction(CMD_POP   , REG_SES       , UINT32_MAX, UINT32_MAX), // 07
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 08
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 09
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 0A
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 0B
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 0C
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 0D
    CpuInstruction(CMD_PUSH  , REG_SCS       , UINT32_MAX, UINT32_MAX), // 0E
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 0F
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 10
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 11
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 12
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 13
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 14
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 15
    CpuInstruction(CMD_PUSH  , REG_SSS       , UINT32_MAX, UINT32_MAX), // 16
    CpuInstruction(CMD_POP   , REG_SSS       , UINT32_MAX, UINT32_MAX), // 17
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 18
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 19
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 1A
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 1B
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 1C
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 1D
    CpuInstruction(CMD_PUSH  , REG_SDS       , UINT32_MAX, UINT32_MAX), // 1E
    CpuInstruction(CMD_POP   , REG_SDS       , UINT32_MAX, UINT32_MAX), // 1F
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 20
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 21
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 22
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 23
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 24
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 25
    CpuInstruction(PREFIX_26 , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 26
    CpuInstruction(CMD_DAA   , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 27
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 28
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 29
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 2A
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 2B
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 2C
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 2D
    CpuInstruction(PREFIX_2E , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 2E
    CpuInstruction(CMD_DAS   , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 2F
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 30
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 31
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 32
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 33
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 34
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 35
    CpuInstruction(PREFIX_36 , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 36
    CpuInstruction(CMD_AAA   , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 37
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 38
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 39
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 3A
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 3B
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 3C
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 3D
    CpuInstruction(PREFIX_3E , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 3E
    CpuInstruction(CMD_AAS   , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 3F
    CpuInstruction(CMD_INC   , REG_EAX       , UINT32_MAX, UINT32_MAX), // 40
    CpuInstruction(CMD_INC   , REG_ECX       , UINT32_MAX, UINT32_MAX), // 41
    CpuInstruction(CMD_INC   , REG_EDX       , UINT32_MAX, UINT32_MAX), // 42
    CpuInstruction(CMD_INC   , REG_EBX       , UINT32_MAX, UINT32_MAX), // 43
    CpuInstruction(CMD_INC   , REG_ESP       , UINT32_MAX, UINT32_MAX), // 44
    CpuInstruction(CMD_INC   , REG_EBP       , UINT32_MAX, UINT32_MAX), // 45
    CpuInstruction(CMD_INC   , REG_ESI       , UINT32_MAX, UINT32_MAX), // 46
    CpuInstruction(CMD_INC   , REG_EDI       , UINT32_MAX, UINT32_MAX), // 47
    CpuInstruction(CMD_DEC   , REG_EAX       , UINT32_MAX, UINT32_MAX), // 48
    CpuInstruction(CMD_DEC   , REG_ECX       , UINT32_MAX, UINT32_MAX), // 49
    CpuInstruction(CMD_DEC   , REG_EDX       , UINT32_MAX, UINT32_MAX), // 4A
    CpuInstruction(CMD_DEC   , REG_EBX       , UINT32_MAX, UINT32_MAX), // 4B
    CpuInstruction(CMD_DEC   , REG_ESP       , UINT32_MAX, UINT32_MAX), // 4C
    CpuInstruction(CMD_DEC   , REG_EBP       , UINT32_MAX, UINT32_MAX), // 4D
    CpuInstruction(CMD_DEC   , REG_ESI       , UINT32_MAX, UINT32_MAX), // 4E
    CpuInstruction(CMD_DEC   , REG_EDI       , UINT32_MAX, UINT32_MAX), // 4F
    CpuInstruction(CMD_PUSH  , REG_EAX       , UINT32_MAX, UINT32_MAX), // 50
    CpuInstruction(CMD_PUSH  , REG_ECX       , UINT32_MAX, UINT32_MAX), // 51
    CpuInstruction(CMD_PUSH  , REG_EDX       , UINT32_MAX, UINT32_MAX), // 52
    CpuInstruction(CMD_PUSH  , REG_EBX       , UINT32_MAX, UINT32_MAX), // 53
    CpuInstruction(CMD_PUSH  , REG_ESP       , UINT32_MAX, UINT32_MAX), // 54
    CpuInstruction(CMD_PUSH  , REG_EBP       , UINT32_MAX, UINT32_MAX), // 55
    CpuInstruction(CMD_PUSH  , REG_ESI       , UINT32_MAX, UINT32_MAX), // 56
    CpuInstruction(CMD_PUSH  , REG_EDI       , UINT32_MAX, UINT32_MAX), // 57
    CpuInstruction(CMD_POP   , REG_EAX       , UINT32_MAX, UINT32_MAX), // 58
    CpuInstruction(CMD_POP   , REG_ECX       , UINT32_MAX, UINT32_MAX), // 59
    CpuInstruction(CMD_POP   , REG_EDX       , UINT32_MAX, UINT32_MAX), // 5A
    CpuInstruction(CMD_POP   , REG_EBX       , UINT32_MAX, UINT32_MAX), // 5B
    CpuInstruction(CMD_POP   , REG_ESP       , UINT32_MAX, UINT32_MAX), // 5C
    CpuInstruction(CMD_POP   , REG_EBP       , UINT32_MAX, UINT32_MAX), // 5D
    CpuInstruction(CMD_POP   , REG_ESI       , UINT32_MAX, UINT32_MAX), // 5E
    CpuInstruction(CMD_POP   , REG_EDI       , UINT32_MAX, UINT32_MAX), // 5F
    CpuInstruction(CMD_PUSHA , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 60
    CpuInstruction(CMD_POPA  , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 61
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 62
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 63
    CpuInstruction(PREFIX_64 , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 64
    CpuInstruction(PREFIX_65 , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 65
    CpuInstruction(PREFIX_66 , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 66
    CpuInstruction(PREFIX_67 , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 67
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 68
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 69
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 6A
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 6B
    CpuInstruction(CMD_INSB  , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 6C
    CpuInstruction(CMD_INSD  , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 6D
    CpuInstruction(CMD_OUTSB , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 6E
    CpuInstruction(CMD_OUTSD , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 6F
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 70
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 71
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 72
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 73
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 74
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 75
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 76
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 77
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 78
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 79
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 7A
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 7B
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 7C
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 7D
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 7E
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 7F
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 80
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 81
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 82
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 83
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 84
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 85
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 86
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 87
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 88
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 89
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 8A
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 8B
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 8C
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 8D
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 8E
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 8F
    CpuInstruction(CMD_NOP   , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 90
    CpuInstruction(CMD_XCHG  , REG_EAX       , REG_ECX   , UINT32_MAX), // 91
    CpuInstruction(CMD_XCHG  , REG_EAX       , REG_EDX   , UINT32_MAX), // 92
    CpuInstruction(CMD_XCHG  , REG_EAX       , REG_EBX   , UINT32_MAX), // 93
    CpuInstruction(CMD_XCHG  , REG_EAX       , REG_ESP   , UINT32_MAX), // 94
    CpuInstruction(CMD_XCHG  , REG_EAX       , REG_EBP   , UINT32_MAX), // 95
    CpuInstruction(CMD_XCHG  , REG_EAX       , REG_ESI   , UINT32_MAX), // 96
    CpuInstruction(CMD_XCHG  , REG_EAX       , REG_EDI   , UINT32_MAX), // 97
    CpuInstruction(CMD_CWDE  , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 98
    CpuInstruction(CMD_CDQ   , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 99
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 9A
    CpuInstruction(CMD_WAIT  , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 9B
    CpuInstruction(CMD_PUSHF , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 9C
    CpuInstruction(CMD_POPF  , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 9D
    CpuInstruction(CMD_SAHF  , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 9E
    CpuInstruction(CMD_LAHF  , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // 9F
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // A0
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // A1
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // A2
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // A3
    CpuInstruction(CMD_MOVSB , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // A4
    CpuInstruction(CMD_MOVSD , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // A5
    CpuInstruction(CMD_CMPSB , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // A6
    CpuInstruction(CMD_CMPSD , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // A7
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // A8
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // A9
    CpuInstruction(CMD_STOSB , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // AA
    CpuInstruction(CMD_STOSD , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // AB
    CpuInstruction(CMD_LODSB , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // AC
    CpuInstruction(CMD_LODSD , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // AD
    CpuInstruction(CMD_SCASB , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // AE
    CpuInstruction(CMD_SCASD , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // AF
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // B0
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // B1
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // B2
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // B3
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // B4
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // B5
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // B6
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // B7
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // B8
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // B9
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // BA
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // BB
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // BC
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // BD
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // BE
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // BF
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // C0
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // C1
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // C2
    CpuInstruction(CMD_RETN  , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // C3
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // C4
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // C5
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // C6
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // C7
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // C8
    CpuInstruction(CMD_LEAVE , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // C9
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // CA
    CpuInstruction(CMD_RETF  , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // CB
    CpuInstruction(CMD_INT   , IMM___UNSIGNED, UINT32_MAX, 0x03      ), // CC
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // CD
    CpuInstruction(CMD_INTO  , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // CE
    CpuInstruction(CMD_IRET  , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // CF
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // D0
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // D1
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // D2
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // D3
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // D4
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // D5
    CpuInstruction(CMD_SETALC, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // D6
    CpuInstruction(CMD_XLAT  , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // D7
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // D8
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // D9
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // DA
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // DB
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // DC
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // DD
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // DE
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // DF
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // E0
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // E1
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // E2
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // E3
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // E4
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // E5
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // E6
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // E7
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // E8
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // E9
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // EA
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // EB
    CpuInstruction(CMD_IN    , REG__AL       , REG__DX   , UINT32_MAX), // EC
    CpuInstruction(CMD_IN    , REG_EAX       , REG__DX   , UINT32_MAX), // ED
    CpuInstruction(CMD_OUT   , REG__DX       , REG__AL   , UINT32_MAX), // EE
    CpuInstruction(CMD_OUT   , REG__DX       , REG_EAX   , UINT32_MAX), // EF
    CpuInstruction(PREFIX_F0 , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // F0
    CpuInstruction(CMD_ICEBP , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // F1
    CpuInstruction(PREFIX_F2 , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // F2
    CpuInstruction(PREFIX_F3 , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // F3
    CpuInstruction(CMD_HLT   , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // F4
    CpuInstruction(CMD_CMC   , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // F5
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // F6
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // F7
    CpuInstruction(CMD_CLC   , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // F8
    CpuInstruction(CMD_STC   , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // F9
    CpuInstruction(CMD_CLI   , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // FA
    CpuInstruction(CMD_STI   , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // FB
    CpuInstruction(CMD_CLD   , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // FC
    CpuInstruction(CMD_STD   , UINT32_MAX    , UINT32_MAX, UINT32_MAX), // FD
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // FE
    CpuInstruction(UINT32_MAX, UINT32_MAX    , UINT32_MAX, UINT32_MAX), // FF
};

inline INSTRUCTION AsCMD(const uint8_t ID) { return static_cast<INSTRUCTION>(ID); }

inline uint8_t Bit012(const uint8_t VALUE) { return  VALUE         % 0x08; }
inline uint8_t Bit345(const uint8_t VALUE) { return (VALUE / 0x08) % 0x08; }
inline uint8_t Bit_67(const uint8_t VALUE) { return  VALUE / 0x40        ; }

template <typename TH, typename TL> inline uint16_t MakeUint16(const TH HIGH, const TL LOW)
{
    return (static_cast<uint16_t>(HIGH) << 0x08) + static_cast< uint8_t>(LOW);
}

template <typename TH, typename TL> inline uint32_t MakeUint32(const TH HIGH, const TL LOW)
{
    return (static_cast<uint32_t>(HIGH) << 0x10) + static_cast<uint16_t>(LOW);
}

static const uint32_t BASE_WITH_OFFSET = MakeUint32(MakeUint16(WITH____OFFSET, NULL), NULL);

int ImmediateValue::Push(const uint8_t OPCODE)
{
    int status;

    value  += OPCODE << (counter * CHAR_BIT);
    counter++                               ;
    status  = counter                       ;

    if (limit == -ONE)
        value  = *reinterpret_cast<int8_t*>(&value);
    else
        if (limit != counter)
            status = STATUS_CONTINUE;

    return status;
}

void ImmediateValue::Setup(const int8_t SIZE)
{
    counter = 0   ;
    limit   = SIZE;
    value   = 0   ;
}

const Decoder::RootDecodeEntry Decoder::DECODE_TABLE[] =
{
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_ADD    , false, REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 00
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_ADD    , false, REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 01
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_ADD    , true , REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 02
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_ADD    , true , REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 03
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_ADD    , false, REG__AL       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // 04
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_ADD    , false, REG_EAX       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // 05
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 06
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 07
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_OR     , false, REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 08
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_OR     , false, REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 09
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_OR     , true , REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 0A
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_OR     , true , REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 0B
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_OR     , false, REG__AL       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // 0C
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_OR     , false, REG_EAX       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // 0D
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 0E
    { &Decoder::UndefinedOp    , NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 0F
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_ADC    , false, REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 10
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_ADC    , false, REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 11
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_ADC    , true , REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 12
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_ADC    , true , REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 13
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_ADC    , false, REG__AL       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // 14
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_ADC    , false, REG_EAX       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // 15
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 16
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 17
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_SBB    , false, REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 18
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_SBB    , false, REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 19
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_SBB    , true , REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 1A
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_SBB    , true , REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 1B
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_SBB    , false, REG__AL       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // 1C
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_SBB    , false, REG_EAX       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // 1D
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 1E
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 1F
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_AND    , false, REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 20
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_AND    , false, REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 21
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_AND    , true , REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 22
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_AND    , true , REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 23
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_AND    , false, REG__AL       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // 24
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_AND    , false, REG_EAX       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // 25
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 26
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 27
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_SUB    , false, REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 28
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_SUB    , false, REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 29
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_SUB    , true , REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 2A
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_SUB    , true , REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 2B
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_SUB    , false, REG__AL       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // 2C
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_SUB    , false, REG_EAX       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // 2D
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 2E
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 2F
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_XOR    , false, REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 30
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_XOR    , false, REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 31
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_XOR    , true , REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 32
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_XOR    , true , REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 33
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_XOR    , false, REG__AL       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // 34
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_XOR    , false, REG_EAX       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // 35
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 36
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 37
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_CMP    , false, REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 38
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_CMP    , false, REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 39
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_CMP    , true , REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 3A
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_CMP    , true , REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 3B
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_CMP    , false, REG__AL       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // 3C
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_CMP    , false, REG_EAX       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // 3D
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 3E
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 3F
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 40
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 41
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 42
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 43
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 44
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 45
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 46
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 47
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 48
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 49
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 4A
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 4B
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 4C
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 4D
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 4E
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 4F
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 50
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 51
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 52
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 53
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 54
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 55
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 56
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 57
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 58
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 59
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 5A
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 5B
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 5C
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 5D
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 5E
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 5F
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 60
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 61
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_BOUND  , true , REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::UndefinedOp    } } }, // 62
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_ARPL   , false, REG_AAX       , REG__AX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 63
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 64
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 65
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 66
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 67
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_PUSH   , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_32, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 68
    { &Decoder::Fixed_Command  , &Decoder::FxMLDispatcher , { CMD_IMUL   , false, REG_AAX       , IMM___UNSIGNED, SIZE_32, 0      , { &Decoder::LeafI0 , &Decoder::LeafI1 , &Decoder::LeafI2 , &Decoder::LeafI3         } } }, // 69
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_PUSH   , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 6A
    { &Decoder::Fixed_Command  , &Decoder::FxMLDispatcher , { CMD_IMUL   , false, REG_AAX       , IMM_____SIGNED, SIZE_8S, 0      , { &Decoder::LeafI0 , &Decoder::LeafI1 , &Decoder::LeafI2 , &Decoder::LeafI3         } } }, // 6B
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 6C
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 6D
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 6E
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 6F
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JO     , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 70
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JNO    , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 71
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JB     , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 72
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JNB    , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 73
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JZ     , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 74
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JNZ    , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 75
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JBE    , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 76
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JA     , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 77
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JS     , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 78
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JNS    , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 79
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JP     , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 7A
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JNP    , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 7B
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JL     , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 7C
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JGE    , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 7D
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JLE    , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 7E
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JG     , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 7F
    { &Decoder::Mixed_Command  , &Decoder::MultiCommand   , { AsCMD(1)   , false, REG_BAX       , IMM___UNSIGNED, SIZE_8U, 0      , { &Decoder::LeafI0 , &Decoder::LeafI1 , &Decoder::LeafI2 , &Decoder::ShiftImm3      } } }, // 80
    { &Decoder::Mixed_Command  , &Decoder::MultiCommand   , { AsCMD(1)   , false, REG_DAX       , IMM___UNSIGNED, SIZE_32, 0      , { &Decoder::LeafI0 , &Decoder::LeafI1 , &Decoder::LeafI2 , &Decoder::ShiftImm3      } } }, // 81
    { &Decoder::Mixed_Command  , &Decoder::MultiCommand   , { AsCMD(1)   , false, REG_BAX       , IMM___UNSIGNED, SIZE_8U, 0      , { &Decoder::LeafI0 , &Decoder::LeafI1 , &Decoder::LeafI2 , &Decoder::ShiftImm3      } } }, // 82
    { &Decoder::Mixed_Command  , &Decoder::MultiCommand   , { AsCMD(1)   , false, REG_DAX       , IMM___UNSIGNED, SIZE_8S, 0      , { &Decoder::LeafI0 , &Decoder::LeafI1 , &Decoder::LeafI2 , &Decoder::ShiftImm3      } } }, // 83
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_TEST   , false, REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 84
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_TEST   , false, REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 85
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_XCHG   , true , REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 86
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_XCHG   , true , REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 87
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_MOV    , false, REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 88
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_MOV    , false, REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 89
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_MOV    , true , REG_AAX       , REG__AL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 8A
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_MOV    , true , REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // 8B
    { &Decoder::Fixed_Command  , &Decoder::Fx8CDispatcher , { CMD_MOV    , false, REG_WAX       , REG_SES       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::ArithmeticSeg3 } } }, // 8C
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_LEA    , true , REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::UndefinedOp    } } }, // 8D
    { &Decoder::Fixed_Command  , &Decoder::Fx8EDispatcher , { CMD_MOV    , true , REG_WAX       , REG_SES       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::ArithmeticSeg3 } } }, // 8E
    { &Decoder::Mixed_Command  , &Decoder::Op8FDispatcher , { AsCMD(3)   , false, REG_DAX       , UINT32_MAX    , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::Tail8F         } } }, // 8F
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 90
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 91
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 92
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 93
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 94
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 95
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 96
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 97
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 98
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 99
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_CALL   , false, IMM___UNSIGNED, IMM___UNSIGNED, SIZE_32, SIZE_16, { NULL             , NULL             , NULL             , NULL                     } } }, // 9A
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 9B
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 9C
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 9D
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 9E
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // 9F
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG__AL       , DIRECT_ADDRESS, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // A0
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG_EAX       , DIRECT_ADDRESS, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // A1
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_MOV    , false, DIRECT_ADDRESS, REG__AL       , SIZE_32, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // A2
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_MOV    , false, DIRECT_ADDRESS, REG_EAX       , SIZE_32, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // A3
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // A4
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // A5
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // A6
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // A7
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_TEST   , false, REG__AL       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // A8
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_TEST   , false, REG_EAX       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // A9
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // AA
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // AB
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // AC
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // AD
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // AE
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // AF
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG__AL       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // B0
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG__CL       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // B1
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG__DL       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // B2
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG__BL       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // B3
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG__AH       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // B4
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG__CH       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // B5
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG__DH       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // B6
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG__BH       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // B7
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG_EAX       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // B8
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG_ECX       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // B9
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG_EDX       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // BA
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG_EBX       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // BB
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG_ESP       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // BC
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG_EBP       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // BD
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG_ESI       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // BE
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_MOV    , false, REG_EDI       , IMM___UNSIGNED, 0      , SIZE_32, { NULL             , NULL             , NULL             , NULL                     } } }, // BF
    { &Decoder::Mixed_Command  , &Decoder::ShfMultiCommand, { AsCMD(2)   , false, REG_BAX       , IMM___UNSIGNED, SIZE_8U, 0      , { &Decoder::LeafI0 , &Decoder::LeafI1 , &Decoder::LeafI2 , &Decoder::ShiftImm3      } } }, // C0
    { &Decoder::Mixed_Command  , &Decoder::ShfMultiCommand, { AsCMD(2)   , false, REG_DAX       , IMM___UNSIGNED, SIZE_8U, 0      , { &Decoder::LeafI0 , &Decoder::LeafI1 , &Decoder::LeafI2 , &Decoder::ShiftImm3      } } }, // C1
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_RETN   , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_16, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // C2
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // C3
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_LES    , true , REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::UndefinedOp    } } }, // C4
    { &Decoder::Fixed_Command  , &Decoder::FxDfDispatcher , { CMD_LDS    , true , REG_AAX       , REG_EAX       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::UndefinedOp    } } }, // C5
    { &Decoder::Mixed_Command  , &Decoder::MultiCommandUo , { AsCMD(4)   , false, REG_BAX       , IMM___UNSIGNED, SIZE_8U, 0      , { &Decoder::LeafI0 , &Decoder::LeafI1 , &Decoder::LeafI2 , &Decoder::ShiftImm3      } } }, // C6
    { &Decoder::Mixed_Command  , &Decoder::MultiCommandUo , { AsCMD(5)   , false, REG_DAX       , IMM___UNSIGNED, SIZE_32, 0      , { &Decoder::LeafI0 , &Decoder::LeafI1 , &Decoder::LeafI2 , &Decoder::ShiftImm3      } } }, // C7
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_ENTER  , false, IMM___UNSIGNED, IMM___UNSIGNED, SIZE_16, SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // C8
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // C9
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_RETF   , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_16, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // CA
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // CB
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // CC
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_INT    , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8U, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // CD
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // CE
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // CF
    { &Decoder::Mixed_Command  , &Decoder::ShfMultiCommand, { AsCMD(2)   , false, REG_BAX       , IMM___UNSIGNED, 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // D0
    { &Decoder::Mixed_Command  , &Decoder::ShfMultiCommand, { AsCMD(2)   , false, REG_DAX       , IMM___UNSIGNED, 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // D1
    { &Decoder::Mixed_Command  , &Decoder::MultiCommand   , { AsCMD(2)   , false, REG_BAX       , REG__CL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // D2
    { &Decoder::Mixed_Command  , &Decoder::MultiCommand   , { AsCMD(2)   , false, REG_DAX       , REG__CL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // D3
    { &Decoder::SnapInstruction, &Decoder::ArgAAccumulator, { CMD_AAM    , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8U, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // D4
    { &Decoder::SnapInstruction, &Decoder::ArgAAccumulator, { CMD_AAD    , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8U, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // D5
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // D6
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // D7
    { &Decoder::Mixed_Command  , &Decoder::MultiCommand   , { AsCMD(0)   , false, REG_DAX       , UINT32_MAX    , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::TailD8         } } }, // D8
    { &Decoder::Mixed_Command  , &Decoder::MultiPairsUo   , { CMD_NULL   , false, 0x00          , REG_NUL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::TailD9         } } }, // D9
    { &Decoder::Mixed_Command  , &Decoder::MultiPairs     , { CMD_NULL   , false, 0x08          , REG_NUL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::TailDA         } } }, // DA
    { &Decoder::Mixed_Command  , &Decoder::MultiPairsUo   , { CMD_NULL   , false, 0x10          , REG_NUL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::TailDB         } } }, // DB
    { &Decoder::Mixed_Command  , &Decoder::MultiCommand   , { AsCMD(0)   , false, REG_QAX       , UINT32_MAX    , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::TailDC         } } }, // DC
    { &Decoder::Mixed_Command  , &Decoder::MultiPairsUo   , { CMD_NULL   , false, 0x18          , REG_NUL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::TailDD         } } }, // DD
    { &Decoder::Mixed_Command  , &Decoder::MultiPairs     , { CMD_NULL   , false, 0x20          , REG_NUL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::TailDE         } } }, // DE
    { &Decoder::Mixed_Command  , &Decoder::MultiPairs     , { CMD_NULL   , false, 0x28          , REG_NUL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::TailDF         } } }, // DF
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_LOOPNE , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // E0
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_LOOPE  , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // E1
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_LOOP   , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // E2
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JECXZ  , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // E3
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_IN     , false, REG__AL       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // E4
    { &Decoder::SnapInstruction, &Decoder::Arg2Accumulator, { CMD_IN     , false, REG_EAX       , IMM___UNSIGNED, 0      , SIZE_8U, { NULL             , NULL             , NULL             , NULL                     } } }, // E5
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_OUT    , false, IMM___UNSIGNED, REG__AL       , SIZE_8U, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // E6
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_OUT    , false, IMM___UNSIGNED, REG_EAX       , SIZE_8U, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // E7
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_CALL   , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_32, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // E8
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JMP    , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_32, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // E9
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JMP    , false, IMM___UNSIGNED, IMM___UNSIGNED, SIZE_32, SIZE_16, { NULL             , NULL             , NULL             , NULL                     } } }, // EA
    { &Decoder::SnapInstruction, &Decoder::Arg1Accumulator, { CMD_JMP    , false, IMM___UNSIGNED, UINT32_MAX    , SIZE_8S, 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // EB
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // EC
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // ED
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // EE
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // EF
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // F0
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // F1
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // F2
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // F3
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // F4
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // F5
    { &Decoder::Mixed_Command  , &Decoder::MixedCommand   , { AsCMD(6)   , false, REG_BAX       , UINT32_MAX    , 0      , SIZE_8U, { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // F6
    { &Decoder::Mixed_Command  , &Decoder::MixedCommand   , { AsCMD(6)   , false, REG_DAX       , UINT32_MAX    , 0      , SIZE_32, { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::LeafLR3        } } }, // F7
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // F8
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // F9
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // FA
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // FB
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // FC
    { &Decoder::TinyInstruction, NULL                     , { CMD_NULL   , false, REG_NUL       , REG_NUL       , 0      , 0      , { NULL             , NULL             , NULL             , NULL                     } } }, // FD
    { &Decoder::Mixed_Command  , &Decoder::MultiPairsUo   , { CMD_NULL   , false, 0x30          , REG_NUL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::TailFE         } } }, // FE
    { &Decoder::Mixed_Command  , &Decoder::MultiPairsUo   , { CMD_NULL   , false, 0x38          , REG_NUL       , 0      , 0      , { &Decoder::LeafLR0, &Decoder::LeafLR1, &Decoder::LeafLR2, &Decoder::TailFF         } } }, // FF
};

const Decoder::DecodeEntry Decoder::DECODE_TABLE_L2[] =
{
    { CMD_FLD    , false, REG_DAX, UINT32_MAX, 0, 0 }, { CMD_NULL   , false, REG_NUL, REG_NUL   , 0, 0 }, // D9:0-1
    { CMD_FST    , false, REG_DAX, UINT32_MAX, 0, 0 }, { CMD_FSTP   , false, REG_DAX, UINT32_MAX, 0, 0 }, // D9:2-3
    { CMD_FLDENV , false, REG_BAX, UINT32_MAX, 0, 0 }, { CMD_FLDCW  , false, REG_WAX, UINT32_MAX, 0, 0 }, // D9:4-5
    { CMD_FNSTENV, false, REG_BAX, UINT32_MAX, 0, 0 }, { CMD_FNSTCW , false, REG_WAX, UINT32_MAX, 0, 0 }, // D9:6-7
    { CMD_FIADD  , false, REG_DAX, UINT32_MAX, 0, 0 }, { CMD_FIMUL  , false, REG_DAX, UINT32_MAX, 0, 0 }, // DA:0-1
    { CMD_FICOM  , false, REG_DAX, UINT32_MAX, 0, 0 }, { CMD_FICOMP , false, REG_DAX, UINT32_MAX, 0, 0 }, // DA:2-3
    { CMD_FISUB  , false, REG_DAX, UINT32_MAX, 0, 0 }, { CMD_FISUBR , false, REG_DAX, UINT32_MAX, 0, 0 }, // DA:4-5
    { CMD_FIDIV  , false, REG_DAX, UINT32_MAX, 0, 0 }, { CMD_FIDIVR , false, REG_DAX, UINT32_MAX, 0, 0 }, // DA:6-7
    { CMD_FILD   , false, REG_DAX, UINT32_MAX, 0, 0 }, { CMD_FISTTP , false, REG_DAX, UINT32_MAX, 0, 0 }, // DB:0-1
    { CMD_FIST   , false, REG_DAX, UINT32_MAX, 0, 0 }, { CMD_FISTP  , false, REG_DAX, UINT32_MAX, 0, 0 }, // DB:2-3
    { CMD_NULL   , false, REG_NUL, REG_NUL   , 0, 0 }, { CMD_FLD    , false, REG_TAX, UINT32_MAX, 0, 0 }, // DB:4-5
    { CMD_NULL   , false, REG_NUL, REG_NUL   , 0, 0 }, { CMD_FSTP   , false, REG_TAX, UINT32_MAX, 0, 0 }, // DB:6-7
    { CMD_FLD    , false, REG_QAX, UINT32_MAX, 0, 0 }, { CMD_FISTTP , false, REG_QAX, UINT32_MAX, 0, 0 }, // DD:0-1
    { CMD_FST    , false, REG_QAX, UINT32_MAX, 0, 0 }, { CMD_FSTP   , false, REG_QAX, UINT32_MAX, 0, 0 }, // DD:2-3
    { CMD_FRSTOR , false, REG_BAX, UINT32_MAX, 0, 0 }, { CMD_NULL   , false, REG_NUL, REG_NUL   , 0, 0 }, // DD:4-5
    { CMD_FNSAVE , false, REG_BAX, UINT32_MAX, 0, 0 }, { CMD_FNSTSW , false, REG_WAX, UINT32_MAX, 0, 0 }, // DD:6-7
    { CMD_FIADD  , false, REG_WAX, UINT32_MAX, 0, 0 }, { CMD_FIMUL  , false, REG_WAX, UINT32_MAX, 0, 0 }, // DE:0-1
    { CMD_FICOM  , false, REG_WAX, UINT32_MAX, 0, 0 }, { CMD_FICOMP , false, REG_WAX, UINT32_MAX, 0, 0 }, // DE:2-3
    { CMD_FISUB  , false, REG_WAX, UINT32_MAX, 0, 0 }, { CMD_FISUBR , false, REG_WAX, UINT32_MAX, 0, 0 }, // DE:4-5
    { CMD_FIDIV  , false, REG_WAX, UINT32_MAX, 0, 0 }, { CMD_FIDIVR , false, REG_WAX, UINT32_MAX, 0, 0 }, // DE:6-7
    { CMD_FILD   , false, REG_WAX, UINT32_MAX, 0, 0 }, { CMD_FISTTP , false, REG_WAX, UINT32_MAX, 0, 0 }, // DF:0-1
    { CMD_FIST   , false, REG_WAX, UINT32_MAX, 0, 0 }, { CMD_FISTP  , false, REG_WAX, UINT32_MAX, 0, 0 }, // DF:2-3
    { CMD_FBLD   , false, REG_TAX, UINT32_MAX, 0, 0 }, { CMD_FILD   , false, REG_QAX, UINT32_MAX, 0, 0 }, // DF:4-5
    { CMD_FBSTP  , false, REG_TAX, UINT32_MAX, 0, 0 }, { CMD_FISTP  , false, REG_QAX, UINT32_MAX, 0, 0 }, // DF:6-7
    { CMD_INC    , false, REG_BAX, UINT32_MAX, 0, 0 }, { CMD_DEC    , false, REG_BAX, UINT32_MAX, 0, 0 }, // FE:0-1
    { CMD_NULL   , false, REG_NUL, REG_NUL   , 0, 0 }, { CMD_NULL   , false, REG_NUL, REG_NUL   , 0, 0 }, // FE:2-3
    { CMD_NULL   , false, REG_NUL, REG_NUL   , 0, 0 }, { CMD_NULL   , false, REG_NUL, REG_NUL   , 0, 0 }, // FE:4-5
    { CMD_NULL   , false, REG_NUL, REG_NUL   , 0, 0 }, { CMD_NULL   , false, REG_NUL, REG_NUL   , 0, 0 }, // FE:6-7
    { CMD_INC    , false, REG_DAX, UINT32_MAX, 0, 0 }, { CMD_DEC    , false, REG_DAX, UINT32_MAX, 0, 0 }, // FF:0-1
    { CMD_CALL   , false, REG_DAX, UINT32_MAX, 0, 0 }, { CMD_CALL   , false, REG_FAX, UINT32_MAX, 0, 0 }, // FF:2-3
    { CMD_JMP    , false, REG_DAX, UINT32_MAX, 0, 0 }, { CMD_JMP    , false, REG_FAX, UINT32_MAX, 0, 0 }, // FF:4-5
    { CMD_PUSH   , false, REG_DAX, UINT32_MAX, 0, 0 }, { CMD_NULL   , false, REG_NUL, UINT32_MAX, 0, 0 }, // FF:6-7
};

inline int Decoder::CommitCommand(const uint32_t COMMAND)
{
    custom_cmd.command    = COMMAND   ;
    custom_cmd.first__arg = UINT32_MAX;

    return Finished(0);
}

inline int Decoder::CommitOperation(const uint32_t LEFT__ARGUMENT, const uint32_t RIGHT_ARGUMENT)
{
    custom_cmd.first__arg = LEFT__ARGUMENT;
    custom_cmd.second_arg = RIGHT_ARGUMENT;
    custom_cmd.third__arg =     UINT32_MAX;

    return Finished(0);
}

inline int Decoder::CommitOperation(const uint32_t COMMAND, const uint32_t LEFT__ARGUMENT, const uint32_t RIGHT_ARGUMENT)
{
    custom_cmd.command    =        COMMAND;
    custom_cmd.first__arg = LEFT__ARGUMENT;
    custom_cmd.second_arg = RIGHT_ARGUMENT;
    custom_cmd.third__arg =     UINT32_MAX;

    return Finished(0);
}

inline int Decoder::CommitPredicateReg(const uint8_t BASE, const uint8_t OPCODE)
{
    custom_cmd.first__arg = BASE + Bit012(OPCODE);
    custom_cmd.second_arg = UINT32_MAX           ;

    return Finished(0);
}

inline int Decoder::CommitPredicateReg(const uint32_t COMMAND, const uint8_t BASE, const uint8_t OPCODE)
{
    custom_cmd.command = COMMAND;

    return CommitPredicateReg(BASE, OPCODE);
}

inline uint32_t Decoder::DirTail(const uint8_t OPCODE) const
{
    if (Bit345(OPCODE) == REG__AH)
        return MakeUint32(           DIRECT_ADDRESS2                , static_cast<uint16_t>(dte.de->left__arg                 ));
    else
        return MakeUint32(MakeUint16(DIRECT___INDEX, Bit_67(OPCODE)), static_cast<uint16_t>(dte.de->left__arg + Bit345(OPCODE)));
}

inline int Decoder::Continue   (       int    (Decoder::*next    )(const uint8_t)) { handler = next          ; return STATUS_CONTINUE      ; }
inline int Decoder::Finished   (const  int               IMM_SIZE                ) { handler = &Decoder::Main; return IMM_SIZE             ; }
inline int Decoder::UndefinedOp(const uint8_t                                    ) { handler = &Decoder::Main; return STATUS_INVALID_OPCODE; }

int Decoder::Main(const uint8_t OPCODE) { return (this->*DECODE_TABLE[OPCODE].root_dispatcher)(OPCODE); }

int Decoder::Fixed_Command(const uint8_t OPCODE)
{
    command = &custom_cmd;

    dte.dde            = &DECODE_TABLE[OPCODE].de           ;
    custom_cmd.command =  DECODE_TABLE[OPCODE].de.de.command;

    return Continue(DECODE_TABLE[OPCODE].main_dispatcher);
}

int Decoder::Mixed_Command(const uint8_t OPCODE)
{
    command = &custom_cmd             ;
    dte.dde = &DECODE_TABLE[OPCODE].de;

    return Continue(DECODE_TABLE[OPCODE].main_dispatcher);
}

int Decoder::SnapInstruction(const uint8_t OPCODE)
{
    command = &custom_cmd             ;
    dte.dde = &DECODE_TABLE[OPCODE].de;

    custom_cmd.command    = DECODE_TABLE[OPCODE].de.de.command  ;
    custom_cmd.first__arg = DECODE_TABLE[OPCODE].de.de.left__arg;
    custom_cmd.second_arg = DECODE_TABLE[OPCODE].de.de.right_arg;

    custom_cmd.first__val.Setup(DECODE_TABLE[OPCODE].de.de.left__size);
    custom_cmd.second_val.Setup(DECODE_TABLE[OPCODE].de.de.right_size);

    return Continue(DECODE_TABLE[OPCODE].main_dispatcher);
}

int Decoder::TinyInstruction(const uint8_t OPCODE)
{
    command = &OPCODE_TABLE_L0[OPCODE];

    return Finished(0);
}

int Decoder::Arg1Accumulator(const uint8_t OPCODE)
{
    const int status = custom_cmd.first__val.Push(OPCODE);

    if (status == STATUS_CONTINUE)
        return STATUS_CONTINUE;

    if (custom_cmd.second_val.IsComplete() == false)
        return Continue(&Decoder::Arg2Accumulator);
    else
        return Finished(status                   );
}

int Decoder::Arg2Accumulator(const uint8_t OPCODE)
{
    const int status = custom_cmd.second_val.Push(OPCODE);

    if (status == STATUS_CONTINUE)
        return STATUS_CONTINUE ;
    else
        return Finished(custom_cmd.first__val.GetCounter() + status);
}

int Decoder::ArgAAccumulator(const uint8_t OPCODE)
{
    if (OPCODE == 0x0A)
    {
        custom_cmd.first__arg = UINT32_MAX;

        return Finished(0);
    }
    else
        return Arg1Accumulator(OPCODE);
}

int Decoder::Fx8CDispatcher(const uint8_t OPCODE)
{
    if (Bit345(OPCODE) <= REG_SGS - REG_SES)
        return (this->*dte.dde->dispatcher[Bit_67(OPCODE)])(OPCODE);
    else
        return UndefinedOp();
}

int Decoder::Fx8EDispatcher(const uint8_t OPCODE)
{
    if ((Bit345(OPCODE) != REG_SCS - REG_SES) && (Bit345(OPCODE) <= REG_SGS - REG_SES))
        return (this->*dte.dde->dispatcher[Bit_67(OPCODE)])(OPCODE);
    else
        return UndefinedOp();
}

int Decoder::FxDfDispatcher(const uint8_t OPCODE)
{
    return (this->*dte.dde->dispatcher[Bit_67(OPCODE)])(OPCODE);
}

int Decoder::FxMLDispatcher(const uint8_t OPCODE)
{
    custom_cmd.third__arg = REG_EAX + Bit345(OPCODE);

    return (this->*dte.dde->dispatcher[Bit_67(OPCODE)])(OPCODE);
}

int Decoder::MixedCommand(const uint8_t OPCODE)
{
    static const Handler PREDICATE_DT[] = {&Decoder::LeafI0, &Decoder::LeafI1, &Decoder::LeafI2, &Decoder::ShiftImm3 };

    custom_cmd.command = COMMAND_TABLE[dte.dde->de.command * CHAR_BIT + Bit345(OPCODE)];

    if (custom_cmd.command == CMD_TEST)
    {
        static DecodeEntry pdt = { CMD_NULL, false, REG_NUL, IMM___UNSIGNED, 0, 0 };

        pdt.left__arg  = dte.dde->de.left__arg ;
        pdt.left__size = dte.dde->de.right_size;
        dte.de         = &pdt                  ;

        return (this->*PREDICATE_DT[Bit_67(OPCODE)])(OPCODE);
    }
    else
        return (this->*dte.dde->dispatcher[Bit_67(OPCODE)])(OPCODE);
    }

int Decoder::MultiCommand(const uint8_t OPCODE)
{
    custom_cmd.command = COMMAND_TABLE[dte.dde->de.command * CHAR_BIT + Bit345(OPCODE)];

    return (this->*dte.dde->dispatcher[Bit_67(OPCODE)])(OPCODE);
}

int Decoder::MultiCommandUo(const uint8_t OPCODE)
{
    const int STATUS = Op8FDispatcher(OPCODE);

    if ((STATUS == STATUS_INVALID_OPCODE) && (OPCODE == 0xF8))
    {
        custom_cmd.command    = (dte.dde->de.command == 4) ? (CMD_XABORT) : (CMD_XBEGIN);
        custom_cmd.second_arg = UINT32_MAX                                              ;

        return AccumulateL010(IMM___UNSIGNED, dte.dde->de.left__size);
    }
    else
        return STATUS;
}

int Decoder::MultiPairs(const uint8_t OPCODE)
{
    const Handler HANDLER = dte.dde->dispatcher[Bit_67(OPCODE)];

    dte.de             = &DECODE_TABLE_L2[dte.dde->de.left__arg + Bit345(OPCODE)];
    custom_cmd.command =                  dte.dde->de.command                    ;

    return (this->*HANDLER)(OPCODE);
}

int Decoder::MultiPairsUo(const uint8_t OPCODE)
{
    if (OPCODE < 0xC0)
        if (DECODE_TABLE_L2[dte.dde->de.left__arg + Bit345(OPCODE)].command != CMD_NULL)
            return MultiPairs(OPCODE);
        else
            return UndefinedOp();
    else
        return (this->*dte.dde->dispatcher[Bit_67(OPCODE)])(OPCODE);
}

int Decoder::Op8FDispatcher(const uint8_t OPCODE)
{
    return (COMMAND_TABLE[dte.dde->de.command * CHAR_BIT + Bit345(OPCODE)] != CMD_NULL) ? (MultiCommand(OPCODE)) : (UndefinedOp());
}

int Decoder::ShfMultiCommand(const uint8_t OPCODE)
{
    custom_cmd.second_val.Setup(SIZE_8U);
    custom_cmd.second_val.Push (ONE    );

    return MultiCommand(OPCODE);
}

int Decoder::LeafLR0(const uint8_t OPCODE) { custom_cmd.second_arg = dte.de->right_arg + Bit345(OPCODE); return LeafR0(OPCODE); }
int Decoder::LeafLR1(const uint8_t OPCODE) { custom_cmd.second_arg = dte.de->right_arg + Bit345(OPCODE); return LeafR1(OPCODE); }
int Decoder::LeafLR2(const uint8_t OPCODE) { custom_cmd.second_arg = dte.de->right_arg + Bit345(OPCODE); return LeafR2(OPCODE); }
int Decoder::LeafLr0(const uint8_t OPCODE) { custom_cmd.second_arg = dte.de->right_arg                 ; return LeafR0(OPCODE); }
int Decoder::LeafLr1(const uint8_t OPCODE) { custom_cmd.second_arg = dte.de->right_arg                 ; return LeafR1(OPCODE); }
int Decoder::LeafLr2(const uint8_t OPCODE) { custom_cmd.second_arg = dte.de->right_arg                 ; return LeafR2(OPCODE); }

int Decoder::LeafLR3(const uint8_t OPCODE)
{
    return CommitOperation(dte.de->right_arg + Bit012(OPCODE)          , dte.de->right_arg + Bit345(OPCODE));
}

int Decoder::LeafLr3(const uint8_t OPCODE)
{
    return CommitOperation(dte.de->left__arg + Bit012(OPCODE) - REG_BAX, dte.de->right_arg                 );
}

int Decoder::LeafI0(const uint8_t OPCODE)
{
    switch (Bit012(OPCODE))
    {
        case REG__AH: return Continue      (&Decoder::LeafI0Tail                                   );
        case REG__CH: return AccumulateLX1X(MakeUint32(DIRECT_ADDRESS2, dte.de->left__arg), SIZE_32);
        default     : return AccumulateLX0X(Bit012(OPCODE)            + dte.de->left__arg          );
    }
}

int Decoder::LeafI1(const uint8_t OPCODE)
{
    if (Bit012(OPCODE) != REG__AH)
        return AccumulateLX1X(BASE_WITH_OFFSET + Bit012(OPCODE) + dte.de->left__arg, SIZE_8S);

    custom_cmd.second_arg = dte.de->right_arg;

    return Continue(&Decoder::LeafI1Tail);
}

int Decoder::LeafI2(const uint8_t OPCODE)
{
    if (Bit012(OPCODE) != REG__AH)
        return AccumulateLX1X(BASE_WITH_OFFSET + Bit012(OPCODE) + dte.de->left__arg, SIZE_32);

    custom_cmd.second_arg = dte.de->right_arg;

    return Continue(&Decoder::LeafI2Tail);
}

int Decoder::LeafI3(const uint8_t OPCODE)
{
    if (Bit345(OPCODE) == Bit012(OPCODE))
        custom_cmd.third__arg = UINT32_MAX;

    return AccumulateLX0X(REG_EAX + Bit012(OPCODE));
}

int Decoder::LeafR0(const uint8_t OPCODE)
{
    switch (Bit012(OPCODE))
    {
        case REG__AH: return Continue      (&Decoder::LeafR0Tail                                   );
        case REG__CH: return AccumulateL010(MakeUint32(DIRECT_ADDRESS2, dte.de->left__arg), SIZE_32);
        default     : custom_cmd.first__arg = Bit012(OPCODE) + dte.de->left__arg                    ;
                      return Finished(0)                                                            ;
    }
}

int Decoder::LeafR1(const uint8_t OPCODE)
{
    if (Bit012(OPCODE) != REG__AH)
        return AccumulateL010(BASE_WITH_OFFSET + Bit012(OPCODE) + dte.de->left__arg, SIZE_8S);
    else
        return Continue(&Decoder::LeafR1Tail);
}

int Decoder::LeafR2(const uint8_t OPCODE)
{
    if (Bit012(OPCODE) != REG__AH)
        return AccumulateL010(BASE_WITH_OFFSET + Bit012(OPCODE) + dte.de->left__arg, SIZE_32);
    else
        return Continue(&Decoder::LeafR2Tail);
}

int Decoder::ArithmeticSeg3(const uint8_t OPCODE)
{
    return CommitOperation(REG_EAX + Bit012(OPCODE), dte.de->right_arg + Bit345(OPCODE));
}


int Decoder::ShiftImm3(const uint8_t OPCODE) { return AccumulateLX0X    (dte.de->left__arg + Bit012(OPCODE) - REG_BAX        ); }
int Decoder::Tail8F   (const uint8_t OPCODE) { return CommitPredicateReg(                                     REG_EAX, OPCODE); }

int Decoder::TailD9(const uint8_t OPCODE)
{
    if (CMD_D9E[Bit345(OPCODE)] != CMD_NULL)
        return CommitPredicateReg(CMD_D9E[Bit345(OPCODE)], REG_ST0, OPCODE);

    switch (OPCODE)
    {
        case 0xD0: return CommitCommand(CMD_FNOP   ); break;
        case 0xE0: return CommitCommand(CMD_FCHS   ); break;
        case 0xE1: return CommitCommand(CMD_FABS   ); break;
        case 0xE4: return CommitCommand(CMD_FTST   ); break;
        case 0xE5: return CommitCommand(CMD_FXAM   ); break;
        case 0xE8: return CommitCommand(CMD_FLD1   ); break;
        case 0xE9: return CommitCommand(CMD_FLDL2T ); break;
        case 0xEA: return CommitCommand(CMD_FLDL2E ); break;
        case 0xEB: return CommitCommand(CMD_FLDPI  ); break;
        case 0xEC: return CommitCommand(CMD_FLDLG2 ); break;
        case 0xED: return CommitCommand(CMD_FLDLN2 ); break;
        case 0xEE: return CommitCommand(CMD_FLDZ   ); break;
        case 0xF0: return CommitCommand(CMD_F2XM1  ); break;
        case 0xF1: return CommitCommand(CMD_FYL2X  ); break;
        case 0xF2: return CommitCommand(CMD_FPTAN  ); break;
        case 0xF3: return CommitCommand(CMD_FPATAN ); break;
        case 0xF4: return CommitCommand(CMD_FXTRACT); break;
        case 0xF5: return CommitCommand(CMD_FPREM1 ); break;
        case 0xF6: return CommitCommand(CMD_FDECSTP); break;
        case 0xF7: return CommitCommand(CMD_FINCSTP); break;
        case 0xF8: return CommitCommand(CMD_FPREM  ); break;
        case 0xF9: return CommitCommand(CMD_FYL2XP1); break;
        case 0xFA: return CommitCommand(CMD_FSQRT  ); break;
        case 0xFB: return CommitCommand(CMD_FSINCOS); break;
        case 0xFC: return CommitCommand(CMD_FRNDINT); break;
        case 0xFD: return CommitCommand(CMD_FSCALE ); break;
        case 0xFE: return CommitCommand(CMD_FSIN   ); break;
        case 0xFF: return CommitCommand(CMD_FCOS   ); break;
        default  : return UndefinedOp  (           ); break;
    }
}

int Decoder::TailDA(const uint8_t OPCODE)
{
    if (CMD_DAE[Bit345(OPCODE)] != CMD_NULL)
        return CommitOperation(CMD_DAE[Bit345(OPCODE)], REG_ST0, REG_ST0 + Bit012(OPCODE));
    else
        return (OPCODE == 0xE9) ? (CommitCommand(CMD_FUCOMPP)) : (UndefinedOp());
}

int Decoder::TailDB(const uint8_t OPCODE)
{
    if (CMD_DBE[Bit345(OPCODE)] != CMD_NULL)
        return CommitOperation(CMD_DBE[Bit345(OPCODE)], REG_ST0, REG_ST0 + Bit012(OPCODE));

    switch (OPCODE)
    {
        case 0xE0: return CommitCommand(CMD_FNENI ); break;
        case 0xE1: return CommitCommand(CMD_FNDISI); break;
        case 0xE2: return CommitCommand(CMD_FNCLEX); break;
        case 0xE3: return CommitCommand(CMD_FNINIT); break;
        case 0xE4: return CommitCommand(CMD_FSETPM); break;
        default  : return UndefinedOp  (          ); break;
    }
}

int Decoder::TailD8(const uint8_t OPCODE)
{
    if ((custom_cmd.command == CMD_FCOM) || (custom_cmd.command == CMD_FCOMP))
        return CommitPredicateReg(REG_ST0,                  OPCODE );
    else
        return CommitOperation   (REG_ST0, REG_ST0 + Bit012(OPCODE));
}

int Decoder::TailDC(const uint8_t OPCODE)
{
    switch(custom_cmd.command)
    {
        case CMD_FSUB : return CommitOperation   (CMD_FSUBR, REG_ST0 + Bit012(OPCODE), REG_ST0);
        case CMD_FSUBR: return CommitOperation   (CMD_FSUB , REG_ST0 + Bit012(OPCODE), REG_ST0);
        case CMD_FDIV : return CommitOperation   (CMD_FDIVR, REG_ST0 + Bit012(OPCODE), REG_ST0);
        case CMD_FDIVR: return CommitOperation   (CMD_FDIV , REG_ST0 + Bit012(OPCODE), REG_ST0);
        case CMD_FCOM : return CommitPredicateReg(REG_ST0  ,                  OPCODE          );
        case CMD_FCOMP: return CommitPredicateReg(REG_ST0  ,                  OPCODE          );
        default:        return CommitOperation   (           REG_ST0 + Bit012(OPCODE), REG_ST0);
    }
}

int Decoder::TailDD(const uint8_t OPCODE)
{
    if (CMD_DDE[Bit345(OPCODE)] != CMD_NULL)
        return CommitPredicateReg(CMD_DDE[Bit345(OPCODE)], REG_ST0, OPCODE);
    else
        return UndefinedOp       (                                        );
}

int Decoder::TailDE(const uint8_t OPCODE)
{
    if (CMD_DEE[Bit345(OPCODE)] != CMD_NULL)
        if (OPCODE == 0xD1)
            return CommitPredicateReg(CMD_DEE[Bit345(OPCODE)], REG_ST1, REG__AL);
        else
            return CommitOperation(CMD_DEE[Bit345(OPCODE)], REG_ST0 + Bit012(OPCODE), REG_ST0);
    else
        return (OPCODE == 0xD9) ? (CommitCommand(CMD_FCOMPP)) : (UndefinedOp());
}

int Decoder::TailDF(const uint8_t OPCODE)
{
    if (CMD_DFE[Bit345(OPCODE)] != CMD_NULL)
        if ((CMD_DFE[Bit345(OPCODE)] == CMD_FUCOMIP) || (CMD_DFE[Bit345(OPCODE)] == CMD_FCOMIP))
            return CommitOperation(CMD_DFE[Bit345(OPCODE)], REG_ST0, REG_ST0 + Bit012(OPCODE));
        else
            return CommitPredicateReg(CMD_DFE[Bit345(OPCODE)], REG_ST0, OPCODE);
    else
        return (OPCODE == 0xE0) ? (CommitPredicateReg(CMD_FNSTSW, REG__AX, REG__AL)) : (UndefinedOp());
}

int Decoder::TailFE(const uint8_t OPCODE)
{
    if (DECODE_TABLE_L2[0x30 + Bit345(OPCODE)].command != CMD_NULL)
        return CommitPredicateReg(DECODE_TABLE_L2[0x30 + Bit345(OPCODE)].command, REG__AL, OPCODE);
    else
        return UndefinedOp       (                                                               );
}

int Decoder::TailFF(const uint8_t OPCODE)
{
    if (CMD_FFE[Bit345(OPCODE)] != CMD_NULL)
        return CommitPredicateReg(CMD_FFE[Bit345(OPCODE)], REG_EAX, OPCODE);
    else
        return UndefinedOp       (                                        );
}

int Decoder::LeafR0Tail(const uint8_t OPCODE)
{
    if (Bit012(OPCODE) == REG__CH)
        return AccumulateL010(DirTail(OPCODE), SIZE_32);

    custom_cmd.first__arg = PackArgument(OPCODE, 0, WITH_____INDEX);

    return Finished(0);
}

int Decoder::LeafI0Tail(const uint8_t OPCODE)
{
    if (Bit012(OPCODE) == REG__CH)
        return AccumulateL01X(DirTail     (OPCODE                   ), SIZE_32);
    else
        return AccumulateLX0X(PackArgument(OPCODE, 0, WITH_____INDEX)         );
}

int Decoder::LeafR1Tail(const uint8_t OPCODE) { return AccumulateL010(PackArgument(OPCODE, WITH____OFFSET, WITH___ADDRESS), SIZE_8S); }
int Decoder::LeafI1Tail(const uint8_t OPCODE) { return AccumulateL01X(PackArgument(OPCODE, WITH____OFFSET, WITH___ADDRESS), SIZE_8S); }
int Decoder::LeafR2Tail(const uint8_t OPCODE) { return AccumulateL010(PackArgument(OPCODE, WITH____OFFSET, WITH___ADDRESS), SIZE_32); }
int Decoder::LeafI2Tail(const uint8_t OPCODE) { return AccumulateL01X(PackArgument(OPCODE, WITH____OFFSET, WITH___ADDRESS), SIZE_32); }

int Decoder::AccumulateL01X(const uint32_t LEFT__TYPE, const int8_t SIZE_L)
{
    custom_cmd.first__arg = LEFT__TYPE;

    custom_cmd.first__val.Setup(SIZE_L            );
    custom_cmd.second_val.Setup(dte.de->left__size);

    return Continue(&Decoder::Arg1Accumulator);
}

int Decoder::AccumulateLX1X(const uint32_t LEFT__TYPE, const int8_t SIZE_L)
{
    custom_cmd.first__arg = LEFT__TYPE       ;
    custom_cmd.second_arg = dte.de->right_arg;

    custom_cmd.first__val.Setup(SIZE_L            );
    custom_cmd.second_val.Setup(dte.de->left__size);

    return Continue(&Decoder::Arg1Accumulator);
}

int Decoder::AccumulateLX0X(const uint32_t LEFT__TYPE)
{
    custom_cmd.first__arg = LEFT__TYPE       ;
    custom_cmd.second_arg = dte.de->right_arg;

    custom_cmd.first__val.Setup(0                 );
    custom_cmd.second_val.Setup(dte.de->left__size);

    return Continue(&Decoder::Arg2Accumulator);
}

int Decoder::AccumulateL010(const uint32_t LEFT__TYPE, const int8_t SIZE_1)
{
    custom_cmd.first__arg = LEFT__TYPE;

    custom_cmd.first__val.Setup(SIZE_1);

    return Continue(&Decoder::Arg1Accumulator);
}

uint32_t Decoder::PackArgument(const uint8_t OPCODE, const uint8_t TYPE_A, const uint8_t TYPE_X)
{
    const uint8_t INDEX_REG = Bit345(OPCODE);

    uint8_t index = REG_EAX + INDEX_REG;
    uint8_t scale =      Bit_67(OPCODE);
    uint8_t type  =             TYPE_X ;

    if (INDEX_REG == REG__AH)
    {
        index =      0;
        scale =      0;
        type  = TYPE_A;
    }

    return MakeUint32(MakeUint16(type, scale), MakeUint16(index, static_cast<uint8_t>(dte.de->left__arg + Bit012(OPCODE))));
}

