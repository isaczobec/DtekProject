#include "platforms.h"
#include "draw.h"

volatile int* const VGA_CONTROL = (volatile int*)0x04000100;

#define PIXEL_BUFFER_X_SIZE 320
#define  PIXEL_BUFFER_Y_SIZE 240

volatile char* buffer = (volatile char*)0x08000000;
volatile char* back_buffer = (volatile char*)0x08000000 + PIXEL_BUFFER_Y_SIZE * PIXEL_BUFFER_X_SIZE * sizeof(char);
// char back_buffer_arr[PIXEL_BUFFER_Y_SIZE * PIXEL_BUFFER_X_SIZE * sizeof(char)];
// char* back_buffer = back_buffer_arr;

void handle_interrupt(unsigned mcause) {}

void delay(unsigned int cycles) {
    for (int i = 0; i < cycles; i++) {
        asm volatile ("nop");
    }
}

void fill(char rgb, char* write_to_buffer)
{
    for (int j = 0; j < PIXEL_BUFFER_Y_SIZE; j++)
    {
        for (int i = 0; i < PIXEL_BUFFER_X_SIZE; i++)
        {

            int offset = i + j * PIXEL_BUFFER_X_SIZE;

            // // set red
            // *(back_buffer + offset) &= ~0xE0;
            // *(back_buffer + offset) |= (i / (320 / 8)) << 5;

            // // set green
            // *(back_buffer + offset) &= ~0x1C;
            // *(back_buffer + offset) |= (j / (240 / 8)) << 2;

            // set to rgb
            *(write_to_buffer + offset) = rgb;
        }
    }
}

int main() {
    
    print("Hello from DTEK-V board!\n");

    // make the back buffer register point at the back buffer
    *(VGA_CONTROL+1) = (int)back_buffer;

    unsigned int buffer_ptr = *(VGA_CONTROL+0);
    unsigned int back_buffer_ptr = *(VGA_CONTROL+1);
    print("Buffer address is ");
    print_hex32(buffer_ptr);
    print("\nBackBuffer address is ");
    print_hex32(back_buffer_ptr);
    print("\n");



    unsigned int screen_res_x =  *(VGA_CONTROL+2) & 0x0000FFFF;
    unsigned int screen_res_y = (*(VGA_CONTROL+2) & 0xFFFF0000) >> 16;
    print("Screen resolution is (");
    print_dec(screen_res_x);
    print(", ");
    print_dec(screen_res_y);
    print(").");


        // **FIX: Initialize both frame buffers to a known color (e.g., black/0x00)**
    // The first buffer (front buffer initially)
    fill(0x00, (char*)buffer_ptr); 
    // The second buffer (back buffer initially)
    fill(0x00, (char*)back_buffer_ptr); 


    // for (int j = 0; j < PIXEL_BUFFER_Y_SIZE; j++)
    // {
    //     for (int i = 0; i < PIXEL_BUFFER_X_SIZE; i++)
    //     {

    //         int offset = i + j * PIXEL_BUFFER_X_SIZE;

    //         // set red
    //         // *(back_buffer + offset) &= ~0xE0;
    //         // *(back_buffer + offset) |= (i / (320 / 8)) << 5;

    //         // set green
    //         // *(back_buffer + offset) &= ~0x1C;
    //         // *(back_buffer + offset) |= (j / (240 / 8)) << 2;
    //         *(back_buffer + offset) = 0xF0;
    //         // delay(30000);
    //     }
    // }

    int ctr = 0;
    while(1) {

        unsigned int buffer_ptr = *(VGA_CONTROL+0);
        unsigned int back_buffer_ptr = *(VGA_CONTROL+1);
        print("Buffer address is ");
        print_hex32(buffer_ptr);
        print("\nBackBuffer address is ");
        print_hex32(back_buffer_ptr);
        print("\n");

        fill(ctr, (char*)back_buffer_ptr);

        *(VGA_CONTROL+0) = 0; // request swap
        
        while (*(VGA_CONTROL+3) & 1) {/*do nothing while the swap is performing*/}

        delay(16000000);
        ctr = (ctr + 1) % 256;
    }

    /*
    do a write to the `Buffer` register of the VGA, which is interpreted
    as a request to copy the backbuffer into the main buffer and display it
    */ 

    print("writing done! exiting"); 
    
    // free the back buffer and return
    return 0;
}

