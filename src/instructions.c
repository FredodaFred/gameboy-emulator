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

    //LOAD INSTRUCTIONS (AND LDH)
    [0x01] = {LD, R_D16, BC, R_NONE, CT_NONE}, [0x02] = {LD, MR_R, BC, A, CT_NONE},   [0x06] = {LD, R_D8, B, R_NONE, CT_NONE}, [0x08] = {LD, A16_R, SP, R_NONE, CT_NONE}, [0x0A] = {LD, R_MR, A,  DE, CT_NONE}, [0x0E] = {LD, R_D8, C, R_NONE, CT_NONE},
    [0x11] = {LD, R_D16, DE, R_NONE, CT_NONE}, [0x12] = {LD, MR_R, DE, A, CT_NONE},   [0x16] = {LD, R_D8, D, R_NONE, CT_NONE},                                            [0x1A] = {LD, R_MR, A, DE, CT_NONE }, [0x1E] = {LD, R_D8, E, R_NONE, CT_NONE},
    [0x21] = {LD, R_D16, HL, R_NONE, CT_NONE}, [0x22]={LD, R_HLI, A, R_NONE, CT_NONE},[0x26] = {LD, R_D8, H, R_NONE, CT_NONE},                                            [0x2A] = {LD,R_HLI,A,R_NONE,CT_NONE}, [0x2E] = {LD, R_D8, E, R_NONE, CT_NONE},
    [0x31] = {LD, R_D16, SP, R_NONE, CT_NONE}, [0x32]={LD, R_HLD, A, R_NONE, CT_NONE},[0x36] = {LD, MR_D8,HL,R_NONE, CT_NONE},                                            [0x3A] = {LD,R_HLD,A,R_NONE,CT_NONE}, [0x3E] = {LD, R_D8, A, R_NONE, CT_NONE},

    [0x40] = {LD, R_R, B, B, CT_NONE},  [0x41] = {LD, R_R, B, C, CT_NONE},[0x42] = {LD, R_R, B, D, CT_NONE},[0x43] = {LD, R_R, B, E, CT_NONE},[0x44] = {LD, R_R, B, H, CT_NONE}, [0x45] = {LD, R_R, B, L, CT_NONE},
    [0x46] = {LD, R_MR, B, HL, CT_NONE},[0x47] = {LD, R_R, B, A, CT_NONE},[0x48] = {LD, R_R, C, B, CT_NONE}, [0x49] = {LD, R_R, C, C, CT_NONE}, [0x4A] = {LD, R_R, C, D, CT_NONE},[0x4B] = {LD, R_R, C, E, CT_NONE},
    [0x4C] = {LD, R_R, C, H, CT_NONE},  [0x4D] = {LD, R_R, C, L, CT_NONE}, [0x4E] = {LD, R_MR, C, HL, CT_NONE}, [0x4F] = {LD, R_R, C, A, CT_NONE},

    [0x50] = {LD, R_R, D, B, CT_NONE}, [0x51] = {LD, R_R, D, C, CT_NONE},[0x52] = {LD, R_R, D, D, CT_NONE},[0x53] = {LD, R_R, D, E, CT_NONE},[0x54] = {LD, R_R, D, H, CT_NONE}, [0x55] = {LD, R_R, D, L, CT_NONE},
    [0x56] = {LD, R_MR,D, HL, CT_NONE},[0x57] = {LD, R_R, D, A, CT_NONE},[0x58] = {LD, R_R, E, B, CT_NONE}, [0x59] = {LD, R_R, E, C, CT_NONE}, [0x5A] = {LD, R_R, E, D, CT_NONE},[0x5B] = {LD, R_R, E, E, CT_NONE},
    [0x5C] = {LD, R_R, E, H, CT_NONE}, [0x5D] = {LD, R_R, E, L, CT_NONE}, [0x5E] = {LD, R_MR, E, HL, CT_NONE}, [0x5F] = {LD, R_R, E, A, CT_NONE},   

    [0x60] = {LD, R_R, H, B, CT_NONE},  [0x61] = {LD, R_R, H, C, CT_NONE},[0x62] = {LD, R_R, H, D, CT_NONE},[0x63] = {LD, R_R, H, E, CT_NONE},[0x64] = {LD, R_R, H, H, CT_NONE}, [0x65] = {LD, R_R, H, L, CT_NONE},
    [0x66] = {LD, R_MR, H, HL, CT_NONE},[0x67] = {LD, R_R, H, A, CT_NONE},[0x68] = {LD, R_R, L, B, CT_NONE}, [0x69] = {LD, R_R, L, C, CT_NONE}, [0x6A] = {LD, R_R, L, D, CT_NONE},[0x6B] = {LD, R_R, L, E, CT_NONE},
    [0x6C] = {LD, R_R, L, H, CT_NONE},  [0x6D] = {LD, R_R, L, L, CT_NONE}, [0x6E] = {LD, R_MR, L, HL, CT_NONE}, [0x6F] = {LD, R_R, L, A, CT_NONE},     

    [0x70] = {LD, MR_R, HL, B, CT_NONE}, [0x71] = {LD, MR_R, HL, C, CT_NONE},[0x72] = {LD, MR_R, HL, D, CT_NONE},[0x73] = {LD, MR_R, HL, E, CT_NONE},[0x74] = {LD, MR_R, HL, H, CT_NONE}, [0x75] = {LD, MR_R, HL, L, CT_NONE},
    [0x77] = {LD, MR_R, HL, A, CT_NONE}, [0x78] = {LD, R_R, A, B, CT_NONE}, [0x79] = {LD, R_R, A, C, CT_NONE}, [0x7A] = {LD, R_R, A, D, CT_NONE},[0x7B] = {LD, R_R, A, E, CT_NONE},
    [0x7C] = {LD, R_R, A, H, CT_NONE},   [0x7D] = {LD, R_R, A, L, CT_NONE}, [0x7E] = {LD, R_MR, A, HL, CT_NONE}, [0x7F] = {LD, R_R, A, A, CT_NONE},   
                                               //LD C,($FF00+A)
    [0xE0] = {LDH, A8_R, A, R_NONE, CT_NONE}, [0xE2] = {LD, MR_R, C, A, CT_NONE}, [0xEA] = {LDH, A16_R, A, R_NONE, CT_NONE},  
                                               //LD ($FF00+A),C
    [0xF0] = {LDH, R_A8, A, R_NONE, CT_NZ},   [0xF2] = {LD, R_MR, A, C, CT_NONE}, [0xF8] = {LD, HL_SPR, HL, SP, CT_NONE}, [0xF9] = {LD, R_R, SP, HL, CT_NONE}, [0xFA] = {LD, R_A16, A, R_NONE, CT_NONE},
    
    //POP INSTRUCTIONS
    [0xC1] = {POP, R, BC, R_NONE, CT_NONE}, [0xD1] = {POP, R, DE, R_NONE, CT_NONE}, [0xE1] = {POP, R, HL, R_NONE, CT_NONE}, [0xF1] = {POP, R, AF, R_NONE, CT_NONE},
    //PUSH INSTRUCTIONS
    [0xC5] = {PUSH, R, BC, R_NONE, CT_NONE}, [0xD5]= {PUSH, R, DE, R_NONE, CT_NONE}, [0xE5]= {PUSH, R, HL, R_NONE, CT_NONE},  [0xF5]= {PUSH, R, AF, R_NONE, CT_NONE},
    //JUMP INSTRUCTIONS
    [0xC2] = {JP, D16, R_NONE, R_NONE, CT_NZ}, [0xC3] ={JP, D16, R_NONE, R_NONE, CT_NONE}, [0xCA] = {JP, D16, R_NONE, R_NONE, CT_Z}, [0xD2] = {JP, D16, R_NONE, R_NONE, CT_NC}, [0xDA] = {JP, D16, R_NONE, R_NONE, CT_C}, [0xE9] = {JP, MR, HL, R_NONE, CT_NONE}, 
    //CALL INSTRUCTIONS
    [0xC4] = {CALL, D16, R_NONE, R_NONE, CT_NZ}, [0xCC] = {CALL, D16, R_NONE, R_NONE, CT_Z}, [0xCD] = {CALL, D16, R_NONE, R_NONE, CT_NONE}, [0xD4] = {CALL, D16, R_NONE, R_NONE, CT_NC}, [0xDC] = {CALL, D16, R_NONE, R_NONE, CT_C},
    //JUMP RETURN
    [0x18] ={JR, R_D8, R_NONE, R_NONE, CT_NONE}, [0x20] = {JR, R_D8, R_NONE, R_NONE, CT_NZ}, [0x28] = {JR, R_D8, R_NONE, R_NONE, CT_Z}, [0x30] = {JR, R_D8, R_NONE, R_NONE, CT_NC}, [0x38] = {JR, R_D8, R_NONE, R_NONE, CT_C},
    //RET 
    [0xC0] = {RET, IMP, R_NONE, R_NONE, CT_NZ}, [0xC8] = {RET, IMP, R_NONE, R_NONE, CT_NZ}, [0xC9] = {RET, IMP, R_NONE, R_NONE, CT_NONE}, [0xD0] = {RET, IMP, R_NONE, R_NONE, CT_NC}, [0xD8] = {RET, IMP, R_NONE, R_NONE, CT_C},
    //RETI
    [0xD9] = {RETI, IMP, R_NONE, R_NONE, CT_NONE},
    //RST
    [0xC7] = {RST, IMP, R_NONE, R_NONE, CT_NONE, 0x00}, [0xCF] = {RST, IMP, R_NONE, R_NONE, CT_NONE, 0x08}, [0xD7] = {RST, IMP, R_NONE, R_NONE, CT_NONE, 0x10}, [0xDF] = {RST, IMP, R_NONE, R_NONE, CT_NONE, 0x18}, [0xE7] = {RST, IMP, R_NONE, R_NONE, CT_NONE, 0x20}, [0xEF] = {RST, IMP, R_NONE, R_NONE, CT_NONE, 0x28}, [0xF7] = {RST, IMP, R_NONE, R_NONE, CT_NONE, 0x30}, [0xFF] = {RST, IMP, R_NONE, R_NONE, CT_NONE, 0x38}, 
    //INC
    [0x03] = {INC, R, BC}, [0x04] = {INC, R, B}, [0x0C] = {INC, R}, [0x13] = {INC, R, DE}, [0x14] = {INC, R, D}, [0x1C] = {INC, R, E},  [0x23] = {INC, R, HL}, [0x24] = {INC, R, H}, [0x2C] = {INC, R, L}, [0x33] = {INC, R, SP}, [0x34] = {INC, MR, HL}, [0x3C] = {INC, R, A},
    //DEC
    [0x05] = {DEC, R, B}, [0x0B] = {DEC, R, BC}, [0x0D] = {DEC, R, C}, [0x15] = {DEC, R, D}, [0x1B] = {DEC, R, DE}, [0x1D] = {DEC, R, E}, [0x25] = {DEC, R, H}, [0x2B] = {DEC, R, HL}, [0x2D] = {DEC, R, L}, [0x35] = {DEC, MR, HL}, [0x3B] = {DEC, R, SP}, [0x3D] = {DEC, R, A},
    //ADD
    [0x09] ={ADD, R_R, HL, BC}, [0x19]={ADD, R_R, HL, DE}, [0x29] = {ADD, R_R, HL, HL}, [0x39] = {ADD, R_R, HL, SP}, [0x80] = {ADD, R_R, A, B}, [0x81] = {ADD, R_R, A, C}, [0x82] = {ADD, R_R, A, B}, [0x83] = {ADD, R_R, A, E}, [0x84] = {ADD, R_R, A, H}, [0x85] = {ADD, R_R, A, L}, [0x86] = {ADD, R_MR, A, HL}, [0x87] = {ADD, R_MR, A, A}, [0xC6] = {ADD, R_A8, A}, [0xE8] = {ADD, R_D8, SP},
    //ADC
    [0x88] = {ADC, R_R, B}, [0x89] = {ADC, R_R, C}, [0x8A] = {ADC, R_R, D}, [0x8B] = {ADC, R_R, E}, [0x8C] = {ADC, R_R, H}, [0x8D] = {ADC, R_R, L}, [0x8E] = {ADC, R_MR, HL}, [0x8F] = {ADC, R_R, A}, [0xCE] = {ADC, R_D8, R_NONE},
    //SUB
    [0x90] = {SUB, R, B}, [0x91] = {SUB, R, C}, [0x92] = {SUB, R, D}, [0x93] = {SUB, R, E}, [0x94] = {SUB, R, H}, [0x95] = {SUB, R, L}, [0x96] = {SUB, MR, HL}, [0x97] = {SUB, R, A}, [0xD6] = {SUB, D8, R_NONE},
    //SBC
    [0x98] = {SBC, R_R, B}, [0x99] = {SBC, R_R, C},[0x9A] = {SBC, R_R, D},[0x9B] = {SBC, R_R, E},[0x9C] = {SBC, R_R, H},[0x9D] = {SBC, R_R, L},[0x9E] = {SBC, R_MR, HL},[0x9F] = {SBC, R_R, A}, [0xDE] = {SBC, R_D8, R_NONE},
    //AND
    [0xA0] = {AND, R_R, B}, [0xA1] = {AND, R_R, C}, [0xA2] = {AND, R_R, D}, [0xA3] = {AND, R_R, E}, [0xA4] = {AND, R_R, H}, [0xA5] = {AND, R_R, L}, [0xA6] = {AND, R_MR, HL}, [0xA7] = {AND, R_R, A}, [0xE6] = {AND, R_D8, R_NONE},
    //XOR
    [0xA8] = {XOR, R_R, B}, [0xA9] = {XOR, R_R, C}, [0xAA] = {XOR, R_R, D}, [0xAB] = {XOR, R_R, E}, [0xAC] = {XOR, R_R, H}, [0xAD] = {XOR, R_R, L}, [0xAE] = {XOR, R_MR, HL}, [0xAF] = {XOR, R_R, A}, [0xEE] = {XOR, R_D8, R_NONE},  
    //OR
    [0xB0] = {OR, R_R, B}, [0xB1] = {OR, R_R, C}, [0xB2] = {OR, R_R, D}, [0xB3] = {OR, R_R, E}, [0xB4] = {OR, R_R, H}, [0xB5] = {OR, R_R, L}, [0xB6] = {OR, R_MR, HL}, [0xB7] = {OR, R_R, A}, [0xF6] = {OR, R_D8, R_NONE},  
    //CP
    [0xB8] = {CP, R_R, B}, [0xB9] = {CP, R_R, C}, [0xBA] = {CP, R_R, D}, [0xBB] = {CP, R_R, E}, [0xBC] = {CP, R_R, H}, [0xBD] = {CP, R_R, L}, [0xBE] = {CP, R_MR, HL}, [0xBF] = {CP, R_R, A}, [0xFE] = {CP, R_D8, R_NONE}, 
    //OTHER INSTRUCTIONS
    [0xCB] = {CB},
    [0x07] = {RLCA},
    [0x0F] = {RRCA},
    [0x10] = {STOP},
    [0x17] = {RLA},
    [0x1F] = {RRA},
    [0x27] = {DAA},
    [0x2F] = {CPL},
    [0x37] = {SCF},
    [0x3F] = {CCF},
    [0xF3] = {DI},
    [0xFB] = {EI},

};

char *inst_name(instr_type t) { return instr_type_to_name[t]; }

Instruction* instr_lookup(uint8_t opcode) { return &instruction_lookup_table[opcode]; }



char* reg_to_symbol(reg r){
    switch(r){
        case(R_NONE): return "No Register";
        case(A): return "A";
        case(B): return "B";
        case(C): return "C";
        case(D): return "D";
        case(E): return "E";
        case(F): return "F";
        case(H): return "H";
        case(L): return "L";
        case(AF): return "AF";
        case(BC): return "BC";
        case(DE): return "DE";
        case(HL): return "HL";
        case(SP): return "SP";
        case(PC): return "PC";
        default: return "Invalid register lookup";
    }
}