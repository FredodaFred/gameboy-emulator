#pragma once

#include "common.h"

/* --- INSTRUCTIONS ---  */

// Addressing Modes
typedef enum addr_mode{
  IMP, R_D16, R_R, MR_R, R, R_D8, R_MR,
  R_HLI, R_HLD, HLI_R, HLD_R, R_A8, A8_R,
  HL_SPR, D16, D8, D16_R, MR_D8, MR, A16_R, R_A16,
} addr_mode;

// Types of Instructions
typedef enum instr_type{
    NONE, NOP, LD, INC, DEC, RLCA, ADD,
    RRCA, STOP, RLA, JR, RRA, DAA, CPL,
    SCF, CCF, HALT, ADC, SUB, SBC, AND,
    XOR, OR, CP, POP, JP, PUSH, RET, CB,
    CALL, RETI, LDH, JPHL, DI, EI, RST, ERR,
    //CB instructions...
    RLC, RRC, RL, RR, SLA, SRA, SWAP, SRL,
    BIT, RES, SET
} instr_type;

//Specifies which register
typedef enum reg{
  R_NONE, A, B, C, D, E, F, H, L, AF, BC, DE, HL, SP, PC
} reg;

typedef enum cond_type{
    CT_NONE, CT_NZ, CT_Z, CT_NC, CT_C
} cond_type;


/* Instruction Functions*/
// typedef void (*LD_R_16)(reg reg1, reg reg2, uint16_t d16);
// typedef void (*LD_R_8) (reg reg1, uint16_t d8);

// typedef union InstructionFunction{
//   LD_R_16 ld_r_16;
//   LD_R_8 ld_r_8;
// }InstructionFunction;

/**
 * instr_type type - specifies type of instruction
 * addr_mode mode - specifies the addressing mode
 * reg reg_1 - reg1
 * reg reg_2 - reg2
 * cond_type cond - assists in control flow/branching
 **/
typedef struct Instruction{
    instr_type type;
    addr_mode mode;
    reg reg_1;
    reg reg_2;
    cond_type cond;
    uint8_t param;
} Instruction;


/**
 * @Param instr_type t - takes enum type of instruction
 * @Return - char name of instruction
 **/
char *inst_name(instr_type t);

/**
 * @Param uint8_t opcode - opcode of instruction
 * @Return - data about instruction
 **/
Instruction* instr_lookup(uint8_t opcode);


//input enum reg type output its char symbol
char* reg_to_symbol(reg r);