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

// -------------------------------------------------- Simple IA-32 Disassembler Application --------------------------------------------------
#include <iostream>
#include <string>
#include <vector>

#include "Disassembler.h"

static const int STATUS_SUCCESS         =  0;
static const int STATUS_NOT_ENOUGH_ARGS = -1;
static const int STATUS_INVALID_PATH    = -2;

void DisassembleFile(FILE* file, std::vector<std::string>& instructions)
{
    size_t       current_offset = 0;
    Disassembler   disassembler;
    std::string   opcode_buffer;

    for (int code_byte = fgetc(file); code_byte != EOF; code_byte = fgetc(file))
    {
        const int STATUS = disassembler(code_byte);

        opcode_buffer.push_back(code_byte);

        current_offset++;

        if (STATUS != STATUS_CONTINUE)
        {
            char buffer[128];

            if (STATUS == STATUS_INVALID_OPCODE)
            {
                for (size_t i = 0; i < opcode_buffer.size(); i++)
                {
                    *disassembler.GetUndefined(buffer, opcode_buffer[i]) = '\0';

                    instructions.push_back(buffer);
                }
            }
            else
            {
                *disassembler.GetInstruction(buffer, current_offset) = '\0';

                instructions.push_back(buffer);
            }

            opcode_buffer.clear();
        }
    }
}

int main(const int ARGC, const char* const ARGV[])
{
    static const int ARG_FILE  = 1;
    static const int AGS_COUNT = 2;

    int status = STATUS_NOT_ENOUGH_ARGS;

    if (ARGC == AGS_COUNT)
    {
        FILE* file = fopen(ARGV[ARG_FILE], "rb");

        status = STATUS_INVALID_PATH;

        if (file != NULL)
        {
            std::vector<std::string> instructions;

            DisassembleFile(file, instructions);

            for (size_t i = 0; i < instructions.size(); i++)
                std::cout << instructions[i] << std::endl;

            fclose(file);

            status = STATUS_SUCCESS;
        }
    }

    return status;
}

