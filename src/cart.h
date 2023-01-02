#pragma once

#include "common.h"
typedef struct RomHeader{
    uint8_t entry[4];
    uint8_t logo[0x30];
    char title[16];
    uint16_t new_lic_code;
    uint8_t sgb_flag;
    uint8_t type;
    uint8_t rom_size;
    uint8_t ram_size;
    uint8_t dest_code;
    uint8_t lic_code;
    uint8_t version;
    uint8_t checksum;
    uint16_t global_checksum;
} RomHeader;

// typedef struct Rom{
//     uint8_t* rom_data;
// }Rom;

//Open Rom file and load data into memory
bool cart_load(char* file_name);

//free memory allocated for file
bool cart_close();

//read data from cart
uint8_t cart_read(uint16_t addr);

//write data to the cart
void cart_write(uint16_t addr, uint8_t data);

//get cart's size
int cart_size();