#include "instructions.h"
#include "cpu.h"

/* INSTRUCTIONS */

//Same order as enum, so we can get the char name of the instruction type
char* instr_type_to_name[] = {
    "<NONE>", "NOP", "LD", "INC", "DEC",
    "RLCA", "ADD", "RRCA", "STOP", "RLA",
    "JR", "RRA", "DAA", "CPL", "SCF", "CCF",
    "HALT", "ADC", "SUB", "SBC", "AND", "XOR",
    "OR", "CP", "POP", "JP", "PUSH", "RET",
    "CB", "CALL", "RETI", "LDH", "JPHL", "DI",
    "EI", "RST", "IN_ERR", "IN_RLC",  "IN_RRC",
    "IN_RL", "IN_RR", "IN_SLA", "IN_SRA", "IN_SWAP", 
    "IN_SRL", "IN_BIT", "IN_RES", "IN_SET"
};

//Instruction lookup table
Instruction instruction_lookup_table[0x100] = {
    [0x00] = {NOP, IMP},
    //LOAD INSTRUCTIONS
    [0x01] = {LD, R_D16, B, C, CT_NONE},
    [0x02] = {LD, 0, 0, 0, CT_NONE}, //TODO
    [0x06] = {LD, R_D8, B, R_NONE, CT_NONE},
    [0x08] = {LD, A16_R, SP, R_NONE, CT_NONE},
    [0x0E] = {LD, R_D8, C, R_NONE, CT_NONE},
    [0x11] = {LD, R_D16, D, E, CT_NONE }, //TODO
    [0x12] = {LD, 0, 0, 0, CT_NONE}, //TODO
    [0x16] = {LD, R_D8, D, R_NONE, CT_NONE},
    [0x1A] = {LD, 0, 0, 0, CT_NONE }, //TODO
    [0x1E] = {LD, R_D8, E, R_NONE, CT_NONE},
    [0x21] = {LD, R_D16, SP, R_NONE, CT_NONE},
    [0x22] = {LD, R_HLI, A, R_NONE, CT_NONE},
};

char *inst_name(instr_type t) { return instr_type_to_name[t]; }

Instruction* instr_lookup(uint8_t opcode) { return &instruction_lookup_table[opcode]; }
