/*
 * Copyright (c) 2016 Immo Software
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its contributors may
 *   be used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "sequence_reader.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

using namespace slab;

//------------------------------------------------------------------------------
// Code
//------------------------------------------------------------------------------

SequenceReader::SequenceReader()
{
}

void SequenceReader::init()
{
}

SequenceInfo * SequenceReader::parse(const char * data)
{
    SequenceInfo * info = new SequenceInfo;

    size_t n = strlen(data) + 1;
    char * buf = (char *)malloc(n);
    strncpy(buf, data, n);
    bool alldone = false;
    while (!alldone)
    {
        // Handle comment line.
        if (buf[0] == '#')
        {
            continue;
        }

        // Split into key and value.
        char * key = buf;
        char * value = buf;
        char * tmpbuf;
        bool done = false;
        while (!done)
        {
            char c = *buf++;
            switch (c)
            {
                case 0:
                case '\r':
                case '\n':
                    tmpbuf = buf - 1;
                    *tmpbuf = 0;
                    done = true;
                    if (c == 0)
                    {
                        alldone = true;
                    }
                    break;
                case '=':
                    tmpbuf = buf - 1;
                    *tmpbuf = 0;
                    value = buf;
                    break;
                default:
                    break;
            }
        }
        if (value == key)
        {
            printf("Invalid syntax\n");
            continue;
        }

        // Scan for sequence nunber.
        if (strcmp(key, "tempo") == 0)
        {
            int tempo = atoi(value);
            info->tempo = tempo;
        }
        else
        {
            int sequenceNumber = atoi(key);
            if (sequenceNumber >= 0 && sequenceNumber < kSequenceChannelCount)
            {
                int len = (int)strlen(value) + 1;
                char * seq = new char[len];
                memcpy(seq, value, len);
                info->channels[sequenceNumber] = seq;
            }
        }

//         printf("%s", buf);
    }

    return info;
}

//------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------
