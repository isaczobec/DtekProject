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
#include "rasterization/color.h"
#include "chess_engine/chess.h"
#include "game/game.h"
#include "performance_analysis/performance_analysis.h"

// #define __WINDOWS__
#if defined(__WINDOWS__)
#define SDL_DISABLE_OLD_NAMES 0
#include "SDL3/SDL.h"
#include <stdio.h>
#endif

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

    init();

    print("Starting..\n");
    
    print_hex32(multiplyColor(0xFF,0x00008000));

    initMath();
    initDrawing();
    initRasterization();
    initChunking();
    
    // Game game = GAMEINIT;
    // game.positions.StatePerPiece[0] += (uint64)1<<37;
    // game.positions.StatePerSide[0] += (uint64)1<<37;
    // game.positions.StatePerSide[2] += (uint64)1<<37;
    // game.positions.StatePerPiece[10] += (uint64)1<<30;
    // game.positions.StatePerSide[1] += (uint64)1<<30;
    // game.positions.StatePerSide[2] += (uint64)1<<30;

    // print_board(game.positions.StatePerSide[2]);

    // print_board(check_legal_moves(&game, (uint64)1<<37, 0));

    // make_move(&game, (uint64)1<<37, (uint64)1<<30);

    // print_board(game.positions.StatePerSide[2]);


    gameInit();

    camera.position.y = FP(6,0);
    camera.rotation.x = FP(0,0xC800);

    updateCameraTransform(&camera);
    centerRenderRegionAtCamera(&camera);

    int currentHandle = 0;
    int ctr = 0;
    print("\nYOOBAGOOBA\n");
    while(1) {

        poll();

        clearGizmoLayer(&gizmoLayer);
        clearDepthBuffer();
        fill(0);
        
        updateCameraTransform(&camera);
        centerRenderRegionAtCamera(&camera);

        resetCsrCounterVars();
        clearCsrRegs();
        for (int i = 0; i < RENDER_REGION_TOTAL_CHUNKS; ++i) {
            for (int objIdx = 0; objIdx < chunks[i]._backPtr; objIdx += 1) {
                // print("drawing");
                drawModel(&chunks[i]._objects[objIdx]);
            }
        }
        printCsrCounterVars();

        swapBuffer();
        
        gameState = GAMESTATE_ACTIVE_WAITING_FOR_MOVE;
        while(gameState != GAMESTATE_ACTIVE_WAITING_TO_RENDER) {
            int action = poll();
            if (action == 1) {
                return 0;
            }
        }
        ctr += 1;
        } 

        exitDrawing();
        print("writing done! exiting"); 
        return 0;
    }


