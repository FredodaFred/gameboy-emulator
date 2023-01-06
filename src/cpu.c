#include "cpu.h"

CPU_STATE cpu = {0, false, false, 0};

/* -------- CPU REGISTERS -------- */

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

/* -------- Interrupts -------- */

//The IME flag is used to disable all interrupts, overriding any enabled bits in the IE register.
bool IME = false;
//Marks interrupt that are enabled
uint8_t ie_reg = 0x0;
//Any set bits in the IF register are only requesting an interrupt
uint8_t if_reg = 0x0;

void handle_interrupt(){
    stkpush16(pc_reg);
    if((if_reg & IT_VBLANK) && (ie_reg & IT_VBLANK)){
        pc_reg = 0x40;
    }
    else if( (if_reg & IT_LCD_STAT) && (ie_reg & IT_LCD_STAT)){
        pc_reg = 0x48;
    }
    else if((if_reg & IT_TIMER) && (ie_reg & IT_TIMER)){
        pc_reg = 0x50;
    }
    else if((if_reg & IT_SERIAL) && (ie_reg & IT_SERIAL)){
        pc_reg = 0x58;
    }   
    else if((if_reg & IT_JOYPAD) && (ie_reg & IT_JOYPAD)){
        pc_reg = 0x60;
    } 
    if_reg &= ~if_reg;
    cpu.halted = false;    
    IME = false;
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

/**
 * Z - bit 7
 * N - bit 6
 * H - bit 5
 * C - bit 4
 **/
void set_flag(bool val, uint8_t flag){
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
    else if (flag == 7){
        uint8_t mask = 1 << 7;
        flag_reg = (flag_reg & ~mask) | ( (val << 7) & mask);
    }
    else{
        printf("Invalid flag bit attempted to be changed\n");
    }
}

//assist with instrucitons that have conditions
bool check_cond(cond_type cond){
    bool z = zero_flag();
    bool c = carry_flag();
    switch(cond){
        case(CT_NONE): return true;
        case(CT_C):  return c;
        case(CT_NC): return !c;
        case(CT_Z):  return z;
        case(CT_NZ): return !z;
    }
    return false;
}
//retrieve reg value from enum type
uint16_t read_reg(reg regt){
    switch(regt){
        case(A):  return a_reg;
        case(B):  return b_reg;
        case(C):  return c_reg;
        case(D):  return d_reg;
        case(E):  return e_reg;
        case(F):  return flag_reg;
        case(H):  return h_reg;
        case(L):  return l_reg;
        //((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8); is the code below correct?
        case(AF): return ( (uint16_t)(a_reg << 8) | (uint16_t)flag_reg);
        case(BC): return ( (uint16_t)(b_reg << 8) | (uint16_t)c_reg );
        case(DE): return ( (uint16_t)(d_reg << 8) | (uint16_t)e_reg);
        case(HL): return ( (uint16_t)(h_reg << 8) | (uint16_t)l_reg);
        case(SP): return sp_reg;
        case(PC): return pc_reg;
        default: printf("Read reg fail\n"); return 0;

    }
}

void set_reg(reg regt, uint16_t val){
    switch(regt){
        case(A): a_reg = val & 0xFF; break;
        case(B): b_reg = val & 0xFF; break;
        case(C): c_reg = val & 0xFF; break;
        case(D): d_reg = val & 0xFF; break;
        case(E): e_reg = val & 0xFF; break;
        case(F): flag_reg = val & 0xFF; break;
        case(H): h_reg = val & 0xFF; break;
        case(L): l_reg = val & 0xFF; break;
        //is the code below correct?
        case(AF): a_reg = (uint8_t)(val >> 8); flag_reg = (uint8_t)(val & 0x00FF); break;
        case(BC): b_reg = (uint8_t)(val >> 8); c_reg = (uint8_t)(val & 0x00FF); break;
        case(DE): d_reg = (uint8_t)(val >> 8); d_reg = (uint8_t)(val & 0x00FF); break;
        case(HL): h_reg = (uint8_t)(val >> 8); l_reg = (uint8_t)(val & 0x00FF); break;
        case(SP): sp_reg = val; break;
        case(PC): pc_reg = val; break;
        default: printf("Set reg fail\n");
    }
}

/* Instruction Functions */

/* Load Functions */
void ld_r_16(reg reg1){
    uint8_t LSB = bus_read(pc_reg); //lo
    pc_reg++;
    tick();
    uint8_t MSB = bus_read(pc_reg); //hi
    pc_reg++;
    tick();
    uint16_t d16 = LSB | (MSB << 8);    
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
    tick();
}

void ld_r_8(reg reg1){
    uint8_t d8 = bus_read(pc_reg);
    pc_reg++;
    tick();   
    set_reg(reg1, d8);
    tick();
}

void ld_hli_r(reg reg1){
    uint16_t addr = ( (uint16_t)(h_reg << 8) | ( (uint16_t)l_reg ) );
    bus_write(addr, reg1);
    tick();   
    l_reg++;
    tick();
}

void ld_hld_r(reg reg1){
    uint16_t addr = ( (uint16_t)l_reg ) | ( (uint16_t)(h_reg << 8) );
    bus_write(addr, reg1);
    tick();   
    l_reg--;
    tick();
}

void ld_r_hli(reg reg1){
    uint16_t addr = ( (uint16_t)(h_reg << 8) | ( (uint16_t)l_reg ) );
    set_reg(reg1, bus_read(addr));
    tick();
    l_reg++;
    if(l_reg == 0){
        h_reg++;
    }

    tick();

}
void ld_r_hld(reg reg1){
    uint16_t addr = ( (uint16_t)l_reg ) | ( (uint16_t)(h_reg << 8) );
    set_reg(reg1, bus_read(addr));
    tick();
    l_reg--;
    tick();
}

void ld_r_mr(reg reg1, reg reg2){
    uint16_t addr = read_reg(reg2);
    uint8_t data = bus_read(addr);
    set_reg(reg1, data);
    tick();
}

void ld_mr_r(reg reg1, reg reg2){
    uint16_t addr = read_reg(reg1);
    uint8_t data = read_reg(reg2);
    bus_write(addr, data);
    tick(); 
}

void ld_r_r(reg reg1, reg reg2){
    set_reg(reg1, read_reg(reg2));
    tick();
}


void ld_mr_d8(reg reg1, uint8_t d8){
    uint16_t addr = read_reg(reg1);
    bus_write(addr, d8);
}


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
           printf("Invalid reg2 on ldh_mr_r\n"); 
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
           printf("Invalid reg2 on ld_a8_r\n"); 
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
           printf("Invalid reg2 on ld_a16_r\n"); 
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

/* JUMP, PUSH AND POP INSTRUCTIONS */

void push(reg reg1){

    uint16_t hi = 0;
    uint16_t lo = 0;
    if(reg1 == AF){
        hi = get_a_reg();
        tick();
        lo = get_f_reg();
        tick();
    }
    else if(reg1 == BC){
        hi = get_b_reg();
        tick();
        lo = get_c_reg();
        tick();
    }
    else if(reg1 == DE){
        hi = get_d_reg();
        tick();
        lo = get_e_reg();
        tick();
    }
    else if(reg1 == HL){
        hi = get_h_reg();
        tick();
        lo = get_l_reg();
        tick();
    }
    stkpush(hi);
    stkpush(lo);
    tick();
}

void pop(reg reg1){
    uint8_t hi = stkpop();
    tick();
    uint8_t lo = stkpop();
    tick();
    //uint16_t data = (hi << 8) | lo;
    if(reg1 == AF){
        set_a_reg(hi);
        set_f_reg(lo & 0xF0); //QUESTIONABLE
    }
    else if(reg1 == BC){
        set_b_reg(hi);
        set_c_reg(lo);
    }
    else if(reg1 == DE){
        set_d_reg(hi);
        set_e_reg(lo);
    }
    else if(reg1 == HL){
        set_h_reg(hi);
        set_l_reg(lo);
    }
}

void jump(reg reg1, addr_mode mode, cond_type cond){

    uint16_t LSB = bus_read(pc_reg); //lo
    pc_reg++;
    tick();
    uint16_t MSB = bus_read(pc_reg); //hi
    pc_reg++;
    tick();

    if(!check_cond(cond)){
        return;
    }
    pc_reg = (MSB << 8) | LSB;
    tick();
}

void call(reg reg1, addr_mode mode, cond_type cond){
    if(!check_cond(cond)){
        return;
    }
    tick();
    tick();
    stkpush16(pc_reg);

    uint16_t LSB = bus_read(pc_reg); //lo
    pc_reg++;
    tick();
    uint16_t MSB = bus_read(pc_reg); //hi
    pc_reg++;
    tick();
    pc_reg = (MSB << 8) | LSB;
}

void jumpr(cond_type cond){
    //get r8
    int8_t rel = (char)bus_read(pc_reg);
    pc_reg++;
    tick();
    uint16_t addr = pc_reg + rel;
    if(!check_cond(cond)){
        return;    
    }
    pc_reg = addr;
    tick();
}

void ret(cond_type cond){
    if(cond != CT_NONE){
        tick();
    }
    if(check_cond(cond)){
        uint16_t lo = stkpop();
        tick();
        uint16_t hi = stkpop();
        tick();      
        uint16_t dest = (hi << 8) | lo;
        pc_reg = dest; 
        tick();
    }
}

void reti(){
    IME = true;
    ret(CT_NONE);
}

void rst(uint8_t param){
    tick();
    tick();
    stkpush16(pc_reg);
    pc_reg = param;
    tick();
 
}

void inc(reg reg1, addr_mode mode, uint8_t currOP){
    uint16_t data = read_reg(reg1) + 1;
    if(reg1 >= AF){
        tick();
    }
    if(reg1 == HL && mode == MR){
        data = bus_read(read_reg(HL)) + 1;
        data &= 0xFF;
        bus_write(read_reg(HL), (uint8_t)data);
    }
    else{
        set_reg(reg1, data);
    }

    if((currOP & 0x03) == 0x03){
        return;
    }

    set_flag(read_reg(reg1) == 0, 7);
    set_flag(0, 6);
    set_flag( (read_reg(reg1) & 0x0F) == 0, 5);
}

void dec(reg reg1, addr_mode mode, uint8_t currOP){
    uint16_t data = read_reg(reg1) -1;
    if(reg1 >= AF){
        tick();
    }
    if(reg1 == HL && mode == MR){
        data = bus_read(read_reg(HL)) - 1;
        bus_write(read_reg(HL), (uint8_t)data);
    }
    else{
        set_reg(reg1, data);
    }

    if((currOP & 0x0B) == 0x0B){
        return;
    }
    set_flag(read_reg(reg1) == 0, 7);
    set_flag(0, 6);
    set_flag(( read_reg(reg1) & 0x0F) == 0x0F, 5);
}

void add(reg reg1, reg reg2, uint8_t currOP){
    uint32_t res = 0;
    if(currOP == 0x89){
        uint32_t d8 = (uint32_t)bus_read(read_reg(HL));
        tick();
        res = read_reg(reg1) + d8;
    }
    else if (currOP == 0xC9){
        uint32_t d8 = (uint32_t)bus_read(pc_reg);
        pc_reg++;
        res = read_reg(reg1) + d8;
    }
    else if(reg1 == SP){
        uint32_t d8 = (uint32_t)bus_read(pc_reg);
        pc_reg++;
        tick();
        res = read_reg(reg1) + d8;
    }    
    else{
        res = read_reg(reg1) + read_reg(reg2);
    }
     
    if(reg1 >= AF){
        tick(); 
    }

    
    //handle flags
    if( reg1 < AF){
        set_flag(((res & 0xFF) == 0), 7); 
        set_flag(((read_reg(reg1) & 0xF) + (read_reg(reg2) & 0xF) >= 0x10), 5 );
        set_flag((((int)(read_reg(reg1) & 0xFF) + (int)(read_reg(reg2) & 0xFF)) >= 0x100), 4 );
    }
    else if(reg1 == SP){
        set_flag(0, 7);
        set_flag(((read_reg(reg1) & 0xF) + (read_reg(reg2) & 0xF) >= 0x10), 5);
        set_flag((( (int)(read_reg(reg1) & 0xFF) + (int)(read_reg(reg2) & 0xFF)) >= 0x100),  4);
    }
    else{
        set_flag(((read_reg(reg1) & 0xFFF) + (read_reg(reg2) & 0xFFF) >= 0x1000), 5 );
        uint32_t n = (uint32_t)read_reg(reg1) + (uint32_t)read_reg(reg2);
        set_flag(n >= 0x10000, 4);
    }
    set_flag(0, 6);

    //finally set the reg
    set_reg(reg1, res & 0xFFFF);
    tick();
}

void adc(reg reg1, uint8_t currOP){
    uint16_t data = 0;
    if(currOP == 0x8E){
        tick();
        data = (uint16_t)bus_read(read_reg(HL));
    }
    else if (currOP == 0xCE){
        tick();
        data = bus_read(pc_reg);
        pc_reg++;
    }
    else{
        data = read_reg(reg1);
    }
    a_reg = (a_reg + carry_flag() + data) & 0xFF;
    set_flag(a_reg == 0, 7);
    set_flag(0, 6);
    set_flag( ((a_reg & 0xF)+ carry_flag() + (data & 0xF)) > 0xF, 5);
    set_flag((a_reg + carry_flag() + data) > 0xFF, 4);
    tick();
}

void sub(reg reg1, uint8_t currOP){
    uint8_t data = 0;
    if(currOP == 0x96){
        data = bus_read(read_reg(HL));
        tick();
    }
    else if(currOP == 0xD6){
        data = bus_read(pc_reg);
        pc_reg++;
        tick();
    }
    else{
        data = read_reg(reg1);
    }
    uint16_t res = a_reg - data;
    
    set_flag(res == 0, 7);
    set_flag(1, 6);
    set_flag(( ((int)a_reg & 0xF ) - ( (int)data & 0xF) < 0),5);
    set_flag(( ((int)a_reg) - ( (int)data) < 0), 4);

    a_reg = res & 0xFF;
    tick();
}

void sbc(reg reg1, uint8_t currOP){
    uint8_t data = 0;
    if(currOP == 0x9E){
        data = bus_read(read_reg(HL));
        tick();
    }
    else if (currOP == 0xDE){
        data = bus_read(pc_reg);
        pc_reg++;
        tick();
    }
    else{
        data = read_reg(reg1);
    }

    uint16_t res = a_reg - (data + carry_flag());

    set_flag(res == 0, 7);
    set_flag(1, 6);
    set_flag(( ((int)a_reg & 0xF) -(int)(data & 0xF) - carry_flag()) < 0, 5);
    set_flag(( ((int)a_reg) -(int)(data) - carry_flag()) < 0, 4);  

    a_reg = res; 
    tick();
}

void and(reg reg1, uint8_t currOP){
    uint8_t data = 0;
    if(currOP == 0xA6){
        data = bus_read(read_reg(HL));
        tick();
    }
    else if (currOP == 0xE6){
        data = bus_read(pc_reg);
        pc_reg++;
        tick();
    }
    else{
        data = read_reg(reg1);
    }   

    a_reg &= data;
    set_flag(a_reg == 0, 7);
    set_flag(0, 6);
    set_flag(1, 5);
    set_flag(0, 4);
    tick();
}

void xor(reg reg1, uint8_t currOP){
    uint8_t data = 0;
    if(currOP == 0xAE){
        data = bus_read(read_reg(HL));
        tick();
    }
    else if (currOP == 0xEE){
        data = bus_read(pc_reg);
        pc_reg++;
        tick();
    }
    else{
        data = read_reg(reg1);
    }   

    a_reg ^= (data & 0xFF);
    set_flag(a_reg == 0, 7);
    set_flag(0, 6);
    set_flag(0, 5);
    set_flag(0, 4);
    tick();
}

void or(reg reg1, uint8_t currOP){
    uint8_t data = 0;
    if(currOP == 0xB6){
        data = bus_read(read_reg(HL));
        tick();
    }
    else if (currOP == 0xF6){
        data = bus_read(pc_reg);
        pc_reg++;
        tick();
    }
    else{
        data = read_reg(reg1);
    }   

    a_reg |= (data & 0xFF);
    set_flag(a_reg == 0, 7);
    set_flag(0, 6);
    set_flag(0, 5);
    set_flag(0, 4);
    tick();
}

void cp(reg reg1, uint8_t currOP){
    uint8_t data = 0;
    if(currOP == 0xBE){
        data = bus_read(read_reg(HL));
        tick();
    }
    else if (currOP == 0xFE){
        data = bus_read(pc_reg);
        pc_reg++;
        tick();
    }
    else{
        data = read_reg(reg1);
    }   

    int res = (int)a_reg - (int)data;
    set_flag(res == 0, 7);
    set_flag(1, 6);
    set_flag( (((int)a_reg & 0x0F) - ((int)data)& 0x0F ) < 0, 5);
    set_flag(res < 0, 4);
    tick();
}

reg cb_reg_lookup[] = {B, C, D, E, H, L, HL, A};

void cb(){
    uint8_t cb_op = bus_read(pc_reg);
    pc_reg++;
    tick();
    
    //figuring out info for the cb instr
    reg cb_reg = ((cb_op & 0b111) > 0b111) ? R_NONE : cb_reg_lookup[(cb_op & 0b111)];
    uint8_t bit = (cb_op >> 3) & 0b111;
    uint8_t bit_op = (cb_op >> 6) & 0b11;
    uint8_t reg_val = read_reg(cb_reg);

    if(cb_reg == HL){ tick(); tick(); }
    tick(); //unsure about this tick
    switch(bit_op) {
        case 1:
            //BIT
            set_flag(!(reg_val & (1 << bit)), 7);
            set_flag(0, 6);
            set_flag(1, 5);
            return;
        case 2:
            //RST
            reg_val &= ~(1 << bit);
            set_reg(cb_reg, reg_val);
            return;
        case 3:
            //SET
            reg_val |= (1 << bit);
            set_reg(cb_reg, reg_val);
            return;
    }  

    switch(bit) {
        case 0: {
            //RLC
            bool setC = false;
            uint8_t result = (reg_val << 1) & 0xFF;
            if ((reg_val & (1 << 7)) != 0) {
                result |= 1;
                setC = true;
            }
            set_reg(cb_reg, result);
            set_flag(result == 0, 7);
            set_flag(0, 6);
            set_flag(0, 5);
            set_flag(4, setC);
        } return;

        case 1: {
            //RRC
            uint8_t old = reg_val;
            reg_val >>= 1;
            reg_val |= (old << 7);

            set_reg(cb_reg, reg_val);
            set_flag(!reg_val, 7);
            set_flag(0, 6);
            set_flag(0, 5);
            set_flag(old & 1, 4);
        } return;

        case 2: {
            //RL
            uint8_t old = reg_val;
            reg_val <<= 1;
            reg_val |= carry_flag();
            set_reg(cb_reg, reg_val);
            set_flag(!reg_val, 7);
            set_flag(0, 6);
            set_flag(0, 5);
            set_flag(!!(old & 0x80), 4);
        } return;
        case 3: {
            //RR
            uint8_t old = reg_val;
            reg_val >>= 1;
            reg_val |= (carry_flag() << 7);

            set_reg(cb_reg, reg_val);
            set_flag(!reg_val, 7);
            set_flag(0, 6);
            set_flag(0, 5);
            set_flag(old & 1, 4);
        } return;
        case 4: {
            //SLA
            uint8_t old = reg_val;
            reg_val <<= 1;

            set_reg(cb_reg, reg_val);
            set_flag(!reg_val, 7);
            set_flag(0, 6);
            set_flag(0, 5);
            set_flag(!!(old & 0x80), 4);
        } return;

        case 5: {
            //SRA
            uint8_t u = (int8_t)reg_val >> 1;
            set_reg(cb_reg, u);

            set_flag(!u, 7);
            set_flag(0, 6);
            set_flag(0, 5);
            set_flag(reg_val & 1, 4);            
        } return;

        case 6: {
            //SWAP
            reg_val = ((reg_val & 0xF0) >> 4) | ((reg_val & 0xF) << 4);
            set_reg(cb_reg, reg_val);
            set_flag(reg_val == 0, 7);
            set_flag(0, 6);
            set_flag(0, 5);
            set_flag(0, 4);              
        } return;

        case 7: {
            //SRL
            uint8_t u = reg_val >> 1;
            set_reg(cb_reg, u);

            set_flag(!u, 7);
            set_flag(0, 6);
            set_flag(0, 5);
            set_flag(reg_val & 1, 4);           
        } return;
    }

    //if all else failed
    printf("Error on CB: %02X\n", cb_op);
}

void rlca(){
    uint8_t u = a_reg;
    bool c = (u << 7) & 1;
    u = (u << 1) | c ;
    a_reg = u;
    set_flag(0, 7);
    set_flag(0, 6);
    set_flag(0, 5);
    set_flag(c, 4); 
    tick();           
}

void rrca(){
    uint8_t b = a_reg & 1;
    a_reg >>= 1;
    a_reg |= (b << 7);
    set_flag(0, 7);
    set_flag(0, 6);
    set_flag(0, 5);
    set_flag(b, 4);    
    tick();
}

void rla(){
    uint8_t u = a_reg;
    uint8_t cf = carry_flag();
    uint8_t c = (u >> 7) & 1;
    a_reg = (u << 1) | cf;
    set_flag(0, 7);
    set_flag(0, 6);
    set_flag(0, 5);
    set_flag(c, 4);  
    tick();
}

void rra(){
    uint8_t old_c = carry_flag();
    uint8_t new_c = a_reg & 1;
    a_reg >>= 1;
    a_reg |= (old_c << 7);
    set_flag(0, 7);
    set_flag(0, 6);
    set_flag(0, 5);
    set_flag(new_c, 4);  
    tick();
}

void daa(){
    uint8_t u = 0;
    int fc = 0;

    if (half_carry_flag() || (!add_zero_flag() && (a_reg & 0xF) > 9)) {
        u = 6;
    }

    if (carry_flag() || (!add_zero_flag() && a_reg > 0x99)) {
        u |= 0x60;
        fc = 1;
    }

    a_reg += add_zero_flag() ? -u : u;

    set_flag(a_reg == 0, 7);
    set_flag(0, 5);
    set_flag(fc, 4); 
    tick();
}

void cpl(){ 
    a_reg = ~a_reg;
    set_flag(1, 6);
    set_flag(1, 5);
}

void scf(){
    set_flag(0, 6);
    set_flag(0, 5);
    set_flag(1, 4);    
}

void ccf(){
    set_flag(0, 6);
    set_flag(0, 5);
    set_flag(carry_flag()^1, 4);
}

void halt(){
    cpu.halted = true;
}
/* ----- CPU FUNCTIONALITY ----- */



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
                ld_r_16(instruction->reg_1);
                break;
            case(R_D8):
                ld_r_8(instruction->reg_1);
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
                // if(instruction->reg_2 == A){
                //     ld_mr_r(instruction->reg_1, (uint16_t)0xFF00 + (uint16_t)instruction->reg_2);
                //     break;
                // }

                ld_mr_r(instruction->reg_1, instruction->reg_2);
                break; 
            case(R_MR):
                // if(instruction->reg_1 == A){
                //     ld_mr_r((uint16_t)0xFF00 + (uint16_t)instruction->reg_1,instruction->reg_2);
                //     break;
                // }
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
            uint16_t a16;           
            uint16_t LSB;
            uint16_t MSB;
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
            default:
                printf("No mode exists on LDH instruction\n");            
        }
    }
    else if(instruction->type == INC){
        inc(instruction->reg_1, instruction->mode, cpu.currInstrOpcode );
    }
    else if(instruction->type == DEC){                                             
        dec(instruction->reg_1, instruction->mode, cpu.currInstrOpcode);       
    }
    else if(instruction->type == RLCA){
        rlca();
    }
    else if(instruction->type == ADD){
        add(instruction->reg_1, instruction->reg_2, instruction->mode);
    }
    else if(instruction->type == RRCA){
        rrca();
    }
    else if(instruction->type == STOP){
        
    }
    else if(instruction->type == RLA){
        rla();
    }
    else if(instruction->type == JR){
        jumpr(instruction->cond);
    }
    else if(instruction->type == RRA){
        rra();
    }
    else if(instruction->type == DAA){
        daa();
    }
    else if(instruction->type == CPL){
        cpl();
    }
    else if(instruction->type == SCF){
        scf();
    }
    else if(instruction->type == CCF){
        ccf();
    }
    else if(instruction->type == HALT){
        halt();
    }
    else if(instruction->type == ADC){
        adc(instruction->reg_1, cpu.currInstrOpcode);
    }
    else if(instruction->type == SUB){
        sub(instruction->reg_1, cpu.currInstrOpcode);
    }  
    else if(instruction->type == SBC){
        sbc(instruction->reg_1, cpu.currInstrOpcode);
    }
    else if(instruction->type == AND){
        and(instruction->reg_1, cpu.currInstrOpcode);
    }
    else if(instruction->type == XOR){
        xor(instruction->reg_1, cpu.currInstrOpcode);
    }
    else if(instruction->type == OR){
        or(instruction->reg_1, cpu.currInstrOpcode);
    }
    else if(instruction->type == CP){
        cp(instruction->reg_1, cpu.currInstrOpcode);
    }
    else if(instruction->type == POP){
        pop(instruction->reg_1);
    }
    else if(instruction->type == JP){
        jump(instruction->reg_1, instruction->mode, instruction->cond);
    }
    else if(instruction->type == PUSH){
        push(instruction->reg_1);
        
    }
    else if(instruction->type == RET){
        ret(instruction->cond);
    }
    else if(instruction->type == CB){
        cb();
    }
    else if(instruction->type == CALL){
        call(instruction->reg_1, instruction->mode, instruction->cond);
    }
    else if(instruction->type == RETI){
        reti();
    }
    else if(instruction->type == JPHL){

    }
    else if(instruction->type == DI){
        IME = false;
        tick();
    }
    else if(instruction->type == EI){
        IME = true;
        tick();
    }
    else if(instruction->type == RST){
        rst(instruction->param);
    }
    else if(instruction->type == NOP){
        tick();
    }
}
void fetch_instruction(){
    cpu.currInstrOpcode =  bus_read(pc_reg);
    pc_reg++;
    tick();
    cpu.currInstr = instr_lookup(cpu.currInstrOpcode);
}

/* DEBUGGING */
static char dbg_msg[1024] = {0};
static int  msg_size =  0;

//read instruction and execute, also handle interrupts
bool cpu_step(){
    if(!cpu.halted){
        fetch_instruction();
        printf("Current Instruction: %6s (%02X)  // Registers: A %02X B %02X C %02X D %02X E %02X F (%C%C%C%C) H %02X L %02X PC %04X SP %04X\n"
        ,inst_name(cpu.currInstr->type), cpu.currInstrOpcode, a_reg, b_reg, c_reg, d_reg, e_reg, flag_reg & (1 << 7) ? 'Z' : '-', flag_reg & (1 << 6) ? 'N' : '-', flag_reg & (1 << 5) ? 'H' : '-', flag_reg & (1 << 4) ? 'C' : '-',
        h_reg, l_reg, pc_reg, sp_reg);
        dbg_update();
        dbg_print();

        execute_instruction();

        return true;
    }
    else{
        tick();
        if(ie_reg){
            cpu.halted = false;
        }
    }
    if(IME){
        handle_interrupt();
        
    }

    return false;
}

/* getters and setters*/
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
uint8_t get_ie_reg(){return ie_reg;}
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

void set_ie_reg(uint8_t x){ie_reg = x;}