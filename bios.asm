;Copyright (c) 2007 - 2009 Jordan "Earlz/hckr83" Earls  <http://www.Earlz.biz.tm>
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

BITS 32

org 0
mov al, 'x'
out 0, al
mov eax, 0xFFFF0004
mov ecx, 'foob'
mov [eax], ecx

CLD							;Clears direction flag, causes STOSW to increment
		CLI							;Disable interupts		
;jmp _test2
jmp word _test3
ud2
ud2
_test1:
hlt
ud2
ud2
_test2:
jmp word _test1

_test3:
;hlt

;For right now, this is just a test file

seperate_cpus:
;trap all processors but first
mov al,0x08
out 0xC1,al
out 0xC2,al
mov ax,0
mov ds,ax
mov [34],cs
mov [32],word int_8
mov [38],cs
mov [36],word int_9
mov [42],cs
mov [40],word int_a
mov [46],cs
mov [44],word int_b
mov [800],byte 0
sti
out 0xC0,al
;;All CPUs are now weeded out except for CPU #0.


bios_init:
mov sp,0x5000 ;not official...
mov ss,sp
mov sp,0x505 ;not sure if this is right..
mov ax,0
mov es,ax
;mov cx,0xFAF4
mov [es:0x10],word 0xFAF4
call load_os
call 0:0x7C00
mov al,1
out 0xF0,al
cli
hlt
;;;;END;;;;




















speed_test:
;Speed loop test
;On my older PC, this code runs at 14s in emulation, and .125s not emulated...
;emulated ran at 12.5s on my new dual-core 64bit pc..
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



;This will copy the file out of ROM and to 0:0x7C00
load_os:
push ax
push si
push di
mov ax,OS_file_end
sub ax,OS_file
mov cx,ax ;cx has our file length
;CX is correct
push cx
push es
push ds
mov ax,word 0
mov es,ax
mov ax,cs
mov ds,ax
mov di,0x7C00
mov si,OS_file
cld
rep movsb ;do it!
pop ds
pop es
pop cx
pop di
pop si
pop ax
ret






int_8:
;CPU #0!
lock add byte [0x800],byte 1

iret

int_9:
;CPU #1!
lock add byte [0x800],byte 1
mov ax,0
mov ds,ax
mov [0x700],byte 0
.check_loop:
cmp byte[0x700],byte 0
je .check_loop
;CPU #1 activated!
;Set SP and SS
mov ss,[0x702]
mov sp,[0x704]
iret


int_a:
lock add byte [0x800],byte 1
mov ax,0
mov ds,ax
mov [0x710],byte 0
.check_loop:
cmp byte[0x710],byte 0
je .check_loop
;CPU #1 activated!
;Set SP and SS
mov ss,[0x712]
mov sp,[0x714]

iret

int_b:
lock add byte [0x800],byte 1
mov ax,0
mov ds,ax
mov [0x720],byte 0
.check_loop:
cmp byte[0x720],byte 0
je .check_loop
;CPU #1 activated!
;Set SP and SS
mov ss,[0x722]
mov sp,[0x724]
iret

;How to get additional CPUs
;The total numbers of CPUs are in 0x800.
;In order to put additional cpus into action, you must do some things.
;the base address of each CPU (after CPU #0) is 0x700,0x710,0x720.
;first, the SS is from 0x02 and SP is from 0x04
;Note that an iret is done from this stack!
;and finally, mov 1 to 0x01 and it will put the cpu into action...






bah: db 0xD5,0xD6


OS_file: incbin 'test_os.bin'
OS_file_end:

test_string: db 0xD1,0xD2,0xD3,0xD4

call_address:
dw 0x7C00
dw 0
