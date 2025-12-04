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
    
     Game game = GAMEINIT;
     //game.turn = !game.turn;
     game.positions.StatePerSide[2] -= (uint64)1<<57;
     game.positions.StatePerSide[2] -= (uint64)1<<58;
     game.positions.StatePerSide[2] -= (uint64)1<<59;
     game.positions.StatePerSide[1] -= (uint64)1<<57;
     game.positions.StatePerSide[1] -= (uint64)1<<58;
     game.positions.StatePerSide[1] -= (uint64)1<<59;
     game.positions.StatePerPiece[KNIGHT] -= (uint64)1<<57;
     game.positions.StatePerPiece[BISHOP] -= (uint64)1<<58;
     game.positions.StatePerPiece[QUEEN] -= (uint64)1<<59;


    print_board(game.positions.StatePerSide[2]);
    print_board(check_legal_moves(&game, (uint64)1<<60, 5));

    make_move(&game, (uint64)1<<60, (uint64)1<<58);

    print_board(game.positions.StatePerPiece[ROOK]);
    print_board(game.positions.StatePerPiece[KING]);


     
     
     /*game.positions = POS0;
     game.turn = !game.turn;
     for(int i = 0; i < 4; i++) {
        game.castling[i] = 0;
     }
     game.positions.StatePerPiece[5] += (uint64)1<<63;
     game.positions.StatePerSide[0] += (uint64)1<<63;
     game.positions.StatePerSide[2] += (uint64)1<<63;

     game.positions.StatePerPiece[9] += (uint64)1<<61;
     game.positions.StatePerSide[1] += (uint64)1<<61;
     game.positions.StatePerSide[2] += (uint64)1<<61;
     
     game.positions.StatePerPiece[9] += (uint64)1<<45;
     game.positions.StatePerSide[1] += (uint64)1<<45;
     game.positions.StatePerSide[2] += (uint64)1<<45;

     Game opp_game = game;

     opp_game.turn = !game.turn;

     print_board(game.positions.StatePerSide[2]);
     print_board(check_legal_moves(&game, (uint64)1<<45, 9));
     print_board(check_legal_moves(&opp_game, (uint64)1<<63, 5));

     make_move(&game, (uint64)1<<45, (uint64)1<<53);
     print_board(game.positions.StatePerSide[2]);
     print_board(game.positions.StatePerSide[1]);
     print_board(game.positions.StatePerPiece[9]);
     print_board(check_legal_moves(&game, (uint64)1<<63, 5));
     printf("%d", game.gamestate);*/



    /*gameInit();

    camera.position.y = FP(6,0);
    camera.rotation.x = FP(0,0xC800);

    updateCameraTransform(&camera);
    centerRenderRegionAtCamera(&camera);

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
        
        
            clearGizmoLayer(&gizmoLayer);
            clearDepthBuffer();
            fill(0);
            
            updateCameraTransform(&camera);
            centerRenderRegionAtCamera(&camera);
            for (int i = 0; i < RENDER_REGION_TOTAL_CHUNKS; ++i) {
                for (int objIdx = 0; objIdx < chunks[i]._backPtr; objIdx += 1) {
                    // print("drawing");
                    drawModel(&chunks[i]._objects[objIdx]);
                }
            }

            swapBuffer();
            
            gameState = GAMESTATE_ACTIVE_WAITING_FOR_MOVE;
            while(gameState != GAMESTATE_ACTIVE_WAITING_TO_RENDER) {
                asm volatile ("nop");
                // if (gameState == GAMESTATE_ACTIVE_WAITING_TO_RENDER) {
                //     print("gamestate changed to waiting to render, doing rendering!\n");
                //     break;
                // } else {
                //     print("a");
                // }
            } 

            ctr += 1;

    }*/

    exitDrawing();

    print("writing done! exiting"); 

    return 0;
}

