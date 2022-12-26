#include <stdint.h>
#include <stdbool.h>
#include "bus.h"
#include "instructions.h"

/* CPU UTIL */

/**
 * int    cycles: amount of cycles left to complete current instructon
 * bool stepping: defines if we are in step mode (complete instruction and stop) for debugging
 * bool   halted: lets us know if we have paused emulation
**/
typedef struct CPU_STATE {
  int cycles;
  bool stepping;
  bool halted;
  uint8_t currInstrOpcode;
  Instruction* currInstr;

} CPU_STATE;

/* Other external functions (async) */
void clock();
void reset();
void interr_req();
void non_maskable_interr_req();

/* Instruction handler funcs */

//load immediate data d16 to 16 bit register (reg1 + reg2)
void ld_r_16(reg reg1, reg reg2, uint16_t d16);

//load immediate data d8 to 8 bit register (reg1)
void ld_r_8(reg reg1, uint8_t d8);









