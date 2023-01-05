#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//https://stackoverflow.com/questions/7775991/how-to-get-hexdump-of-a-structure-data
// Usage:
//     hexDump(desc, addr, len, perLine);
//         desc:    if non-NULL, printed as a description before hex dump.
//         addr:    the address to start dumping from.
//         len:     the number of bytes to dump.
//         perLine: number of bytes on each output line.
void hexDump (const char * desc, const void * addr, const int len,int perLine);

//Given the flag register, return Z N H C in string represenation
char* flagBinaryString(uint8_t f);