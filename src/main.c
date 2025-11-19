#include "platforms.h"
#include "drawing/draw.h"
#include "types.h"
#include "math/math.h"
#include "math/fixed_point.h"
#include "io/io.h"
#include "rasterization/rasterization.h"
#include "rasterization/camera.h"
#include "rasterization/chunks.h"
#include "rasterization/object_modification.h"

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
    initChunking();
    
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
    
    // updateCameraTransform(&camera);

    int32 testvar = 16;
    int32 testObjectsAmount = (2*testvar+1)*(2*testvar+1);
    ObjectHandle* testHandles[testObjectsAmount];
    int ch = 0;
    for (int gx = -testvar; gx <= testvar; gx++) {
        for (int gz = -testvar; gz <= testvar; gz++) {

            int32 worldX = fpMul(FP(gx,0), fpDiv(FP(CHUNK_SIZE,0),FP(4,0)));
            int32 worldZ = fpMul(FP(gz,0), fpDiv(FP(CHUNK_SIZE,0),FP(4,0)));

            Transform t = {
                .pos = { .x = worldX, .y = FP(1,0), .z = worldZ },
                .rotation = { .x = 0, .y = 0, .z = 0 },
                .scale = { .x = FP(1,0), .y = FP(1,0), .z = FP(1,0) }
            };

            testHandles[ch] = createObjectInstance(&models[0], t);

            if (ch == 0) {
                print("index in handle:");
                print_dec(testHandles[ch]->index); print("\n");
            }
            ch += 1;
        }
    }

    int currentHandle = 0;
    int ctr = 0;
    print("\nYOOBAGOOBA\n");
    while(1) {

        #if defined(__WINDOWS__)
        SDL_Event event;    
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) return 0;
        }
        #endif

            
            fill(0);
    
            for (int i = 0; i < RENDER_REGION_TOTAL_CHUNKS; ++i) {
                for (int objIdx = 0; objIdx < chunks[i]._backPtr; objIdx += 1) {
                    // print("drawing");
                    drawModel(&chunks[i]._objects[objIdx]);
                }
            }


            camera.position.y = FP(5,0);
            camera.position.x = fpMul(fpSin(fpMul(FP(-ctr,0),FP(0,0x0080))),FP(50,0x8400));
            camera.rotation.y = fpMul(fpSin(fpMul(FP(-ctr,0),FP(0,0x0080))),FP(0,0x8400));
            // camera.rotation.x = fpMul(fpSin(fpMul(FP(-ctr,0),FP(0,0x0080))),FP(0,0x8400));
            updateCameraTransform(&camera);
            centerRenderRegionAtCamera(&camera);

            for (int u = 0; u < 1000; u++) {
                // print("test handle index: "); print_dec(testHandles[u]->index); print("\n");

                if (testHandles[u]->status!=OBJECT_STATUS_ISALIVE) continue;

                Vec3 objPos = testHandles[u]->objectInstance->transform.pos;
                objPos.y = fpMul(fpSin(fpMul(FP(-ctr,0),FP(0,0x00F0))+FP(u,0)),FP(2,0x0000));
                
                // objPos.x = objPos.x + FP(0,0x0800);
                setObjectPosition(testHandles[u], &objPos);
                
                testHandles[u]->objectInstance->transform.rotation.x = fpMul(FP(-ctr,0),FP(0,0x0800))+FP(u,0);
                testHandles[u]->objectInstance->transform.rotation.y = fpMul(FP(-ctr,0),FP(0,0x0400))+FP(u,0);
                testHandles[u]->objectInstance->transform.rotation.z = fpMul(FP(-ctr,0),FP(0,0x0200))+FP(u,0);

                // testHandles[u]->objectInstance->transform.pos.y = fpMul(fpSin(fpMul(FP(-ctr,0),FP(0,0x0080))+FP(u,0)),FP(2,0x0000));
            }
    
            // drawModel(&testObject);
            // testObject.transform.rotation.x += FP(0,0x0400);
            // // testObject.transform.rotation.z += FP(0,0x0200);
            // // testObject.transform.rotation.y += FP(0,0x0100);
            // testObject.transform.pos.x = fpMul(fpSin(fpMul(FP(ctr,0),FP(0,0x0100))),FP(0,0x8000));
            // // testObject.transform.pos.y = fpMul(fpCos(fpMul(FP(ctr,0),FP(0,0x0100))),FP(0,0x8000));
            // testObject.transform.pos.z = fpMul(fpCos(fpMul(FP(ctr,0),FP(0,0x0100))),FP(1,0x0000));
            
            // drawModel(&testObject2);
            // testObject2.transform.rotation.x += FP(0,0x0400);
            // testObject2.transform.rotation.z += FP(0,0x0200);
            // testObject2.transform.rotation.y += FP(0,0x0100);
            // // delay(16000000);
    
            for (int i = 2000000; i > 0; i--) {
                
            }
            ctr += 1;
            swapBuffer();
            clearDepthBuffer();
        
            // if (ctr % 5 == 0 && currentHandle <= 1000) {
            //     destroyObject(testHandles[currentHandle]);
            //     currentHandle += 1;
            //     // moveChunks(1,0);
            //     // print("switch\n");
            // } else if (ctr % 200 == 100) {
            //     // moveChunks(-1,0);
            //     // print("switch_back\n");
            // }

    }

    exitDrawing();

    print("writing done! exiting"); 

    return 0;
}

