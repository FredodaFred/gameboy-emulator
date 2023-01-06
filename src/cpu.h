#pragma once

#include "common.h"
#include "bus.h"
#include "instructions.h"
#include "stack.h"
#include "emu.h"
#include "io.h"
#include "dbg.h"

/* CPU UTIL */

/**
 * int    cycles: amount of cycles left to complete current instructon
 * bool stepping: defines if we are in step mode (complete instruction and stop) for debugging
 * bool   halted: lets us know if we have paused emulation
 * uint8_t currInstrOpcode: Current Instruction's opcode
 * Instruction currInstr: Current Instruction w/ mapping data
**/
typedef struct CPU_STATE {
  int cycles;
  bool stepping;
  bool halted;
  uint8_t currInstrOpcode;
  Instruction* currInstr;
} CPU_STATE;

/* Interrupt handeling */

//For ie and if registers..
// Bit 0: VBlank   Interrupt Enable  (INT $40)  (1=Enable)
// Bit 1: LCD STAT Interrupt Enable  (INT $48)  (1=Enable)
// Bit 2: Timer    Interrupt Enable  (INT $50)  (1=Enable)
// Bit 3: Serial   Interrupt Enable  (INT $58)  (1=Enable)
// Bit 4: Joypad   Interrupt Enable  (INT $60)  (1=Enable)

typedef enum Interrupt{
    IT_VBLANK = 1,
    IT_LCD_STAT = 2,
    IT_TIMER = 4,
    IT_SERIAL = 8,
    IT_JOYPAD = 16
}Interrupt;

//request that the CPU interrupts given a type of interrupt
void request_interrupt(Interrupt i);
void handle_interrupt();

uint8_t get_ie_reg();
void set_ie_reg(uint8_t x);

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

//set content of stack pointer register
void   set_sp(uint16_t x);
void   set_pc(uint16_t x);



bool check_cond(cond_type cond);