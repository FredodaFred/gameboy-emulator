#include "bus.h"

// 0x0000 - 0x3FFF : ROM Bank 0
// 0x4000 - 0x7FFF : ROM Bank 1 - Switchable
// 0x8000 - 0x97FF : CHR RAM
// 0x9800 - 0x9BFF : BG Map 1
// 0x9C00 - 0x9FFF : BG Map 2
// 0xA000 - 0xBFFF : Cartridge RAM
// 0xC000 - 0xCFFF : RAM Bank 0
// 0xD000 - 0xDFFF : RAM Bank 1-7 - switchable - Color only
// 0xE000 - 0xFDFF : Reserved - Echo RAM
// 0xFE00 - 0xFE9F : Object Attribute Memory
// 0xFEA0 - 0xFEFF : Reserved - Unusable
// 0xFF00 - 0xFF7F : I/O Registers
// 0xFF80 - 0xFFFE : Zero Page

static uint8_t wram[WRAMSIZE];
static uint8_t hram[HRAMSIZE];

uint8_t wram_read(uint16_t addr){
    addr -= 0xC000;
    if(addr >= 0x2000){
        printf("Invalid wram address %08X\n", addr);
        exit(-1);
    }
    return wram[addr];
}

void wram_write(uint16_t addr, uint8_t data){
    addr -= 0xC000;
    if(addr >= 0x2000){
        printf("Invalid wram address %08X\n", addr);
        exit(-1);
    }
    wram[addr] = data;
}

uint8_t hram_read(uint16_t addr){
    addr -= 0xFF80;
    return hram[addr];
}

void hram_write(uint16_t addr, uint8_t data){
    addr -= 0xFF80;
    hram[addr] = data;
}

void bus_write(uint16_t addr, uint8_t data){
    if (addr < 0x8000) {
        //ROM Data
        cart_write(addr, data);
    } else if (addr < 0xA000) {
        //Char/Map Data
    } else if (addr < 0xC000) {
        //Cartridge RAM
        cart_write(addr, data);
    } else if (addr < 0xE000) {
        //WRAM (Working RAM)
        wram_write(addr, data);
    } else if (addr < 0xFE00) {
        //reserved echo ram...
        return;
    } else if (addr < 0xFEA0) {
        //OAM
        //TODO
        printf("UNSUPPORTED bus_read(%04X)\n", addr);
    } else if (addr < 0xFF00) {
        //reserved unusable...
        return;
    } else if (addr < 0xFF80) {
        io_write(addr, data);
        return;
    } else if (addr == 0xFFFF) {
        set_ie_reg(data);
        return;
    }
    return;
}

uint8_t bus_read(uint16_t addr){
    if (addr < 0x8000) {
        //ROM Data
        return cart_read(addr);
    } else if (addr < 0xA000) {
        //8 KiB Video RAM (VRAM)
    } else if (addr < 0xC000) {
        //8 KiB External RAM
        return cart_read(addr);
    } else if (addr < 0xE000) {
        //4 KiB Work RAM (WRAM)
        return wram_read(addr);
    } else if (addr < 0xFE00) {
        //Mirror of C000~DDFF (ECHO RAM)
        return 0;
    } else if (addr < 0xFEA0) {
        //Sprite attribute table (OAM)
        printf("UNSUPPORTED bus_read(%04X)\n", addr);
    } else if (addr < 0xFF00) {
        printf("Address unusable...\n");
        //exit(-1);
        return 0;
    } else if (addr < 0xFF80) {
        return io_read(addr);
    } else if (addr == 0xFFFF) {
        get_ie_reg();
        return 0;
    }
    return 0;
}