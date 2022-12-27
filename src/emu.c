#include "emu.h"

uint64_t clock_ticks = 0;
bool running = true;
bool halted = false;

uint64_t tick(){
    clock_ticks++;
    return clock_ticks;
}

uint64_t get_clock_ticks(){ return clock_ticks; }