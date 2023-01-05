#pragma once

#include "common.h"

//read data from io
uint8_t io_read(uint16_t addr);

//write data to the io
void io_write(uint16_t addr, uint8_t data);