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
CPU i386
BITS 32

org 0
mov eax, log
out 1, eax

mov ebx, 0xF0000
mov ecx, [ebx]
mov ebx, logData
mov [ebx], ecx
out 1, eax

;result = ++counter + [0xF0000]
mov edx,[counter]
inc edx
mov [counter], edx
add edx, [0xF0000]
mov [result], edx

mov eax, abi_exit
out 0xFF, eax

;struct ABI_MakeLog{
;    uint16_t dataSize;
;    uint32_t dataAddress;
;    uint16_t topicCount;
;    uint32_t topicsAddress;
;}__attribute__((__packed__));
log:
dw 4
dd logData
dw 1
dd logTopics

logData:
dd 'xyc',0

logTopics:
times 32 db 10

abi_exit:
dd 4
dd result

result:
resd 0

counter:
resd 0