; Test case for op_or_rm8_r8
; dependency opcodes:
;   MOV, CMP, JE, JP, JS, JZ, HLT, RET
CPU i386
BITS 32
ORG 0x1000 ;required because code is loaded at address 0x1000

start:

; ****************
; **** test 1 ****
; ****************

TEST1:

MOV AL, 0x16 ; 0001 0110
MOV BL, 0x89 ; 1000 1001
OR AL, BL ; expect: 1001 1111, i.e. 0x9f

PF_TEST1:
JP SF_TEST1 ; PF should be 1
HLT ;shouldn't reach here

SF_TEST1:
JS ZF_TEST1 ; SF should be 1
HLT ;shouldn't reach here

ZF_TEST1:
JNZ VAL_TEST1 ; ZF should be 0
HLT ;shouldn't reach here

VAL_TEST1:
CMP AL, 0x9f ; validate result
JE TEST2 ; TEST1 pass, jump to TEST2
HLT ;shouldn't reach here

; ****************
; **** test 2 ****
; ****************

TEST2:

MOV AL, 0x76 ; 0111 0110
MOV BL, 0x09 ; 0000 1001
OR AL, BL ; expect: 0111 1111, i.e. 0x7f

PF_TEST2:
JNP SF_TEST2 ; PF should be 0
HLT ;shouldn't reach here

SF_TEST2:
JNS ZF_TEST2 ; SF should be 0
HLT ;shouldn't reach here

ZF_TEST2:
JNZ VAL_TEST2 ; ZF should be 0
HLT ;shouldn't reach here

VAL_TEST2:
CMP AL, 0x7f ; validate result
JE PASS
HLT ;shouldn't reach here

PASS:
RET
