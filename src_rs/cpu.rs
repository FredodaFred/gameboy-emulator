mod bus
use bus::*

enum Register { A, F, AF, B, C, BC, D, E, DE, H, L, HL, SP, PC }

struct Registers{
    a: u8, f: u8,
    b: u8, c: u8,
    d: u8, e: u8,
    h: u8, l: u8,
    sp: u16,
    pc: u16
}
impl Registers {
    fn init(&self){
        self.a = 0x01
        self.f = 0xB0
        self.b = 0x00
        self.c = 0x13
        self.d = 0x00
        self.e = 0xD8
        self.h = 0x01
        self.l = 0x4D
        self.sp = 0xFFFE
        self.pc = 0x0100
    }
    //flag getters
    fn z(&self) -> bool { return self.f >> 7}
    fn n(&self) -> bool { return (self.f << 1) >> 7}
    fn h(&self) -> bool { return (self.f << 2) >> 7}
    fn c(&self) -> bool { return (self.f << 3) >> 7}

    fn get_reg(&self, reg: Register) -> u16{
        let val: u16 = match reg {
            A => self.a,
            F => self.f,
            B => self.b,
            C => self.c,
            D => self.d,
            E => self.e,
            H => self.h,
            L => self.l,
            SP => self.sp, 
            PC => self.pc,
            AF => (self.a << 2) | self.f, 
            BC => (self.b << 2) | self.c, 
            DE => (self.d << 2) | self.e, 
            HL => (self.h << 2) | self.l, 
            _ => println!("Invalid Register!"),
        }
        return val
    }

}

