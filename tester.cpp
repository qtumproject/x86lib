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
#undef X86LIB_BUILD //so we don't need special makefile flags for this specific file.
#include <x86Lib.h>

using namespace std;
using namespace x86Lib;

uint32_t fileLength;

uint8_t *ptr_memory;
size_t size_memory;
void init_memory(char* fileToLoad){
	size_memory=0xFF000;
	ptr_memory=new uint8_t[size_memory];
	memset(ptr_memory,0x66,size_memory); //initialize it all to 0x66, an invalid opcode
	ifstream file(fileToLoad, ios::binary);
	if(!file){
		cout << "file " << fileToLoad << " does not exist" << endl;
		exit(1);
	}
	fileLength = file.tellg();
	file.seekg(0, std::ios::end);
	fileLength = (uint32_t) (file.tellg() - (long) fileLength);
	file.seekg(0, std::ios::beg);
	file.read((char*)&ptr_memory[0x0000],size_memory);
}
bool ROMLoaded = false;
class ROMemory : public MemoryDevice{
	uint8_t *ptr;
	uint32_t size;
	public:
	ROMemory(){
		if(ROMLoaded){
			throw new std::runtime_error("can't load two ROM memories");
		}
		ROMLoaded = true;
		ptr = ptr_memory;
		size = fileLength;
	}
	~ROMemory(){
		delete ptr;
	}
	virtual void Read(uint32_t address,int count,void *buffer){
		if(address + count > fileLength){
			throw new Mem_excp(address);
		}
		memcpy(buffer,&ptr_memory[address],count);
	}
	virtual void Write(uint32_t address,int count,void *buffer){
		throw new Mem_excp(address);
	}
};

class RAMemory : public MemoryDevice{
	uint8_t *ptr;
	uint32_t size;
	public:
	RAMemory(uint32_t size_){
		ptr = new uint8_t[size];
		size = size_;
	}
	~RAMemory(){
		free(ptr);
	}
	virtual void Read(uint32_t address,int count,void *buffer){
		memcpy(buffer,&ptr_memory[address],count);
	}
	virtual void Write(uint32_t address,int count,void *buffer){
		memcpy(&ptr_memory[address],buffer,count);
	}
};

class MapMemory : public MemoryDevice{
    bool range(uint32_t val, uint32_t min, uint32_t len){
        return val >= min && val < min+len;
    }
public:
    virtual void Read(uint32_t address,int count,void *buffer){
        //nothing yet
        memset(buffer, 0, count);
    }
    virtual void Write(uint32_t address,int count,void *buffer){
        //note memmap prefix is already subtracted out
        if(range(address, 0, 4)) {
            cout << hex << *((uint32_t *) buffer) << endl;
        }else if(range(address, 4, 8)){
            char tmp[5];
            tmp[4]=0;
            memcpy(tmp, buffer, 4);
            cout << tmp << endl;
        }else{
            cout << "unknown memmap" << endl;
        }
    }
};

MemorySystem Memory;

volatile bool int_cause;
volatile uint8_t int_cause_number=33;

void DumpMemory(){
	uint32_t i;
	FILE *fh;
	fh=fopen("mem_dump.bin","w");
	uint8_t *tmp = new uint8_t[0x100000];
	Memory.Read(0x100000, 0x100000 - 1, tmp);
	for(i=0;i<=0x100000;i++){
		fputc(tmp[i],fh);
	}
	fclose(fh);
}

void WritePort(uint16_t port,uint32_t val){
	/*Not going to try to emulate actual hardware...but rather just give us some useful
	functions to try out...*/
	switch(port){
		case 0: //print ascii char of val
		cout << (char) val << flush;
		break;
		case 1: //print value of byte
		cout << hex << (int)(uint8_t)val << flush;
		break;
		case 2: //print value of word
		cout << hex << (int)(uint16_t)val << flush;
		break;
		case 3: //print value of dword
		cout << hex << (int)(uint32_t)val << flush;
		break;
		case 4: //cause an interrupt
		int_cause=1;
		break;

		case 0xF0: //exit with val
		cout << "exiting with code " << val << endl;
		exit(val);
		break;
		case 0xF3: /*Dump memory to external file*/
		DumpMemory();
		break;

		case 0x30: //prints value
		putchar(val);
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
	}else if(size == 4){
		val=(uint32_t)*(uint32_t*)buffer;
	}else{
		throw;
	}
	WritePort(port,val);
}

class PCPorts : public PortDevice{
	public:
	~PCPorts(){}
	virtual void Read(uint16_t port,int size,void *buffer){
		port_read(NULL,port,size,buffer);
	}
	virtual void Write(uint16_t port,int size,void *buffer){
		port_write(NULL,port,size,buffer);
	}
};

PCPorts ports;

x86CPU *cpu;

void each_opcode(x86CPU *thiscpu){
	if(int_cause){
		int_cause=false;
		cpu->Int(int_cause_number);
	}
}

bool singleStep=false;

int main(int argc, char* argv[]){
	if(argc < 2){
		cout << "./x86test program.bin [-singlestep]" << endl;
		return 1;
	}
	if(argc > 2){
		if(strcmp(argv[2], "-singlestep") == 0){
			singleStep = true;
		}
	}

	init_memory(argv[1]);
	PortSystem Ports;
	ROMemory coderom;
	RAMemory config(0x1000);
	RAMemory scratch(0x100000);
	
	Memory.Add(0, 0xFFF, &config);
	Memory.Add(0x1000, 0xFFFFF, &coderom);
	Memory.Add(0x100000, 0x1FFFFF, &scratch);

	Ports.Add(0,0xFFFF,(PortDevice*)&ports);
	cpu=new x86CPU();
	cpu->Memory=&Memory;
	cpu->Ports=&Ports;
	cout << "Loaded! Beginning execution..." << endl;

	
	for(;;){
		try{
			if(!singleStep){
				cpu->Exec(1000);
				if(int_cause){
					int_cause=false;
					cpu->Int(int_cause_number);
				}
			}else{
				cpu->Exec(1);
				cout <<"OPCODE: " << cpu->GetLastOpcodeName() << "; hex: 0x" << hex << cpu->GetLastOpcode() << endl;
				cpu->DumpState(cout);
				cout << "-------------------------------" << endl;
				if(int_cause){
					int_cause=false;
					cpu->Int(int_cause_number);
				}
			}
		}
		catch(CpuPanic_excp err){
			cout << "CPU Panic!" <<endl;
			cout << "Message: " << err.desc << endl;
			cout << "Code: 0x" << hex << err.code << endl;
			cout <<"OPCODE: " << cpu->GetLastOpcodeName() << "; hex: 0x" << hex << cpu->GetLastOpcode() << endl;
			cpu->DumpState(cout);
			cout << endl;
			return 1;
		}
		catch(Default_excp err){
			cout << "!!Undefined Error!!" << endl;
			cout << "File: " << err.file << endl;
			cout << "Function: " << err.func << "()" <<endl;
			cout << "Line: " << err.line << endl;
			return 1;
		}
	}
	return 0;
}













