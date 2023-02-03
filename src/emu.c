#include "emu.h"

uint64_t clock_ticks = 0;
bool running = true;
bool halted = false;
bool stepping_l = false;

uint64_t tick(){
    clock_ticks++;
    return clock_ticks;
}

uint64_t get_clock_ticks(){ return clock_ticks; }

void* run_cpu(){
    while(running){  
        cpu_step();
        if(stepping_l){
            char c = getchar();
            if(c == 'c'){
                stepping_l = false;
            }
        }
    }
    return 0;
}
void emu_begin(bool stepping){
    printf("Emulation begun\n");
    // pthread_t cpu_thread;
    // if(pthread_create(&cpu_thread, NULL, run_cpu, NULL)){
    //     printf("Failed to create cpu thread. Exiting...\n");
    //     exit(-1);
    // }
    if(stepping){
        stepping_l = true;
    }
    run_cpu();
}