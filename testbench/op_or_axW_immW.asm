; Test case for op_or_rW_rmW
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

MOV EAX, 0x1616 ; 0001 0110 0001 0110
;[NUM1] 0x8989 ; 1000 1001 1000 1001
OR EAX, 0x8989; expect: 1001 1111 1001 1111, i.e. 0x9f9f

PF_TEST1:
JP SF_TEST1 ; PF should be 1
HLT ;shouldn't reach here

SF_TEST1:
JNS ZF_TEST1 ; SF should be 0
HLT ;shouldn't reach here

ZF_TEST1:
JNZ VAL_TEST1 ; ZF should be 0
HLT ;shouldn't reach here

VAL_TEST1:
CMP EAX, 0x9f9f ; validate result
JE TEST2 ; TEST1 pass, jump to TEST2
HLT ;shouldn't reach here

; ****************
; **** test 2 ****
; ****************

TEST2:

MOV EAX, 0x7674 ; 0111 0110 0111 0100
;[NUM2] 0x0909  0000 1001 0000 1001
OR EAX, 0x0909 ; expect: 0111 1111 0111 1101, i.e. 0x7f7d

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
CMP EAX, 0x7f7d ; validate result
JE PASS
HLT ;shouldn't reach here

PASS:
RET

