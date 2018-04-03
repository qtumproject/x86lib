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
#pragma once

#include <climits>
#include <cstddef>
#include <cstdint>

enum INSTRUCTION
{
    CMD_AAA     , CMD_AAD     , CMD_AAM     , CMD_AAS     , CMD_ADC     , CMD_ADD     , CMD_AND     , CMD_ARPL    ,
    CMD_BOUND   , CMD_BSWAP   , CMD_CALL    , CMD_CDQ     , CMD_CLC     , CMD_CLD     , CMD_CLI     , CMD_CLTS    ,
    CMD_CMC     , CMD_CMP     , CMD_CMPSB   , CMD_CMPSD   , CMD_CPUID   , CMD_CWDE    , CMD_DAA     , CMD_DAS     ,
    CMD_DEC     , CMD_DIV     , CMD_EMMS    , CMD_ENTER   , CMD_F2XM1   , CMD_FABS    , CMD_FADD    , CMD_FADDP   ,
    CMD_FBLD    , CMD_FBSTP   , CMD_FCHS    , CMD_FCMOVB  , CMD_FCMOVBE , CMD_FCMOVE  , CMD_FCMOVNB , CMD_FCMOVNBE,
    CMD_FCMOVNE , CMD_FCMOVNU , CMD_FCMOVU  , CMD_FCOM    , CMD_FCOMI   , CMD_FCOMIP  , CMD_FCOMP   , CMD_FCOMP5  ,
    CMD_FCOMPP  , CMD_FCOS    , CMD_FDECSTP , CMD_FDIV    , CMD_FDIVP   , CMD_FDIVR   , CMD_FDIVRP  , CMD_FEMMS   ,
    CMD_FFREE   , CMD_FFREEP  , CMD_FIADD   , CMD_FICOM   , CMD_FICOMP  , CMD_FIDIV   , CMD_FIDIVR  , CMD_FILD    ,
    CMD_FIMUL   , CMD_FINCSTP , CMD_FIST    , CMD_FISTP   , CMD_FISTTP  , CMD_FISUB   , CMD_FISUBR  , CMD_FLD     ,
    CMD_FLD1    , CMD_FLDCW   , CMD_FLDENV  , CMD_FLDL2E  , CMD_FLDL2T  , CMD_FLDLG2  , CMD_FLDLN2  , CMD_FLDPI   ,
    CMD_FLDZ    , CMD_FMUL    , CMD_FMULP   , CMD_FNCLEX  , CMD_FNDISI  , CMD_FNENI   , CMD_FNINIT  , CMD_FNSAVE  ,
    CMD_FNSTCW  , CMD_FNSTENV , CMD_FNSTSW  , CMD_FNOP    , CMD_FPATAN  , CMD_FPREM   , CMD_FPREM1  , CMD_FPTAN   ,
    CMD_FRNDINT , CMD_FRSTOR  , CMD_FSCALE  , CMD_FSETPM  , CMD_FSIN    , CMD_FSINCOS , CMD_FSQRT   , CMD_FST     ,
    CMD_FSTP    , CMD_FSTP1   , CMD_FSTP8   , CMD_FSTP9   , CMD_FSUB    , CMD_FSUBR   , CMD_FSUBRP  , CMD_FSUBP   ,
    CMD_FTST    , CMD_FUCOM   , CMD_FUCOMI  , CMD_FUCOMIP , CMD_FUCOMP  , CMD_FUCOMPP , CMD_FXAM    , CMD_FXCH    ,
    CMD_FXCH4   , CMD_FXCH7   , CMD_FXTRACT , CMD_FYL2X   , CMD_FYL2XP1 , CMD_GETSEC  , CMD_HLT     , CMD_ICEBP   ,
    CMD_IDIV    , CMD_IMUL    , CMD_IN      , CMD_INC     , CMD_INSB    , CMD_INSD    , CMD_INT     , CMD_INTO    ,
    CMD_IRET    , CMD_INVD    , CMD_JA      , CMD_JB      , CMD_JECXZ   , CMD_JBE     , CMD_JG      , CMD_JGE     ,
    CMD_JL      , CMD_JMP     , CMD_JLE     , CMD_JNB     , CMD_JNO     , CMD_JNP     , CMD_JNS     , CMD_JNZ     ,
    CMD_JO      , CMD_JP      , CMD_JS      , CMD_JZ      , CMD_LAHF    , CMD_LDS     , CMD_LEA     , CMD_LEAVE   ,
    CMD_LES     , CMD_LOCK    , CMD_LODSB   , CMD_LODSD   , CMD_LOOP    , CMD_LOOPE   , CMD_LOOPNE  , CMD_MOV     ,
    CMD_MOVSB   , CMD_MOVSD   , CMD_MUL     , CMD_NEG     , CMD_NOP     , CMD_NOT     , CMD_OR      , CMD_OUT     ,
    CMD_OUTSB   , CMD_OUTSD   , CMD_PAUSE   , CMD_POP     , CMD_POPA    , CMD_POPF    , CMD_PUSH    , CMD_PUSHA   ,
    CMD_PUSHF   , CMD_RCL     , CMD_RCR     , CMD_RDMSR   , CMD_RDPMC   , CMD_RDTSC   , CMD_REPNE   , CMD_RETF    ,
    CMD_RETN    , CMD_RMS     , CMD_ROL     , CMD_ROR     , CMD_SAHF    , CMD_SAL     , CMD_SAR     , CMD_SBB     ,
    CMD_SCASB   , CMD_SCASD   , CMD_SETALC  , CMD_SHL     , CMD_SHR     , CMD_STC     , CMD_STD     , CMD_STI     ,
    CMD_STOSB   , CMD_STOSD   , CMD_SUB     , CMD_SYSCALL , CMD_SYSENTER, CMD_SYSEXIT , CMD_SYSRET  , CMD_TEST    ,
    CMD_UD2     , CMD_WAIT    , CMD_WBINVD  , CMD_WRMSR   , CMD_XABORT  , CMD_XBEGIN  , CMD_XCHG    , CMD_XLAT    ,
    CMD_XOR     , PREFIX_26   , PREFIX_2E   , PREFIX_36   , PREFIX_3E   , PREFIX_64   , PREFIX_65   , PREFIX_66   ,
    PREFIX_67   , PREFIX_F0   , PREFIX_F2   , PREFIX_F3   , CMD_NULL
};

enum
{
    REG__AL, REG__CL, REG__DL, REG__BL, REG__AH, REG__CH, REG__DH, REG__BH,
    REG__AX, REG__CX, REG__DX, REG__BX, REG__SP, REG__BP, REG__SI, REG__DI,
    REG_EAX, REG_ECX, REG_EDX, REG_EBX, REG_ESP, REG_EBP, REG_ESI, REG_EDI,
    REG_AAX, REG_ACX, REG_ADX, REG_ABX, REG_ASP, REG_ABP, REG_ASI, REG_ADI,
    REG_BAX, REG_BCX, REG_BDX, REG_BBX, REG_BSP, REG_BBP, REG_BSI, REG_BDI,
    REG_WAX, REG_WCX, REG_WDX, REG_WBX, REG_WSP, REG_WBP, REG_WSI, REG_WDI,
    REG_DAX, REG_DCX, REG_DDX, REG_DBX, REG_DSP, REG_DBP, REG_DSI, REG_DDI,
    REG_FAX, REG_FCX, REG_FDX, REG_FBX, REG_FSP, REG_FBP, REG_FSI, REG_FDI,
    REG_QAX, REG_QCX, REG_QDX, REG_QBX, REG_QSP, REG_QBP, REG_QSI, REG_QDI,
    REG_TAX, REG_TCX, REG_TDX, REG_TBX, REG_TSP, REG_TBP, REG_TSI, REG_TDI,
    REG_ST0, REG_ST1, REG_ST2, REG_ST3, REG_ST4, REG_ST5, REG_ST6, REG_ST7,
    REG_SES, REG_SCS, REG_SSS, REG_SDS, REG_SFS, REG_SGS, REG_NUL
};

static const uint32_t REGULAR____ARG = 0x00000000UL;
static const uint32_t DIRECT_ADDRESS = 0x01000000UL;
static const uint32_t DIRECT___INDEX = 0x02        ;
static const uint32_t IMM___UNSIGNED = 0x03000000UL;
static const uint32_t IMM_____SIGNED = 0x04000000UL;
static const uint8_t  WITH___ADDRESS = 0x06        ;
static const uint8_t  WITH_____INDEX = 0x07        ;
static const uint8_t  WITH____OFFSET = 0x08        ;

static const uint16_t DIRECT_ADDRESS2 = 0x0100;

static const int STATUS_CONTINUE       = -1;
static const int STATUS_INVALID_OPCODE = -2;

class ImmediateValue
{
public:
    inline explicit  ImmediateValue(const uint32_t VALUE = 0) : counter(0), limit(0), value(VALUE) {}
    inline          ~ImmediateValue(      void              )                                      {}

    inline uint32_t Get       (void) const { return  value                                   ; }
    inline int      GetCounter(void) const { return  counter                                 ; }
    inline bool     IsComplete(void) const { return (counter == limit) || (counter == -limit); }

    int  Push (const uint8_t OPCODE);
    void Setup(const  int8_t SIZE  );

private:
     int32_t counter;
     int32_t limit  ;
    uint32_t value  ;
};

struct CpuInstruction
{
public:
    inline CpuInstruction(const uint32_t CMD, const uint32_t ARG_1, const uint32_t ARG_2, const uint32_t VALUE);

    uint32_t command   ;
    uint32_t first__arg;
    uint32_t second_arg;
    uint32_t third__arg;

    ImmediateValue first__val;
    ImmediateValue second_val;
};

class Decoder
{
public:
    inline  Decoder(void) : command(NULL), custom_cmd(UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX), dte(), handler(&Decoder::Main) {}
    inline ~Decoder(void)                                                                                                             {}

    inline const uint32_t GetCmd (void) const { return                                                           command->command          ; }
    inline const uint32_t GetArg1(void) const { return (dte.de->backward_order) ? (command->second_arg      ) : (command->first__arg      ); }
    inline const uint32_t GetArg2(void) const { return (dte.de->backward_order) ? (command->first__arg      ) : (command->second_arg      ); }
    inline const uint32_t GetArg3(void) const { return                                                           command->third__arg       ; }
    inline const uint32_t GetVal1(void) const { return (dte.de->backward_order) ? (command->second_val.Get()) : (command->first__val.Get()); }
    inline const uint32_t GetVal2(void) const { return (dte.de->backward_order) ? (command->first__val.Get()) : (command->second_val.Get()); }

    inline int operator()(const uint8_t OPCODE) { return (this->*handler)(OPCODE); }

private:
    typedef int (Decoder::*Handler)(const uint8_t);

    struct DecodeEntry
    {
        INSTRUCTION command       ;
               bool backward_order;
           uint32_t left__arg     ;
           uint32_t right_arg     ;
             int8_t left__size    ;
             int8_t right_size    ;
    };

    struct DispDecodeEntry
    {
        DecodeEntry de           ;
        Handler     dispatcher[4];
    };

    struct RootDecodeEntry
    {
        Handler         root_dispatcher;
        Handler         main_dispatcher;
        DispDecodeEntry de             ;
    };

    union DtePointer
    {
        const     DecodeEntry*  de;
        const DispDecodeEntry* dde;
    };

    static const RootDecodeEntry DECODE_TABLE   [];
    static const     DecodeEntry DECODE_TABLE_L2[];

    const CpuInstruction* command;

    CpuInstruction custom_cmd;
    DtePointer     dte       ;
    Handler        handler   ;

    Decoder           (const Decoder&); // Prohibited
    Decoder& operator=(const Decoder&); // Prohibited

    inline int      CommitCommand     (const uint32_t COMMAND                                                              )      ;
    inline int      CommitOperation   (                        const uint32_t LEFT__ARGUMENT, const uint32_t RIGHT_ARGUMENT)      ;
    inline int      CommitOperation   (const uint32_t COMMAND, const uint32_t LEFT__ARGUMENT, const uint32_t RIGHT_ARGUMENT)      ;
    inline int      CommitPredicateReg(                        const  uint8_t BASE          , const  uint8_t OPCODE        )      ;
    inline int      CommitPredicateReg(const uint32_t COMMAND, const  uint8_t BASE          , const  uint8_t OPCODE        )      ;
    inline uint32_t DirTail           (                                                       const  uint8_t OPCODE        ) const;

    inline int Continue   (       int    (Decoder::*next    )(const uint8_t)    );
    inline int Finished   (const  int               IMM_SIZE                    );
    inline int UndefinedOp(const uint8_t                                     = 0);

    uint32_t PackArgument(const uint8_t OPCODE, const uint8_t TYPE_A, const uint8_t TYPE_X);

    int Main           (const uint8_t OPCODE);
    // Root handlers
    int Fixed_Command  (const uint8_t OPCODE);
    int Mixed_Command  (const uint8_t OPCODE);
    int SnapInstruction(const uint8_t OPCODE);
    int TinyInstruction(const uint8_t OPCODE);
    // Main handlers: accumulators
    int Arg1Accumulator(const uint8_t OPCODE);
    int Arg2Accumulator(const uint8_t OPCODE);
    int ArgAAccumulator(const uint8_t OPCODE);
    // Main handlers: fixed command
    int Fx8CDispatcher (const uint8_t OPCODE);
    int Fx8EDispatcher (const uint8_t OPCODE);
    int FxDfDispatcher (const uint8_t OPCODE);
    int FxMLDispatcher (const uint8_t OPCODE);
    // Main handlers: mixed commands
    int MixedCommand   (const uint8_t OPCODE);
    int MultiCommand   (const uint8_t OPCODE);
    int MultiCommandUo (const uint8_t OPCODE);
    int MultiPairs     (const uint8_t OPCODE);
    int MultiPairsUo   (const uint8_t OPCODE);
    int Op8FDispatcher (const uint8_t OPCODE);
    int ShfMultiCommand(const uint8_t OPCODE);
    // Dispatchers
    int LeafI0         (const uint8_t OPCODE);
    int LeafI0Tail     (const uint8_t OPCODE);
    int LeafI1         (const uint8_t OPCODE);
    int LeafI1Tail     (const uint8_t OPCODE);
    int LeafI2         (const uint8_t OPCODE);
    int LeafI2Tail     (const uint8_t OPCODE);
    int LeafI3         (const uint8_t OPCODE);
    int LeafLR0        (const uint8_t OPCODE);
    int LeafLR1        (const uint8_t OPCODE);
    int LeafLR2        (const uint8_t OPCODE);
    int LeafLR3        (const uint8_t OPCODE);
    int LeafLr0        (const uint8_t OPCODE);
    int LeafLr1        (const uint8_t OPCODE);
    int LeafLr2        (const uint8_t OPCODE);
    int LeafLr3        (const uint8_t OPCODE);
    int LeafR0         (const uint8_t OPCODE);
    int LeafR0Tail     (const uint8_t OPCODE);
    int LeafR1         (const uint8_t OPCODE);
    int LeafR1Tail     (const uint8_t OPCODE);
    int LeafR2         (const uint8_t OPCODE);
    int LeafR2Tail     (const uint8_t OPCODE);

    int ArithmeticSeg3 (const uint8_t OPCODE);
    int ShiftImm3      (const uint8_t OPCODE);
    int Tail8F         (const uint8_t OPCODE);
    int TailD8         (const uint8_t OPCODE);
    int TailD9         (const uint8_t OPCODE);
    int TailDA         (const uint8_t OPCODE);
    int TailDB         (const uint8_t OPCODE);
    int TailDC         (const uint8_t OPCODE);
    int TailDD         (const uint8_t OPCODE);
    int TailDE         (const uint8_t OPCODE);
    int TailDF         (const uint8_t OPCODE);
    int TailFE         (const uint8_t OPCODE);
    int TailFF         (const uint8_t OPCODE);

    // Accumulators of arguments
    int AccumulateL010(const uint32_t LEFT__TYPE, const int8_t SIZE);
    int AccumulateL01X(const uint32_t LEFT__TYPE, const int8_t SIZE);
    int AccumulateLX0X(const uint32_t LEFT__TYPE                   );
    int AccumulateLX1X(const uint32_t LEFT__TYPE, const int8_t SIZE);
};

inline CpuInstruction::CpuInstruction(const uint32_t CMD, const uint32_t ARG_1, const uint32_t ARG_2, const uint32_t VALUE)
: command   (CMD       )
, first__arg(ARG_1     )
, second_arg(ARG_2     )
, third__arg(UINT32_MAX)
, first__val(VALUE     )
, second_val(0         )
{}

