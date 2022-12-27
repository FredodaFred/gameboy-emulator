#include <stdint.h>
#include <stdbool.h>
#include "bus.h"
#include "instructions.h"
#include "emu.h"

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

//------ LOAD FUNCTIONS ----- //

//load immediate data d16 to 16 bit register (reg1 + reg2)
void ld_r_16(reg reg1,  uint16_t d16);

//load immediate data d8 to 8 bit register (reg1)
void ld_r_8(reg reg1, uint8_t d8);

//load reg2 data to reg1
void ld_r_r(reg reg1, reg reg2);

//Load to the absolute address specified by the 16-bit register HL, data from the 8-bit register reg1. HL is incremented after write
void ld_hli_r(reg reg1);

//Load to the absolute address specified by the 16-bit register HL, data from the 8-bit register reg1. HL is decremented after write
void ld_hld_r(reg reg1);

//Load to the 8-bit reg1 register, data from the absolute address specified by the 16-bit register HL. The value of HL is incremented after the memory read
void ld_r_hli(reg reg1);

//Load to the 8-bit reg1 register, data from the absolute address specified by the 16-bit register HL. The value of HL is decremented after the memory read
void ld_r_hld(reg reg1);

//Load to the 8-bit reg1 register, data from the absolute address specified by 16bit reg2
void ld_r_mr(reg reg1, reg reg2);

//Load to the address specified by 16-bit reg1 register, data from 8bit reg2
void ld_mr_r(reg reg1, reg reg2);

//load d8 into memory specified by 16 reg1 register
void ld_mr_d8(reg reg1, uint8_t d8);

/* Test Helpers */
uint8_t get_a_reg();
uint8_t get_f_reg();
uint8_t get_b_reg();
uint8_t get_c_reg();
uint8_t get_d_reg();
uint8_t get_e_reg();
uint8_t get_h_reg();
uint8_t get_l_reg();
uint16_t   get_sp();
uint16_t   get_pc();