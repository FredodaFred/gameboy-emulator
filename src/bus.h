#include <stdint.h>
#include <stdbool.h>

#define RAMSIZE (64*1024)

void bus_write(uint16_t addr, uint8_t data);
uint8_t bus_read(u_int16_t addr);