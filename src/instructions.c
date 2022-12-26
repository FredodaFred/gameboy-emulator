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
    [0x01] = {LD, R_D16, B, C, CT_NONE, 0, ld_r_16}
};

char *inst_name(instr_type t) { return instr_type_to_name[t]; }

Instruction* instr_lookup(uint8_t opcode) { return &instruction_lookup_table[opcode]; }
