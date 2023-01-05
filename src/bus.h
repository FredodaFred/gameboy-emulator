#pragma once

#include "common.h"
#include "cart.h"
#include "cpu.h"
#include "io.h"

#define WRAMSIZE 0x2000
#define HRAMSIZE 0x80

void bus_write(uint16_t addr, uint8_t data);
uint8_t bus_read(uint16_t addr);

//read from working ram
uint8_t wram_read(uint16_t addr);

//write to working ram
void wram_write(uint16_t addr, uint8_t data);

uint8_t hram_read(uint16_t addr);

void hram_write(uint16_t addr, uint8_t data);