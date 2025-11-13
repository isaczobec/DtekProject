#include "platforms.h"
#include "drawing/draw.h"
#include "types.h"
#include "math/math.h"
#include "math/fixed_point.h"
#include "io/io.h"
#include "rasterization/rasterization.h"
#include "rasterization/camera.h"

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
            // if (i % 2 == 0)
            draw(i, j, rgb);
        }
    }
}

int main() {

    initMath();
    initDrawing();
    initRasterization();

    print("got past init");

    print("pi is ");
    print_dec(pi);
    print("\n");

    print("fact is ");
    print_dec(fpFactorial(FP(4,0)));
    print("\n");
    print("exp is ");
    print_dec(fpPow(FP(4,0),FP(4,0)));
    print("\n");
    
    print("sin is ");
    print("\n");
    print_dec(fpSin(fpDiv(fpMul(pi,FP(-1,0)),FP(1,0))));print("\n");
    print_dec(fpSin(fpDiv(fpMul(pi,FP(2,0)),FP(3,0))));print("\n");
    print_dec(fpSin(fpDiv(fpMul(pi,FP(3,0)),FP(4,0))));print("\n");
    print_dec(fpSin(fpDiv(fpMul(pi,FP(-1,0)),FP(6,0))));print("\n");
    print("\n");
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

    print("DSMAdkasp");
    print("entering loop");
    
    // updateCameraTransform(&camera);

    int ctr = 0;
    while(1) {

        #if defined(__WINDOWS__)
        SDL_Event event;    
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) return 0;
        }
        #endif

        fill(0);
        drawModel(&testObject);
        testObject.transform.rotation.x += FP(0,0x0400);
        // testObject.transform.rotation.z += FP(0,0x0200);
        // testObject.transform.rotation.y += FP(0,0x0100);
        testObject.transform.pos.x = fpMul(fpSin(fpMul(FP(ctr,0),FP(0,0x0100))),FP(0,0x8000));
        // testObject.transform.pos.y = fpMul(fpCos(fpMul(FP(ctr,0),FP(0,0x0100))),FP(0,0x8000));
        testObject.transform.pos.z = fpMul(fpCos(fpMul(FP(ctr,0),FP(0,0x0100))),FP(1,0x0000));
        
        drawModel(&testObject2);
        testObject2.transform.rotation.x += FP(0,0x0400);
        testObject2.transform.rotation.z += FP(0,0x0200);
        testObject2.transform.rotation.y += FP(0,0x0100);
        // delay(16000000);

        for (int i = 200000; i > 0; i--) {
            
        }
        ctr += 1;
        swapBuffer();
        clearDepthBuffer();
    }

    exitDrawing();

    print("writing done! exiting"); 

    return 0;
}

