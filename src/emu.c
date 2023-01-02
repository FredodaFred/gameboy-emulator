#include "emu.h"

uint64_t clock_ticks = 0;
bool running = true;
bool halted = false;

uint64_t tick(){
    clock_ticks++;
    return clock_ticks;
}

uint64_t get_clock_ticks(){ return clock_ticks; }

void emu_begin(){
    printf("Emulation begun\n");
    int size = cart_size();
    // for(int i = 0x014F; i < 0x5000; i++){
    //     uint8_t byte = cart_read(i);
    //     printf("Byte: %x\n", byte);
    // }
    while(running){
        cpu_step();
        if(clock_ticks > 100){
            exit(-1);
        }
    }
}