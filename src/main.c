#include "platforms.h"
#include "drawing/draw.h"
#include "types.h"
#include "math/math.h"
#include "math/fixed_point.h"
#include "io/io.h"
#include "rasterization/rasterization.h"

// #define __WINDOWS__
#if defined(__WINDOWS__)
#define SDL_DISABLE_OLD_NAMES 0
#include "SDL3/SDL.h"
#include <stdio.h>
#endif

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
            if (i % 2 == 0)
            draw(i, j, rgb);
        }
    }
}

int main() {

    initMath();
    initDrawing();

    print("pi is ");
    print_dec(pi);
    print("\n");
    
    Vec3 i = {
        .x = fpInt(-2,0),
        .y = fpInt(-2,0),
        .z = fpInt(-2,0),
    };
    Vec3 j = {
        .x = fpInt(2,0),
        .y = fpInt(2,0),
        .z = fpInt(2,0),
    };

    int32 res;
    Vec3 resVector;

    dotProduct(&i, &j, &res);
    addVector(&i, &j, &resVector);
    print_dec(res);
    print_dec(resVector.x);
    print_dec(resVector.y);
    print_dec(resVector.z);

    int ctr = 0;
    while(1) {

        #if defined(__WINDOWS__)
        SDL_Event event;    
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) return 0;
        }
        #endif

        fill(ctr);
        drawModel(models);
        // delay(16000000);

        for (int i = 200000000; i > 0; i--) {
            
        }
        ctr = (ctr + 1) % 256;
        swapBuffer();
    }

    exitDrawing();

    // print("writing done! exiting"); 

    return 0;
}

