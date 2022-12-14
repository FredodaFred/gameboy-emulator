#include "common.h"


char* flagBinaryString(uint8_t f){
    // bool Z = (f >> 7);
    // bool N = (f << 1) >> 7;
    // bool H = (f << 2) >> 7;
    // bool C = (f << 3) >> 7;
    // char* ret = { Z ? 'Z' : '-', N ? 'N' : '-', H ? 'H' : '-', C ? 'C' : '-'};
    // //printf("%s\n", ret);
    return "ret";
}

void hexDump (const char * desc, const void * addr, const int len, int perLine) {
    // Silently ignore silly per-line values.
    if (perLine < 4 || perLine > 64) perLine = 16;

    int i;
    unsigned char buff[perLine+1];
    const unsigned char * pc = (const unsigned char *)addr;

    // Output description if given.

    if (desc != NULL) printf ("%s:\n", desc);

    // Length checks.

    if (len == 0) {
        printf("  ZERO LENGTH\n");
        return;
    }
    if (len < 0) {
        printf("  NEGATIVE LENGTH: %d\n", len);
        return;
    }

    // Process every byte in the data.

    for (i = 0; i < len; i++) {
        // Multiple of perLine means new or first line (with line offset).

        if ((i % perLine) == 0) {
            // Only print previous-line ASCII buffer for lines beyond first.

            if (i != 0) printf ("  %s\n", buff);

            // Output the offset of current line.

            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.

        printf (" %02x", pc[i]);

        // And buffer a printable ASCII character for later.

        if ((pc[i] < 0x20) || (pc[i] > 0x7e)) // isprint() may be better.
            buff[i % perLine] = '.';
        else
            buff[i % perLine] = pc[i];
        buff[(i % perLine) + 1] = '\0';
    }

    // Pad out last line if not exactly perLine characters.

    while ((i % perLine) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII buffer.

    printf ("  %s\n", buff);
}

/* HOW DOES C HANDLE OVERFLOW
    uint8_t a = 0xFF;
    a++;
    printf("0xFF (%d) + 1: %d\n", 0xFF, a);

   THIS GIVES US: 0xFF (255) + 1 =  0
*/