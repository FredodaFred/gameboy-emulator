#pragma once

#include "common.h"

//push 1byte to stack
void stkpush(uint8_t data);
//push 2byte to stack
void stkpush16(uint16_t data);
//pop 1byte from stack
uint8_t stkpop();
//pop 2byte from stack
uint16_t stkpop16();