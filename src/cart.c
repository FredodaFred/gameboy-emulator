#include <stdlib.h>
#include "cart.h"

uint8_t *rom_data;
RomHeader* rom_header;
bool cart_loaded = false;
int rom_size;
static const char *ROM_TYPES[] = {
    "ROM ONLY",
    "MBC1",
    "MBC1+RAM",
    "MBC1+RAM+BATTERY",
    "0x04 ???",
    "MBC2",
    "MBC2+BATTERY",
    "0x07 ???",
    "ROM+RAM 1",
    "ROM+RAM+BATTERY 1",
    "0x0A ???",
    "MMM01",
    "MMM01+RAM",
    "MMM01+RAM+BATTERY",
    "0x0E ???",
    "MBC3+TIMER+BATTERY",
    "MBC3+TIMER+RAM+BATTERY 2",
    "MBC3",
    "MBC3+RAM 2",
    "MBC3+RAM+BATTERY 2",
    "0x14 ???",
    "0x15 ???",
    "0x16 ???",
    "0x17 ???",
    "0x18 ???",
    "MBC5",
    "MBC5+RAM",
    "MBC5+RAM+BATTERY",
    "MBC5+RUMBLE",
    "MBC5+RUMBLE+RAM",
    "MBC5+RUMBLE+RAM+BATTERY",
    "0x1F ???",
    "MBC6",
    "0x21 ???",
    "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
};

static const char *LIC_CODE[0xA5] = {
    [0x00] = "None",
    [0x01] = "Nintendo R&D1",
    [0x08] = "Capcom",
    [0x13] = "Electronic Arts",
    [0x18] = "Hudson Soft",
    [0x19] = "b-ai",
    [0x20] = "kss",
    [0x22] = "pow",
    [0x24] = "PCM Complete",
    [0x25] = "san-x",
    [0x28] = "Kemco Japan",
    [0x29] = "seta",
    [0x30] = "Viacom",
    [0x31] = "Nintendo",
    [0x32] = "Bandai",
    [0x33] = "Ocean/Acclaim",
    [0x34] = "Konami",
    [0x35] = "Hector",
    [0x37] = "Taito",
    [0x38] = "Hudson",
    [0x39] = "Banpresto",
    [0x41] = "Ubi Soft",
    [0x42] = "Atlus",
    [0x44] = "Malibu",
    [0x46] = "angel",
    [0x47] = "Bullet-Proof",
    [0x49] = "irem",
    [0x50] = "Absolute",
    [0x51] = "Acclaim",
    [0x52] = "Activision",
    [0x53] = "American sammy",
    [0x54] = "Konami",
    [0x55] = "Hi tech entertainment",
    [0x56] = "LJN",
    [0x57] = "Matchbox",
    [0x58] = "Mattel",
    [0x59] = "Milton Bradley",
    [0x60] = "Titus",
    [0x61] = "Virgin",
    [0x64] = "LucasArts",
    [0x67] = "Ocean",
    [0x69] = "Electronic Arts",
    [0x70] = "Infogrames",
    [0x71] = "Interplay",
    [0x72] = "Broderbund",
    [0x73] = "sculptured",
    [0x75] = "sci",
    [0x78] = "THQ",
    [0x79] = "Accolade",
    [0x80] = "misawa",
    [0x83] = "lozc",
    [0x86] = "Tokuma Shoten Intermedia",
    [0x87] = "Tsukuda Original",
    [0x91] = "Chunsoft",
    [0x92] = "Video system",
    [0x93] = "Ocean/Acclaim",
    [0x95] = "Varie",
    [0x96] = "Yonezawa/s’pal",
    [0x97] = "Kaneko",
    [0x99] = "Pack in soft",
    [0xA4] = "Konami (Yu-Gi-Oh!)"
};

const char *cart_lic_name() {
    if (rom_header->new_lic_code <= 0xA4) {
        return LIC_CODE[rom_header->lic_code];
    }

    return "UNKNOWN OR NEW LIC CODE";
}

const char *cart_type_name() {
    if (rom_header->type <= 0x22) {
        return ROM_TYPES[rom_header->type];
    }

    return "UNKNOWN";
}

const char *ram_size(uint8_t size){
    if(size == 0)
        return "No Ram";
    else if(size == 1)
        return "Unused";
    else if(size == 2)
        return "8 KiB";
    else if(size == 3)
        return "32 KiB";
    else if(size == 4)
        return "128 KiB";
    else if(size == 5)
        return "64 KiB";
    return "Unknown";
}
bool cart_load(char* file_name){
    FILE *fptr;
    fptr = fopen(file_name,"r");

    if(fptr == NULL){
      printf("File not found!\n");   
      exit(1);             
    }
    fseek(fptr, 0, SEEK_END);
    rom_size = ftell(fptr);
    printf("Rom size: %d bytes\n", rom_size);
    rewind(fptr);

    rom_data = malloc(rom_size);
    fread(rom_data, rom_size, 1, fptr);
    fclose(fptr);
    //hexDump("Gameboy Header", rom_data + 0x104, 0x29, 16);
    rom_header = (RomHeader*)(rom_data + 0x100);

    printf("Cartridge Loaded:\n");
    printf("\t Title    : %s\n", rom_header->title);
    printf("\t Type     : %2.2X (%s)\n", rom_header->type, cart_type_name());
    printf("\t ROM Size : %d KB\n", 32 << rom_header->rom_size);
    printf("\t RAM Size : %2.2X (%s)\n", rom_header->ram_size, ram_size(rom_header->ram_size));
    printf("\t LIC Code : %2.2X (%s)\n", rom_header->lic_code, cart_lic_name());
    printf("\t ROM Vers : %2.2X\n", rom_header->version);
    uint16_t x = 0;
    for (uint16_t i=0x0134; i<=0x014C; i++) { x = x - rom_data[i] - 1;}
    printf("\t Checksum : %2.2X (%s)\n", rom_header->checksum, (x & 0xFF) ? "PASSED" : "FAILED");
    cart_loaded = true;
    return true;
}

bool cart_close(){
    free(rom_data);
    return true;
}

uint8_t cart_read(uint16_t addr){
    if(!cart_loaded){
        printf("Tried to read from card without loading it first\n");
        return 0;
    }
    return rom_data[addr];
}

void cart_write(uint16_t addr, uint8_t data){
        if(!cart_loaded){
            printf("Tried to read from card without loading it first\n");
        }
    rom_data[addr] = data;
}

int cart_size(){
    return rom_size;
}