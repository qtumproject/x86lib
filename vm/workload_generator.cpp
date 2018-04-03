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

// -------------------------------------------------- Workload Generator  for IA-32 decoder --------------------------------------------------
#include <cstring>
#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>

#include "Decoder.h"

using std::vector;

bool WriteBinaryFile(const char* const PATH, std::vector<unsigned char>& data)
{
    std::ofstream file(PATH, std::ios::binary);

    if (file.is_open())
    {
        std::copy(data.begin(), data.end(), std::ostream_iterator<unsigned char>(file));
    }

    return file.is_open();
}

class WorkLoadBuffer : public vector<unsigned char>
{
public:
    inline  WorkLoadBuffer(const bool H, const char* const P) : HUGE(H), PREFIX(P), id(0) {               }
    inline ~WorkLoadBuffer(      void                       )                             { commit(true); }

    void commit(const bool ENFORCED = false);

private:
    const bool        HUGE  ;
    const char* const PREFIX;

    int id;
};

void WorkLoadBuffer::commit(const bool ENFORCED)
{
    if (((size() >= 1 * 1024 * 1024) && (HUGE == false)) || (ENFORCED != false))
    {
        std::stringstream ss;

        ss << PREFIX;
        if (HUGE == false)
        {
            ss << id;
            id ++   ;
        }
        ss << ".bin";

        WriteBinaryFile(ss.str().c_str(), *this);
        clear          (                       );
    }
}

bool Process(const int STATUS, vector<unsigned char>& continuations, WorkLoadBuffer& workload)
{
    bool process = false;

    switch (STATUS)
    {
        case STATUS_CONTINUE      :
            continuations.push_back(0);

            process = true;
            break;
        case STATUS_INVALID_OPCODE:
            for (size_t i = 0; i < continuations.size(); i++)
            {
                workload.push_back(continuations[i]);
            }

            workload.commit();
            break;
        default:
            for (size_t i = 0; i < continuations.size() - STATUS; i++)
                workload.push_back(continuations[i]);

            if (STATUS != 0)
                for (int j = 0; j <= UCHAR_MAX; j++)
                {
                    for (size_t i = 0; i < continuations.size() - STATUS; i++)
                        workload.push_back(continuations[i]        );

                    workload.push_back(j);

                    for (int    i = 0; i < STATUS - 1                   ; i++)
                        workload.push_back(rand() % (UCHAR_MAX + 1));
                }

            workload.commit();
            break;
    }

    return process;
}

bool ProcessSmoke(const int STATUS, vector<unsigned char>& continuations, WorkLoadBuffer& workload)
{
    bool process = false;

    switch (STATUS)
    {
        case STATUS_CONTINUE      :
            continuations.push_back(0);

            process = true;
            break;
        case STATUS_INVALID_OPCODE:
            for (size_t i = 0; i < continuations.size(); i++)
                workload     .push_back(continuations[i]        );
            break;
        default:
            for (int    i = 0; i < STATUS              ; i++)
                continuations.pop_back (                        );
            for (size_t i = 0; i < continuations.size(); i++)
                workload     .push_back(continuations[i]        );
            for (int    i = 0; i < STATUS              ; i++)
                workload     .push_back(rand() % (UCHAR_MAX + 1));
            break;
    }

    if (process == false)
        workload.commit();

    return process;
}

int main(const int ARGC, const char* const ARGV[])
{
    bool huge  = false;
    bool smoke = false;

    for (int i = 1; i < ARGC - 1; i++)
    {
        if (strcmp(ARGV[i], "--smoke") == 0)
            smoke = true;
        if (strcmp(ARGV[i], "--huge" ) == 0)
            huge  = true;
    }
    {
        vector<unsigned char> continuations;
        WorkLoadBuffer        workload     (huge, ARGV[ARGC - 1]);

        continuations.push_back(0);

        for (bool process =  true; continuations.empty() == false ;)
            if (process)
            {
                Decoder decoder;

                for (size_t i = 0; i < continuations.size() - 1; i++)
                    decoder(continuations[i]);

                process = ((smoke) ? (ProcessSmoke) : (Process))(decoder(continuations[continuations.size() - 1]), continuations, workload);
            }
            else
            {
                if (continuations.back() == UCHAR_MAX)
                    continuations.pop_back()        ;
                else
                {
                    continuations.back    () ++     ;
                    process                  =  true;
                }
            }
    }

    return 0;
}
