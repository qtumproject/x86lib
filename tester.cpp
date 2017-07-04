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
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <stdlib.h>
//#include <conio.h>
#undef X86LIB_BUILD //so we don't need special makefile flags for this specific file.
#include <x86Lib.h>

using namespace std;
using namespace x86Lib;

static const uint32_t ROM_START=0xC0000;
uint8_t *ptr_memory;
size_t size_memory;
void init_memory(){
	size_memory=0xFFFFF;
	ptr_memory=new uint8_t[size_memory];
	memset(ptr_memory,0x66,size_memory); //initialize it all to 0x66, an invalid opcode
	//Load bios...
	ifstream bios("bios.bin",ios::binary); //open it readonly binary
	bios.read((char*)&ptr_memory[0xF0000],0xFFFF);
	int i;
	for(i=0;i<=0xF;i++){
		ptr_memory[0xFFFF0+i]=0x90;
	}
}

/*Yea yea. hackjob, I know.. but I want this to work!**/
class PCMemory : MemoryDevice{
	uint8_t *ptr;
	uint32_t size;
	public:
	PCMemory(){
		init_memory();
	}
	virtual void Read(uint32_t address,int count,void *buffer){
		memcpy(buffer,&ptr_memory[address],count);
	}
	virtual void Write(uint32_t address,int count,void *buffer){
		memcpy(&ptr_memory[address],buffer,count);
	}
};


volatile bool int_cause;
volatile uint8_t int_cause_number=0;





void DumpMemory(){
	uint32_t i;
	FILE *fh;
	fh=fopen("mem_dump.bin","w");
	for(i=0;i<=0xFFFFF;i++){
		//fputc(memory.ReadByte(i),fh);
	}
	fclose(fh);
}






void WritePort(uint16_t port,uint32_t val){
	/*Not going to try to emulate actual hardware...but rather just give us some useful
	functions to try out...*/
	static uint8_t interrupt_start=0;
	static uint8_t counter=0;
	switch(port){
		case 0: //print asci char of val
		cout << (char) val << flush;
		break;
		case 1: //print value of byte
		cout << hex << (int)(uint8_t)val;
		break;
		case 2: //print value of word
		cout << hex << (int)(uint16_t)val;
		break;
		case 3: //print value of dword
		cout << hex << (int)(uint32_t)val;
		break;
		case 4: //cause an interrupt
		int_cause=1;
		break;

		case 0xF0: //exit with val
		exit(val);
		break;
		case 0xF1: //dump small bit of memory
		cout << "debug info...Dumping 0:0-to-0:200" << endl;
		/*
		cout << "0: "<< hex << memory.ReadDword(0) << endl;
		cout << "4: "<< hex << memory.ReadDword(4) << endl;
		cout << "8: "<< hex << memory.ReadDword(8) << endl;
		cout << "B: "<< hex << memory.ReadDword(12) << endl;
		cout << "10: "<< hex << memory.ReadDword(16) << endl;
		cout << "14: "<< hex << memory.ReadDword(20) << endl;
		cout << "18: "<< hex << memory.ReadDword(24) << endl;
		cout << "1B: "<< hex << memory.ReadDword(28) << endl;
		cout << "20: "<< hex << memory.ReadDword(32) << endl;*/
		break;
		case 0xF2: //a two word command; first is segment, second is offset; prints value of memory there
		static uint16_t seg_temp;
		static bool flip_flop=0;
		if(flip_flop==1){
			flip_flop=0;
			//cout <<hex << memory.ReadDword((seg_temp<<4)|val) << endl;
		}else{
			flip_flop=1;
			seg_temp=val;
		}
		break;
		case 0xF3: /*Dump memory to external file*/
		DumpMemory();
		break;
		case 0xF20F: //clear flip-flop of F2
		flip_flop=0;
		break;

		case 0x30: //prints value

		putchar(val);
		break;

		case 0xC0: //This will cause an interrupt of the number of the counter.(on only the CURRENT cpu)
		int_cause_number=interrupt_start+counter;
		int_cause=1;
		counter++;
		break;
		case 0xC1: //This will change the base interrupt number
		interrupt_start=val;
		break;
		case 0xC2: //This will reset the counter to 0
		counter=0;
		break;


		default:
		cout << "undefined port" << endl;
		break;
	}


}

uint32_t ReadPort(uint16_t port){
	switch(port){
		case 0x30:
		uint8_t tmp;
		cin.read((char*)&tmp,1);
		return tmp;
		break;

		default:
		cout << "undefined port" << endl;
		return 0;
		break;
	}
}

void port_read(x86CPU *thiscpu,uint16_t port,int size,void *buffer){
	uint32_t val;
	val=ReadPort(port);
	if(size==1){
		*(uint8_t*)buffer=val;
	}else if(size==2){
		*(uint16_t*)buffer=val;
	}else{
		throw;
	}
}
void port_write(x86CPU *thiscpu,uint16_t port,int size,void *buffer){
	uint32_t val;
	if(size==1){
		val=*(uint8_t*)buffer;
	}else if(size==2){
		val=(uint16_t)*(uint16_t*)buffer;
	}else{
		throw;
	}
	WritePort(port,val);
}

class PCPorts : PortDevice{
	public:
	~PCPorts(){}
	void Read(uint16_t port,int size,void *buffer){
		port_read(NULL,port,size,buffer);
	}
	void Write(uint16_t port,int size,void *buffer){
		port_write(NULL,port,size,buffer);
	}
};

PCMemory memory;
PCPorts ports;

x86CPU **cpu_ctrl;

void each_opcode(x86CPU *thiscpu){
	if(int_cause){
		int_cause=false;
		thiscpu->Int(int_cause_number);
	}
}


//typedef unsigned uint128_t __attribute__((mode(TI)));
int main(){
	//cout << sizeof(uint128_t)<<endl;
	//test_newsystem();
	MemorySystem Memory;
	PortSystem Ports;
	init_memory();
	uint8_t cpu_i=0;
	static const uint8_t cpu_number=1;
	cpu_ctrl=new x86CPU *[cpu_number];
	Memory.Add(0,0xFFFFF,(MemoryDevice*)&memory);
	printf("!");
	fflush(stdout);
	Ports.Add(0,0xFFFF,(PortDevice*)&ports);
	printf("!");
	fflush(stdout);
	for(cpu_i=0;cpu_i<cpu_number;cpu_i++){
		cpu_ctrl[cpu_i]=new x86CPU();
		cpu_ctrl[cpu_i]->Memory=(MemorySystem*)&Memory;
		cpu_ctrl[cpu_i]->Ports=(PortSystem*)&Ports;
#ifdef ENABLE_OPCODE_CALLBACK
		cpu_ctrl[cpu_i]->EachOpcodeCallback=&each_opcode;
#endif
		printf("!");
		fflush(stdout);
	}
	cpu_i=0;
	cout << "constructed.."<<endl;

	
	for(;;){
		if(cpu_i==cpu_number){
			cpu_i=0;
		}
		try{
			cpu_ctrl[cpu_i]->Exec(500000);
			//cpu2->Cycle();
			if(int_cause){
				int_cause=false;
				cpu_ctrl[cpu_i]->Int(int_cause_number);
			}
		}
		catch(CpuPanic_excp err){
			cout << "CPU Panic! CPU Number=" << (int)cpu_i <<endl;
			cout << "Message: " << err.desc << endl;
			cout << "Code: 0x" << hex << err.code << endl;
			//cout << "Opcode: 0x" << hex << (int)op_cache[0] << endl;
			for(cpu_i=0;cpu_i<cpu_number;cpu_i++){
				cout << "CPU #" << (int)cpu_i << endl;
				cpu_ctrl[cpu_i]->DumpState(cout);
				cout << endl;
			}
			return 0;
			for(;;){}
		}
		catch(Default_excp err){
			cout << "!!Undefined Error!!" << endl;
			cout << "File: " << err.file << endl;
			cout << "Function: " << err.func << "()" <<endl;
			cout << "Line: " << err.line << endl;
			return 0;
			for(;;){}
		}
		cpu_i++;
	}






}












