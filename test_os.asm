;Copyright (c) 2007 - 2010 Jordan "Earlz/hckr83" Earls  <http://www.Earlz.biz.tm>
;All rights reserved.

;Redistribution and use in source and binary forms, with or without
;modification, are permitted provided that the following conditions
;are met:

;1. Redistributions of source code must retain the above copyright
;   notice, this list of conditions and the following disclaimer.
;2. Redistributions in binary form must reproduce the above copyright
;   notice, this list of conditions and the following disclaimer in the
;   documentation and/or other materials provided with the distribution.
;3. The name of the author may not be used to endorse or promote products
;   derived from this software without specific prior written permission.
   
;THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
;INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
;AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
;THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
;EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
;PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
;OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
;WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
;OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
;ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

;This file is part of the x86Lib project.


start:
org 0x7c00
call LoadInterrupts
cli


;call speed_test
;retf
call math_opcode_test
;call speed_test

cli
hlt


.success:
mov cx,0xF11F


cli
hlt


mov di,0
LoadInterrupts:
mov bx,div0_int
push es
mov es,di
mov [ES:di],bx
mov di,2
mov [ES:di],cs
pop es
ret




div0_int:

mov al,'D'
out 0,al
mov al,'0'
out 0,al
cli
hlt

iret



speed_test:
;Speed loop test
;On my older PC, this code runs at 14s in emulation, and .125s not emulated...
;emulated ran at 12.5s on my new dual-core 64bit pc..Actually emulated ran 9.8s on it with optimizations, but non-native code(they don't play nice)
;I need speed...
mov cx,0x100


.t2:
push cx
mov cx,0xFFFF
.t1:

push ax
mov ax,0xF00
sub ax,0x100

mov ax,0x200
mov ds,ax
pop ax
loop .t1
pop cx
;nop
loop .t2

ret




memread_speed_test:
mov cx,0xFF

mov ax,0x800
mov es,ax
mov ax,0
mov ds,ax
cld

.t2:
mov di,0
push cx
mov cx,0xFFFF
mov si,0

rep movsb

mov ax,ds

add ax,0x100
mov ds,ax
mov di,0
pop cx
loop .t2

ret





opcode_test_set1:
;This will test all cmp, sub, and add opcodes, assuming only
;that all mov opcodes work, and that cmp al,imm8 and cmp ax,imm16 work..

mov ax,0x200
mov bx,0x200
sub ax,bx
cmp ax,0
jne .fail
mov bx,0x230
sub bx,0x30
mov ax,bx
cmp ax,0x200
jne .fail
mov bx,buffer
mov ax,cs
mov ds,ax
mov [bx],word 0x200
sub [bx],word 0x100
mov ax,[bx]
cmp ax,0x100
jne .fail




ret

.fail:
mov al,1
out 0xF0,al
ret






math_opcode_test:
mov al,'T'
out 0,al
mov cx,0
call .save_reg
call .t0
call .restore_reg
jne .error
call .print_good

inc cx
call .save_reg
call .t1
call .restore_reg
jne .error
call .print_good

inc cx
call .save_reg
call .t2
call .restore_reg
jne .error
call .print_good

inc cx
call .save_reg
call .t3
call .restore_reg
jne .error
call .print_good

jmp .skip

.error:
call .save_reg
mov al,'E'
out 0,al
mov al,' '
out 0,al
mov ax,cx
out 1,ax
call .restore_reg
ret

.skip:

inc cx
call .save_reg
call .t4
call .restore_reg
jne .error
call .print_good
inc cx
call .save_reg
call .t5
call .restore_reg
jne .error
call .print_good

inc cx
call .save_reg
call .t6
call .restore_reg
jne .error
call .print_good

inc cx
call .save_reg
call .t7
call .restore_reg
jne .error
call .print_good
inc cx
call .save_reg
call .t8
call .restore_reg
jne .error
call .print_good

inc cx
call .save_reg
call .t9
call .restore_reg
jne .error
call .print_good
inc cx
call .save_reg
call .t10
call .restore_reg
jne .error
call .print_good

inc cx
call .save_reg
call .t11
call .restore_reg
jne .error2
call .print_good
inc cx
call .save_reg
call .t12
call .restore_reg
jne .error2
call .print_good

inc cx
call .save_reg
call .t13
call .restore_reg
jne .error2
call .print_good


jmp .skip2

.error2:
jmp .error


.skip2:

ret

.print_good:
call .save_reg
mov al,'G'
out 0,al
mov al,' '
out 0,al
mov ax,cx
out 1,ax
call .restore_reg
ret


.t0: ;16bit and
mov bx,0xF0F0
mov cx,0x00
and bx,cx
cmp bx,0x0
ret

.t1: ;8bit or
mov bl,0xF0
or bl,0xF
cmp bl,0xFF
ret

.t2: ;cmpsb
mov ax,0
mov es,ax
mov si,0x900
mov [si],byte 'a'
inc si
mov [si],byte 'b'
inc si
mov [si],byte 0
mov cx,3
std
mov di,0x1000
mov [di],byte 'a'
inc di
mov [di],byte 'b'
inc di
mov [di],byte 'a'
repne cmpsb
cmp di,0x1000
ret

.t3: ;cmpsw
mov ax,0
mov es,ax
mov si,0x900
mov di,0x1000
mov [si],word 0xF00F
mov [di],word 0xF00F
add si,2
add di,2
mov [si],word 0x1002
mov [di],word 0x1002
add si,2
add di,2
mov [si],word 0x1000
mov [di],word 0x1001
mov cx,3
repne cmpsw
cmp di,0x1000
ret

.tmp_srcptr: dw 0xC00F,0x11F1 ;should be offset, then segment
.t4: ;lds --reg
push ds
mov ax,cs
mov es,ax
mov bx,.tmp_srcptr
lds di,[ES:bx]
cmp di,0xC00F
pop ds
ret

.t5: ;les --ES
push es
les di,[.tmp_srcptr]
mov ax,es
cmp ax,0x11F1
pop es
ret

.t6: ;lea
lea di,[.tmp_srcptr]
cmp di,word .tmp_srcptr
ret

.t7: ;jneg 8bit
mov al,-50
neg al
cmp al,50
ret

.t8: ;neg 16bit
mov ax,900
neg ax
cmp ax,-900
ret

.t9: ;xor8bit
mov al,0xF0
xor al,0xFF
cmp al,0x0F
ret

.t10: ;xor 16bit
mov ax,0xF00F
xor ax,0xFFFF
cmp ax,0x0FF0
ret

.t11: ;xchg first
mov bx,0x800
mov cx,0x90F
xchg bx,cx
cmp bx,0x90F
ret

.t12: ;xchg second
mov bx,0x800
mov cx,0x90F
xchg bx,cx
cmp cx,0x800
ret

.t13: ;shift tests...
mov bx,0
mov bl,0x32
mov cl,5
stc
rcr bl,cl
adc bl,0
cmp bl,0x2A
mov bp,bx
ret



.save_reg:
pop si
push ax
push bx
push cx
push dx
push si
ret

.restore_reg:
pop si
pop dx
pop cx
pop bx
pop ax
push si ;save and restore return address
ret











buffer:
db 4




















