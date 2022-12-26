#include "bus.h"

static uint8_t ram[RAMSIZE];

void ram_init(){
    for(int i = 0; i < RAMSIZE; ++i){
        ram[i] = 0;
    }
}

void bus_write(uint16_t addr, uint8_t data){
    //Guard full range
    if(addr <= 0x0000 || addr >= 0xFFFF){
        printf("Write to ram failed. Address out of range\n");
        return;
    }
    ram[addr] = data;
}

uint8_t bus_read(u_int16_t addr){
    if(addr <= 0x0000 || addr >= 0xFFFF){
        printf("Read to ram failed. Address out of range\n");
        return 0;
    }
    return ram[addr];
}