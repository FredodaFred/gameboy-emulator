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
    [0x01] = {LD, R_D16, BC, R_NONE, CT_NONE}, [0x02] = {LD, MR_R, BC, A, CT_NONE},   [0x06] = {LD, R_D8, B, R_NONE, CT_NONE}, [0x08] = {LD, A16_R, SP, R_NONE, CT_NONE}, [0x0A] = {LD, R_MR, A,  DE, CT_NONE}, [0x0E] = {LD, R_D8, C, R_NONE, CT_NONE},
    [0x11] = {LD, R_D16, DE, R_NONE, CT_NONE}, [0x12] = {LD, MR_R, DE, A, CT_NONE},   [0x16] = {LD, R_D8, D, R_NONE, CT_NONE},                                            [0x1A] = {LD, R_MR, A, DE, CT_NONE }, [0x1E] = {LD, R_D8, E, R_NONE, CT_NONE},
    [0x21] = {LD, R_D16, HL, R_NONE, CT_NONE}, [0x22]={LD, R_HLI, A, R_NONE, CT_NONE},[0x26] = {LD, R_D8, H, R_NONE, CT_NONE},                                            [0x2A] = {LD,R_HLI,A,R_NONE,CT_NONE}, [0x2E] = {LD, R_D8, E, R_NONE, CT_NONE},
    [0x31] = {LD, R_D16, SP, R_NONE, CT_NONE}, [0x32]={LD, R_HLD, A, R_NONE, CT_NONE},[0x36] = {LD, MR_D8,HL,R_NONE, CT_NONE},                                            [0x3A] = {LD,R_HLD,A,R_NONE,CT_NONE}, [0x2E] = {LD, R_D8, A, R_NONE, CT_NONE},

    [0x40] = {LD, R_R, B, B, R_NONE, CT_NONE}, [0x41] = {LD, R_R, B, C, R_NONE, CT_NONE},[0x42] = {LD, R_R, B, D, R_NONE, CT_NONE},[0x43] = {LD, R_R, B, E, R_NONE, CT_NONE},[0x44] = {LD, R_R, B, H, R_NONE, CT_NONE}, [0x45] = {LD, R_R, B, L, R_NONE, CT_NONE},
    [0x46] = {LD, R_MR, B, HL, R_NONE, CT_NONE},[0x47] = {LD, R_R, B, A, R_NONE, CT_NONE},[0x48] = {LD, R_R, C, B, R_NONE, CT_NONE}, [0x49] = {LD, R_R, C, C, R_NONE, CT_NONE}, [0x4A] = {LD, R_R, C, D, R_NONE, CT_NONE},[0x4B] = {LD, R_R, C, E, R_NONE, CT_NONE},
    [0x4C] = {LD, R_R, C, H, R_NONE, CT_NONE}, [0x4D] = {LD, R_R, C, L, R_NONE, CT_NONE}, [0x4E] = {LD, R_MR, C, HL, R_NONE, CT_NONE},
};

char *inst_name(instr_type t) { return instr_type_to_name[t]; }

Instruction* instr_lookup(uint8_t opcode) { return &instruction_lookup_table[opcode]; }
