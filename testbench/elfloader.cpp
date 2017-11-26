#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "elf.h"

using namespace std;


bool validateElf(Elf32_Ehdr* hdr){
    //check if valid ELF file
    if(hdr->e_ident[EI_MAG0] != ELFMAG0) {
        return false;
    }
    if(hdr->e_ident[EI_MAG1] != ELFMAG1) {
        return false;
    }
    if(hdr->e_ident[EI_MAG2] != ELFMAG2) {
        return false;
    }
    if(hdr->e_ident[EI_MAG3] != ELFMAG3) {
        return false;
    }
    //now check if valid to be loaded into Qtum's x86 VM
    //if(!elf_check_file(hdr)) {
    //    return false;
    //}
    if(hdr->e_ident[EI_CLASS] != ELFCLASS32) {
        return false;
    }
    if(hdr->e_ident[EI_DATA] != ELFDATA2LSB) {
        return false;
    }
    if(hdr->e_machine != EM_386) {
        return false;
    }
    if(hdr->e_ident[EI_VERSION] != EV_CURRENT) {
        return false;
    }
    if(hdr->e_type != ET_EXEC) {
        return false;
    }
    //qtum rules:
    if(hdr->e_entry != CODE_ADDRESS){
        return false;
    }
    return true;
}



//code is memory buffer available for readonly data
//data is memory buffer available for readwrite data
//code is 0x1000, data is 0x100000
bool loadElf(char* code, size_t* codeSize, char* data, char* raw, size_t size){
    if(size < sizeof(Elf32_Ehdr)){
        return false;
    }

    Elf32_Ehdr *hdr = (Elf32_Ehdr*) raw;
    if(!validateElf(hdr)){
        return false;
    }

    bool codeProcessed;
    for(int i=1;i<MAX_SECTIONS;i++){ //1st section is undefined
        if(hdr->e_shoff + (hdr->e_shentsize * i) + sizeof(Elf32_Shdr) < size){
            return false;
        }
        Elf32_Shdr *section = (Elf32_Shdr*) &raw[hdr->e_shoff + (hdr->e_shentsize * i)];

        if(section->sh_type == SHT_PROGBITS && 
            (section->sh_flags & SHF_EXECINSTR && section->sh_flags & SHF_ALLOC)){
            //.text and readonly bits
            if(section->sh_addr != CODE_ADDRESS || codeProcessed){
                //Only allow 1 section to load into code address
                return false;
            }
            if(section->sh_addr + section->sh_size > CODE_ADDRESS + MAX_CODE_SIZE){
                //error if trying to load code outside of code area
                return false;
            }
            if(section->sh_offset + section->sh_size > size){
                return false;
            }
            memcpy(code, &raw[section->sh_offset], section->sh_size);
            *codeSize = section->sh_size;
            codeProcessed=true;
        }else if(section->sh_type == SHT_PROGBITS && 
            (section->sh_flags & SHF_WRITE && section->sh_flags & SHF_ALLOC)){
            //.data and readwrite bits
            if(section->sh_addr < DATA_ADDRESS || 
                section->sh_addr + section->sh_size > DATA_ADDRESS + MAX_DATA_SIZE){
                //error if trying to load data outside of data area
                //TODO: we could allow for this to do allocation or something.. 
                return false;
            }
            if(section->sh_offset + section->sh_size > size){
                return false;
            }
            memcpy(&data[section->sh_addr], &raw[section->sh_offset], section->sh_size);
        }else if(section->sh_type == SHT_NOBITS && 
            (section->sh_flags & SHF_WRITE && section->sh_flags & SHF_ALLOC)){
            //bss.. 
            if(section->sh_addr < DATA_ADDRESS || 
                section->sh_addr + section->sh_size > DATA_ADDRESS + MAX_DATA_SIZE){
                //Even though we load no data, if this is triggered then .bss is 
                //located in an area of memory that is not available, so error.
                return false;
            }
            //anything to do here?
        }else{
            //ignore unknown sections
        }
    }
    return true;
}


