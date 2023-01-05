#pragma once

#include "common.h"
#include "bus.h"
#include "instructions.h"
#include "stack.h"
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

/**
 * Load to the 8-bit reg1 register, data from the address specified by the 8-bit reg2 register. The full 16-bit absolute
 * address is obtained by setting the most significant byte to 0xFFand the least significant byte to the value of reg2,
 * so the possible range is 0xFF00-0xFFFF.
**/
void ldh_r_mr(reg reg1, reg reg2);

/**
* Load to the address specified by the 8-bit reg1 register, data from the 8-bit reg2 register. The full 16-bit absolute
* address is obtained by setting the most significant byte to 0xFFand the least significant byte to the value of reg1,
* so the possible range is 0xFF00-0xFFFF
**/
void ldh_mr_r(reg reg1, reg reg2);

// LDH r,(a8) has alternative mnemonic LD r,($FF00+a8)
// LDH (a8),r has alternative mnemonic LD ($FF00+a8),r

/**
 * Load to the address specified by the 8-bit immediate data a8, data from the 8-bit reg1 register. The full 16-bit
 * absolute address is obtained by setting the most significant byte to 0xFFand the least significant byte to the
 * value of a8, so the possible range is 0xFF00-0xFFF
 **/
void ld_r_a8(reg reg1, uint8_t a8);

/**
 * Load to the 8-bit reg1 register, data from the address specified by the 8-bit immediate data a8. The full 16-bit
 * absolute address is obtained by setting the most significant byte to 0xFFand the least significant byte to the
 * value of a8, so the possible range is 0xFF00-0xFFFF
 **/
void ld_a8_r(reg reg1, uint8_t a8);

//load from reg 1 to a16
void ld_a16_r(reg reg1, uint16_t a16);

//load to r from a16
void ld_r_a16(reg reg1, uint16_t a16);

void push(reg reg1);
void pop(reg reg1);
void call(reg reg1, addr_mode mode, cond_type cond);
void jump(reg reg1, addr_mode mode, cond_type cond);
void jumpr(cond_type cond);

//step the cpu
bool cpu_step();


/* Test Helpers */
uint8_t get_a_reg();
uint8_t get_f_reg();
uint8_t get_b_reg();
uint8_t get_c_reg();
uint8_t get_d_reg();
uint8_t get_e_reg();
uint8_t get_h_reg();
uint8_t get_l_reg();
uint8_t get_interrupt_reg();
//gets content of the stack pointer register
uint16_t   get_sp();
uint16_t   get_pc();

//retrieve reg value from enum type
uint16_t read_reg(reg regt);

//set reg value base on enum
void set_reg(reg regt, uint16_t val);

void set_a_reg(uint8_t x);
void set_f_reg(uint8_t x);
void set_b_reg(uint8_t x);
void set_c_reg(uint8_t x);
void set_d_reg(uint8_t x);
void set_e_reg(uint8_t x);
void set_h_reg(uint8_t x);
void set_l_reg(uint8_t x);
void set_interrupt_reg(uint8_t x);
//set content of stack pointer register
void   set_sp(uint16_t x);
void   set_pc(uint16_t x);

bool check_cond(cond_type cond);