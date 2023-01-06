#include "stack.h"
#include "cpu.h"
#include "bus.h"

void stkpush(uint8_t data){
    //decrement sp by 1
    set_sp(get_sp() - 1);
    bus_write(get_sp(), data);
}

void stkpush16(uint16_t data){
    stkpush( (data >> 8) & 0xFF );
    stkpush( data & 0xFF );
}

uint8_t stkpop(){
    set_sp(get_sp() + 1);
    return( bus_read(get_sp()));
}

uint16_t stkpop16(){
    uint16_t hi = stkpop();
    uint16_t lo = stkpop();
    return (hi << 8) | lo;
}