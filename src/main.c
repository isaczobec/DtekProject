#include "platforms.h"
#include "drawing/draw.h"
#include "types.h"

void handle_interrupt(unsigned mcause) {}

void delay(unsigned int cycles) {
    for (int i = 0; i < cycles; i++) {
        asm volatile ("nop");
    }
}

void fill(uint8 rgb)
{
    for (uint16 j = 0; j < PIXEL_BUFFER_Y_SIZE; j++)
    {
        for (uint16 i = 0; i < PIXEL_BUFFER_X_SIZE; i++)
        {
            draw(i, j, rgb);
        }
    }
}

int main() {
    int ctr = 0;
    while(1) {

        fill(ctr);
        delay(16000000);
        ctr = (ctr + 1) % 256;
    }

    // print("writing done! exiting"); 

    return 0;
}

