#include "common.h"
#include "cart.h"
#include "emu.h"



int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: emu <rom_file>\n");
        return -1;
    }

    if (!cart_load(argv[1])) {
        printf("Failed to load ROM file: %s\n", argv[1]);
        exit(-2);
    }

    emu_begin();

    cart_close();
}

