#include "cpu.h"

/* ------ CPU REGISTERS -------- */

uint8_t a_reg = 0x01;
uint8_t flag_reg = 0xB0; // (f)
uint8_t b_reg = 0x00;
uint8_t c_reg = 0x13;
uint8_t d_reg = 0x00;
uint8_t e_reg = 0xD8;
uint8_t h_reg = 0x01;
uint8_t l_reg = 0x4D;
uint16_t sp_reg = 0xFFFE;
uint16_t pc_reg = 0x100; 


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


/* Load Functions */
void ld_r_16(reg reg1,  uint16_t d16){
    if(reg1 == BC){
        //hi
        b_reg = d16 >> 4;
        //lo
        c_reg = (d16 << 4) >> 4;
    }
    else if(reg1 == DE){
        d_reg = d16 >> 4;
        e_reg = (d16 << 4) >> 4;
    }
    else if(reg1 == HL){
        h_reg = d16 >> 4;
        l_reg = (d16 << 4) >> 4;
    }
    else if(reg1 == SP){
        sp_reg = d16;
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

void ld_hli_r(reg reg1){
    u_int16_t addr = ( (uint16_t)l_reg ) | ( (uint16_t)(h_reg << 8) );
    bus_write(addr, reg1);
    l_reg++;
}

void ld_hld_r(reg reg1){
    u_int16_t addr = ( (uint16_t)l_reg ) | ( (uint16_t)(h_reg << 8) );
    bus_write(addr, reg1);
    l_reg--;
}

void ld_r_hli(reg reg1){
    u_int16_t addr = ( (uint16_t)l_reg ) | ( (uint16_t)(h_reg << 8) );
    reg1 = bus_read(addr);
    l_reg++;

}
void ld_r_hld(reg reg1){
    u_int16_t addr = ( (uint16_t)l_reg ) | ( (uint16_t)(h_reg << 8) );
    reg1 =  bus_read(addr);
    l_reg--;
}

void ld_r_mr(reg reg1, reg reg2){
    uint8_t data;
    uint16_t addr;
    switch(reg2){
        case(AF):
            addr = ( (uint16_t)flag_reg ) | ( (uint16_t)(a_reg << 8) );
            data = bus_read(addr);  
            break;      
        case(BC):
            addr = ( (uint16_t)c_reg ) | ( (uint16_t)(b_reg << 8) );
            data = bus_read(addr);
            break;
        case(DE):
            addr = ( (uint16_t)e_reg ) | ( (uint16_t)(d_reg << 8) );
            data = bus_read(addr);
            break;
        case(HL):
            addr = ( (uint16_t)l_reg ) | ( (uint16_t)(h_reg << 8) );
            data = bus_read(addr);
            break;
        default:
            printf("Invalid reg2 on ld_r_mr\n");
    }
    switch(reg1){
        case(A):
            a_reg = data;
            break;
        case(B):
            b_reg = data;
            break;
        case(C):
            c_reg = data;
            break;
        case(D):
            d_reg = data;
            break;   
        case(E):
            e_reg = data;
            break;   
        case(F):
            flag_reg = data;
            break;            
        case(H):
            h_reg = data;
            break;    
        case(L):
            l_reg = data;
            break; 
        default:
           printf("Invalid reg1 on ld_r_mr\n"); 
    }
}

void ld_mr_r(reg reg1, reg reg2){
     
    uint8_t data;
    switch(reg2){
        case(A):
            data = a_reg;
            break;
        case(B):
            data = b_reg;
            break;
        case(C):
            data = c_reg;
            break;
        case(D):
            data = d_reg;
            break;   
        case(E):
            data = e_reg;
            break;   
        case(F):
            data = flag_reg;
            break;            
        case(H):
            data = h_reg;
            break;    
        case(L):
            data = l_reg;
            break; 
        default:
           printf("Invalid reg2 on ld_mr_r\n"); 
    }
    switch(reg1){
        uint16_t addr;
        case(AF):
            addr = ( (uint16_t)flag_reg ) | ( (uint16_t)(a_reg << 8) );
            bus_write(addr, data);  
            break;      
        case(BC):
            addr = ( (uint16_t)c_reg ) | ( (uint16_t)(b_reg << 8) );
            bus_write(addr, data); 
            break;
        case(DE):
            addr = ( (uint16_t)e_reg ) | ( (uint16_t)(d_reg << 8) );
            bus_write(addr, data); 
            break;
        case(HL):
            addr = ( (uint16_t)l_reg ) | ( (uint16_t)(h_reg << 8) );
            bus_write(addr, data); 
            break;
        default:
            printf("Invalid reg1 on ld_mr_r\n");
    }  
}

void ld_r_r(reg reg1, reg reg2){
    switch(reg1){
        case(A):
            if(reg2 == B){
                a_reg = b_reg;
            }
            else if(reg2 == C){
                a_reg = c_reg;
            }
            else if(reg2 == D){
                a_reg = d_reg;
            }
            else if(reg2 == E){
                a_reg = e_reg;
            }
            else if(reg2 == H){
                a_reg = h_reg;
            }
            else if(reg2 == L){
                a_reg = l_reg;
            }
            else{
                printf("Invalid reg2 on ld_r_r\n");
            }
            break;
        case(B):
            if(reg2 == A){
                b_reg = a_reg;
            }
            else if(reg2 == C){
                b_reg = c_reg;
            }
            else if(reg2 == D){
                b_reg = d_reg;
            }
            else if(reg2 == E){
                b_reg = e_reg;
            }
            else if(reg2 == H){
                b_reg = h_reg;
            }
            else if(reg2 == L){
                b_reg = l_reg;
            }
            else{
                printf("Invalid reg2 on ld_r_r\n");
            }
            break;
        case(C):
            if(reg2 == A){
                c_reg = a_reg;
            }
            else if(reg2 == B){
                c_reg = b_reg;
            }
            else if(reg2 == D){
                c_reg = d_reg;
            }
            else if(reg2 == E){
                c_reg = e_reg;
            }
            else if(reg2 == H){
                c_reg = h_reg;
            }
            else if(reg2 == L){
                c_reg = l_reg;
            }
            else{
                printf("Invalid reg2 on ld_r_r\n");
            }
            break;
        case(D):
            if(reg2 == A){
                d_reg = a_reg;
            }
            else if(reg2 == B){
                d_reg = b_reg;
            }
            else if(reg2 == C){
                d_reg = c_reg;
            }
            else if(reg2 == E){
                d_reg = e_reg;
            }
            else if(reg2 == H){
                d_reg = h_reg;
            }
            else if(reg2 == L){
                d_reg = l_reg;
            }
            else{
                printf("Invalid reg2 on ld_r_r\n");
            }
            break;   
        case(E):
            if(reg2 == A){
                e_reg = a_reg;
            }
            else if(reg2 == B){
                e_reg = b_reg;
            }
            else if(reg2 == C){
                e_reg = c_reg;
            }
            else if(reg2 == D){
                e_reg = d_reg;
            }
            else if(reg2 == H){
                e_reg = h_reg;
            }
            else if(reg2 == L){
                e_reg = l_reg;
            }
            else{
                printf("Invalid reg2 on ld_r_r\n");
            }
            break;   
        case(H):
            if(reg2 == A){
                h_reg = a_reg;
            }
            else if(reg2 == B){
                h_reg = b_reg;
            }
            else if(reg2 == C){
                h_reg = c_reg;
            }
            else if(reg2 == D){
                h_reg = d_reg;
            }
            else if(reg2 == E){
                h_reg = e_reg;
            }
            else if(reg2 == L){
                h_reg = l_reg;
            }
            else{
                printf("Invalid reg2 on ld_r_r\n");
            }
            break;    
        case(L):
            if(reg2 == A){
                l_reg = a_reg;
            }
            else if(reg2 == B){
                l_reg = b_reg;
            }
            else if(reg2 == C){
                l_reg = c_reg;
            }
            else if(reg2 == D){
                l_reg = d_reg;
            }
            else if(reg2 == E){
                l_reg = e_reg;
            }
            else if(reg2 == H){
                l_reg = h_reg;
            }
            else{
                printf("Invalid reg2 on ld_r_r\n");
            }
            break; 
        case(SP):
            sp_reg = ( (uint16_t)l_reg ) | ( (uint16_t)(h_reg << 8) );
            break;
        default:
           printf("Invalid function call of ld_r_8\n"); 
    }
}


void ld_mr_d8(reg reg1, uint8_t d8){
    uint16_t addr;
    switch(reg1){
        case(AF):
            addr= ((uint16_t)flag_reg) | ( (uint16_t)(a_reg << 8) ); 
            break;      
        case(BC):
            addr = ( (uint16_t)c_reg ) | ( (uint16_t)(b_reg << 8) );
            break;
        case(DE):
            addr = ( (uint16_t)e_reg ) | ( (uint16_t)(d_reg << 8) );
            break;
        case(HL):
            addr = ( (uint16_t)l_reg ) | ( (uint16_t)(h_reg << 8) );
            break;
        default:
            printf("Invalid reg2 on ld_r_mr\n");
            return;
    }
    bus_write(addr, d8);
}

// void ldh_r_mr(reg reg1, reg reg2){
//     uint8_t data;
//     switch(reg2){
//         case(A):
//             data = a_reg;
//             break;
//         case(B):
//             data = b_reg;
//             break;
//         case(C):
//             data = c_reg;
//             break;
//         case(D):
//             data = d_reg;
//             break;   
//         case(E):
//             data = e_reg;
//             break;   
//         case(F):
//             data = flag_reg;
//             break;            
//         case(H):
//             data = h_reg;
//             break;    
//         case(L):
//             data = l_reg;
//             break; 
//         default:
//            printf("Invalid reg2 on ld_mr_r\n"); 
//            return;
//     }
//     uint16_t addr = ((uint16_t)data) | ( (uint16_t)(0xFF << 8) );
//     uint8_t data = bus_read(addr);
//     switch(reg1){
//         case(A):
//             a_reg = data;
//             break;
//         case(B):
//             b_reg = data;
//             break;
//         case(C):
//             c_reg = data;
//             break;
//         case(D):
//             d_reg = data;
//             break;   
//         case(E):
//             e_reg = data;
//             break;   
//         case(F):
//             flag_reg = data;
//             break;            
//         case(H):
//             h_reg = data;
//             break;    
//         case(L):
//             l_reg = data;
//             break; 
//         default:
//            printf("Invalid reg1 on ld_r_mr\n"); 
//     }
// }

void ldh_mr_r(reg reg1, reg reg2){
    uint8_t data;
    switch(reg1){
        case(A):
            data = a_reg;
            break;
        case(B):
            data = b_reg;
            break;
        case(C):
            data = c_reg;
            break;
        case(D):
            data = d_reg;
            break;   
        case(E):
            data = e_reg;
            break;   
        case(F):
            data = flag_reg;
            break;            
        case(H):
            data = h_reg;
            break;    
        case(L):
            data = l_reg;
            break; 
        default:
           printf("Invalid reg2 on ld_mr_r\n"); 
           return;
    }  
    uint16_t addr = ((uint16_t)data) | ( (uint16_t)(0xFF << 8) );
    switch(reg2){
        case(A):
            bus_write(addr, a_reg);
            break;
        case(B):
            bus_write(addr, b_reg);
            break;
        case(C):
            bus_write(addr, c_reg);
            break;
        case(D):
            bus_write(addr, d_reg);
            break;   
        case(E):
            bus_write(addr, e_reg);
            break;   
        case(F):
            bus_write(addr, flag_reg);
            break;            
        case(H):
            bus_write(addr, h_reg);
            break;    
        case(L):
            bus_write(addr, l_reg);
            break; 
        default:
           printf("Invalid reg1 on ld_r_mr\n"); 
    }
}

void ld_r_a8(reg reg1, uint8_t a8){
    uint16_t addr = ((uint16_t)a8) | ( (uint16_t)(0xFF << 8) );
    uint8_t data = bus_read(addr);
    switch(reg1){
        case(A):
            a_reg = data;
            break;
        case(B):
            b_reg = data;
            break;
        case(C):
            c_reg = data;
            break;
        case(D):
            d_reg = data;
            break;   
        case(E):
            e_reg = data;
            break;   
        case(F):
            flag_reg = data;
            break;            
        case(H):
            h_reg = data;
            break;    
        case(L):
            l_reg = data;
            break; 
        default:
           printf("Invalid reg1 on ld_r_mr\n"); 
           return;
    }
}

void ld_a8_r(reg reg1, uint8_t a8){
    uint8_t data;
    switch(reg1){
        case(A):
            data = a_reg;
            break;
        case(B):
            data = b_reg;
            break;
        case(C):
            data = c_reg;
            break;
        case(D):
            data = d_reg;
            break;   
        case(E):
            data = e_reg;
            break;   
        case(F):
            data = flag_reg;
            break;            
        case(H):
            data = h_reg;
            break;    
        case(L):
            data = l_reg;
            break; 
        default:
           printf("Invalid reg2 on ld_mr_r\n"); 
           return;
    }  
    uint16_t addr = ((uint16_t)a8) | ( (uint16_t)(0xFF << 8) );
    bus_write(addr, data);
}

void ld_a16_r(reg reg1, uint16_t a16){
    uint8_t data;
    switch(reg1){
        case(A):
            data = a_reg;
            break;
        case(B):
            data = b_reg;
            break;
        case(C):
            data = c_reg;
            break;
        case(D):
            data = d_reg;
            break;   
        case(E):
            data = e_reg;
            break;   
        case(F):
            data = flag_reg;
            break;            
        case(H):
            data = h_reg;
            break;    
        case(L):
            data = l_reg;
            break; 
        default:
           printf("Invalid reg2 on ld_mr_r\n"); 
           return;
    } 
    bus_write(a16, data);
}

void ld_r_a16(reg reg1, uint16_t a16){
    uint8_t data = bus_read(a16);
    switch(reg1){
        case(A):
            a_reg = data;
            break;
        case(B):
            b_reg = data;
            break;
        case(C):
            c_reg = data;
            break;
        case(D):
            d_reg = data;
            break;   
        case(E):
            e_reg = data;
            break;   
        case(F):
            flag_reg = data;
            break;            
        case(H):
            h_reg = data;
            break;    
        case(L):
            l_reg = data;
            break; 
        default:
           printf("Invalid reg1 on ld_r_mr\n"); 
           return;
    }    
}

void ld_hl_spr(int8_t r8){
    /**
    case 0xF8: 
        int8_t offset = (int8_t) __gb_read(gb, gb->cpu_reg.pc++);
        gb->cpu_reg.hl = gb->cpu_reg.sp + offset;
        gb->cpu_reg.f_bits.z = 0;
        gb->cpu_reg.f_bits.n = 0;
        gb->cpu_reg.f_bits.h =
            ((gb->cpu_reg.sp & 0xF) + (offset & 0xF) > 0xF) ? 1 : 0;
        gb->cpu_reg.f_bits.c =
            ((gb->cpu_reg.sp & 0xFF) + (offset & 0xFF) > 0xFF) ? 1 : 0;
    **/

    set_flag(false, 7); //Z
    set_flag(false, 6); //N

    uint16_t res = sp_reg + r8;
    h_reg = res >> 4; // 12345678 => 00001234
    l_reg = (res << 4) >> 4; //12345678 => 56780000 => 00005678
    set_flag( (( (sp_reg & 0xF) + (r8 & 0xF) ) > 0xF ) ? 1 : 0,  5); //H
    set_flag( (( (sp_reg & 0xFF)+ (r8 & 0xFF)) > 0xFF) ? 1 : 0,  4); //C
}

/* ----- CPU FUNCTIONALITY ----- */

CPU_STATE cpu = {0, false, false, 0};

void execute_instruction(){
    Instruction* instruction = cpu.currInstr;
    addr_mode mode = instruction->mode;

    if(instruction->type == LD){
        /* LOAD INSTRUCTIONS */
        switch(mode){
            uint16_t LSB;
            uint16_t MSB;
            uint16_t d16;
            uint8_t d8;
            uint16_t a16;
            uint8_t a8;
            int8_t r8;
            case(R_D16):
                LSB = bus_read(pc_reg); //lo
                pc_reg++;
                tick();
                MSB = bus_read(pc_reg); //hi
                pc_reg++;
                tick();
                d16 = LSB | (MSB << 8);
                ld_r_16(instruction->reg_1, d16);
                break;
            case(R_D8):
                d8 = bus_read(pc_reg);
                pc_reg++;
                tick();
                ld_r_8(instruction->reg_1, d8);
                break;  
            case(R_R):
                ld_r_r(instruction->reg_1, instruction->reg_2);
                break; 
            case(HLI_R):
                ld_hli_r(instruction->reg_1);
                break; 
            case(HLD_R):
                ld_hld_r(instruction->reg_1);
                break; 
            case(R_HLI):
                ld_r_hli(instruction->reg_1);
                break; 
            case(R_HLD):
                ld_hld_r(instruction->reg_1);  
                break; 
            case(MR_R):
                if(instruction->reg_2 == A){
                    ld_mr_r(instruction->reg_1, (uint16_t)0xFF00 + (uint16_t)instruction->reg_2);
                    break;
                }
                ld_mr_r(instruction->reg_1, instruction->reg_2);
                break; 
            case(R_MR):
                if(instruction->reg_1 == A){
                    ld_mr_r((uint16_t)0xFF00 + (uint16_t)instruction->reg_1,instruction->reg_2);
                    break;
                }
                ld_r_mr(instruction->reg_1, instruction->reg_2);    
                break;            
            case(MR_D8):
                d8 = bus_read(pc_reg);
                pc_reg++;
                tick();
                ld_mr_d8(instruction->reg_1, d8);   
                break;   
            case(A16_R):
                LSB = bus_read(pc_reg); //lo
                pc_reg++;
                tick();
                MSB = bus_read(pc_reg); //hi
                pc_reg++;
                tick();
                a16 = LSB | (MSB << 8);  
                ld_a16_r(instruction->reg_1, a16);
                break;    
            case(R_A16):
                LSB = bus_read(pc_reg); //lo
                pc_reg++;
                tick();
                MSB = bus_read(pc_reg); //hi
                pc_reg++;
                tick();
                a16 = LSB | (MSB << 8);   
                ld_r_a16(instruction->reg_1, a16);
                break;     
            case(A8_R):
                a8 = bus_read(pc_reg);
                pc_reg++;
                tick();          
                ld_a8_r(instruction->reg_1, a8);
                break;    
            case(R_A8):
                a8 = bus_read(pc_reg);
                pc_reg++;
                tick();
                ld_r_a8(instruction->reg_1, a8);          
                break;
            case(HL_SPR):
                r8 = bus_read(pc_reg); //this is signed apparently
                pc_reg++;
                tick();
                ld_hl_spr(r8);
                break;                                                                           
            default:
                printf("No mode exists on LD instruction\n");
        }
        return;

    }
    else if(instruction->type == LDH){
        switch(instruction->mode){
            uint8_t a8;
            case(A8_R):
                a8 = bus_read(pc_reg);
                pc_reg++;
                tick();          
                ld_a8_r(instruction->reg_1, a8);
                break;
            case(R_A8):
                a8 = bus_read(pc_reg);
                pc_reg++;
                tick();
                ld_r_a8(instruction->reg_1, a8);
                break;                     
            default:
                printf("No mode exists on LDH instruction\n");            
        }
    }
}
void fetch_instruction(){
    cpu.currInstrOpcode =  bus_read(pc_reg);
    pc_reg++;
    tick();
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

/* testing helpers */
uint8_t get_a_reg(){ return a_reg; }
uint8_t get_f_reg(){ return flag_reg; }
uint8_t get_b_reg(){ return b_reg; }
uint8_t get_c_reg(){ return c_reg; }
uint8_t get_d_reg(){ return d_reg; }
uint8_t get_e_reg(){ return e_reg; }
uint8_t get_h_reg(){ return h_reg; }
uint8_t get_l_reg(){ return l_reg; }
uint16_t   get_sp(){ return sp_reg;}
uint16_t   get_pc(){ return pc_reg;}
void set_a_reg(uint8_t x){a_reg = x;}
void set_f_reg(uint8_t x){flag_reg = x;}
void set_b_reg(uint8_t x){b_reg = x;}
void set_c_reg(uint8_t x){c_reg = x;}
void set_d_reg(uint8_t x){d_reg = x;}
void set_e_reg(uint8_t x){e_reg = x;}
void set_h_reg(uint8_t x){h_reg = x;}
void set_l_reg(uint8_t x){l_reg = x;}
void   set_sp(uint16_t x){sp_reg = x;}
void   set_pc(uint16_t x){pc_reg = x;}