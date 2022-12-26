#include "cpu.h"

/* ------ CPU REGISTERS -------- */

uint8_t a_reg = 0x1;
uint8_t flag_reg = 0xB0; // (f)
uint8_t b_reg = 0x0;
uint8_t c_reg = 0x13;
uint8_t d_reg = 0x0;
uint8_t e_reg = 0xD8;
uint8_t h_reg = 0x1;
uint8_t l_reg = 0x4D;
uint16_t sp_reg = 0xFFFE;
uint16_t pc_reg = 0x100; 


/* ----- CPU FUNCTIONALITY ----- */

CPU_STATE cpu = {0, false, false, 0};

void execute_instruction(){
    Instruction* instruction = cpu.currInstr;
    addr_mode mode = instruction->mode;

    if(instruction->type == LD){
        /* LOAD INSTRUCTIONS */
        switch(mode){
            case(R_D16):
                uint16_t LSB = bus_read(pc_reg); //lo
                pc_reg++;
                uint16_t MSB = bus_read(pc_reg); //hi
                pc_reg++;
                uint16_t d16 = LSB | (MSB << 8);
                ld_r_16(instruction->reg_1, instruction->reg_2, d16);
                break;
            case(R_D8):
                uint8_t d8 = bus_read(pc_reg);
                pc_reg++;
                ld_r_8(instruction->reg_1, d8);
                break;  
            case(R_R):
                printf("");
            case(HLI_R):
                printf("");
            case(R_HLI):
                printf("");
            case(R_HLD):
                printf("");
            case(HLD_R):
                printf("");                               
            default:
                printf("No mode exists on load instruction\n");
        }
        return;

    }
    else if(instruction->type == XOR){
        /* XOR INSTRUCTIONS */

    }
}
void fetch_instruction(){
    cpu.currInstrOpcode =  bus_read(pc_reg);
    pc_reg++;
    cpu.currInstr = instr_lookup(cpu.currInstrOpcode);
}
// void fetch_data(){

// }
bool cpu_step(){
    if(!cpu.halted){
        fetch_instruction();
        execute_instruction();
    }

    return false;
}




/* Access and modify flag register */

//Z - bit 7
uint8_t zero_flag(){ return (flag_reg >> 7); }
//N -  bit 6
uint8_t add_zero_flag(){ return ( (flag_reg << 1) >> 7);}
//H - bit 5
uint8_t half_carry_flag(){ return ( (flag_reg << 2) >> 7); }
//C - bit 4
uint8_t carry_flag(){ return ( (flag_reg << 3) >> 7); }

void set_flag(bool val, uint8_t flag){
    if(flag >= 0 && flag <= 3) {
        printf("Forbidden Bit attempted to be modified on flag register\n");
        return;
    }
    if(flag == 4){
        //modify bit at position (flag) to value of val
        uint8_t mask = 1 << 4;
        flag_reg = (flag_reg & ~mask) | ( (val << 4) & mask);
    }
    else if(flag == 5){
        uint8_t mask = 1 << 5;
        flag_reg = (flag_reg & ~mask) | ( (val << 5) & mask);
    }
    else if (flag == 6 ){
        uint8_t mask = 1 << 6;
        flag_reg = (flag_reg & ~mask) | ( (val << 6) & mask);
    }
    else{
        uint8_t mask = 1 << 7;
        flag_reg = (flag_reg & ~mask) | ( (val << 7) & mask);
    }
}

/* Instruction Functions */

void ld_r_16(reg reg1, reg reg2, uint16_t d16){
    if(reg1 == B && reg2 == C){
        //hi
        b_reg = d16 >> 4;
        //lo
        c_reg = (d16 << 4) >> 4;
    }
    else if(reg1 == D && reg2 == E){
        d_reg = d16 >> 4;
        e_reg = (d16 << 4) >> 4;
    }
    else if(reg1 == H && reg2 == L){
        h_reg = d16 >> 4;
        l_reg = (d16 << 4) >> 4;
    }
    else if(reg1 == SP && reg2 == R_NONE){
        sp_reg == d16;
    }
    else{
        printf("Invalid function call of ld_r_16\n");
    }
}
void ld_r_8(reg reg1, uint8_t d8){
    switch(reg1){
        case(A):
            a_reg = d8;
            break;
        case(B):
            b_reg = d8;
            break;
        case(C):
            c_reg = d8;
            break;
        case(D):
            d_reg = d8;
            break;   
        case(E):
            e_reg = d8;
            break;   
        case(H):
            h_reg = d8;
            break;    
        case(L):
            l_reg = d8;
            break;  
        default:
           printf("Invalid function call of ld_r_8\n");                                        
    }
}