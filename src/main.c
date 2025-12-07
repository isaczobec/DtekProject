// Author: Isac Zobec & Linus Bredin
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
#include "rasterization/font.h"

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
    init_glyphs();
    fill(0);

    
    // each iteration in this loop is one full game
    while (1) {
        
            camera.position.x = FP(0,0);
            camera.position.y = FP(0,0);
            camera.position.z = FP(0,0);
            camera.rotation.x = FP(0,0x0000);
            camera.rotation.y = FP(0,0x0000);
            camera.rotation.z = FP(0,0x0000);
            updateCameraTransform(&camera);
            centerRenderRegionAtCamera(&camera);
        
            ObjectHandle* menuObject = createObjectInstance(m_queen, (Transform){
                .pos = {FP(30,0), N_FP(35,0), FP(80,0)},
                .rotation = {FP(0,0x0000), FP(0,0), N_FP(0,0x8000)},
                .scale = {FP(40,0), FP(40,0), FP(40,0)}
            }, 0x3F);
            drawModel(menuObject->objectInstance);
            draw_string("DTEK CHESS", 30, 120, 0x3F);
            draw_string("PRESS BUTTON TO PLAY", 30, 130, 0xFF);
            swapBuffer();
            
            while (1) {
                int action = poll();
                if (action == 1) return 0;
                InputState state = getInputState();
                if (state.pressedButton) {
                    break;
                }
            }
            
            // delete the object and text and create select color menu
            destroyObject(menuObject);
            fill(0);
            clearGizmoLayer(&textLayer);
            swapBuffer();
        
            draw_string("SELECT PLAYER 1 COLOR", 30, 120, 0xFF);
            draw_string("USE SWITCHES 0-7 AND THEN PRESS BUTTON", 30, 130, 0xFF);
            
            uint8 player1color;
        
            while (1) {
                int action = poll();
                if (action == 1) return 0;
                InputState state = getInputState();
        
                player1color = (uint8)state.switches;
        
                clearGizmoLayer(&gizmoLayer);
                for (uint16 i = 50; i < 100; i++) {
                    for (uint16 j = 50; j < 100; j++) {
                        drawToGizmoLayer(&gizmoLayer, i, j, player1color);
                    }
                }
                swapBuffer();
        
                if (state.pressedButton) {
                    break;
                }
            }
        
            // now select player 2 color
            clearGizmoLayer(&textLayer);
            clearGizmoLayer(&gizmoLayer);
        
            draw_string("SELECT PLAYER 2 COLOR", 30, 120, 0xFF);
            draw_string("USE SWITCHES 0-7 AND THEN PRESS BUTTON", 30, 130, 0xFF);
            
            uint8 player2color;
        
            while (1) {
                int action = poll();
                if (action == 1) return 0;
                InputState state = getInputState();
        
                player2color = (uint8)state.switches;
        
                clearGizmoLayer(&gizmoLayer);
                for (uint16 i = 50; i < 100; i++) {
                    for (uint16 j = 50; j < 100; j++) {
            
                        drawToGizmoLayer(&gizmoLayer, i, j, player2color);
                    }
                }
                swapBuffer();
        
                if (state.pressedButton) {
                    break;
                }
            }
        
            clearGizmoLayer(&textLayer);
            clearGizmoLayer(&gizmoLayer);
        
            gameInit(player1color, player2color);
            
            camera.position.y = FP(6,0);
            camera.rotation.x = FP(0,0xC400);
        
            updateCameraTransform(&camera);
            centerRenderRegionAtCamera(&camera);
        
            
            int ctr = 0;
            print("\nYOOBAGOOBA\n");
            while(1) {
        
                poll();
        
                clearGizmoLayer(&gizmoLayer);
                clearDepthBuffer();
                fill(0);
                
                updateCameraTransform(&camera);
                centerRenderRegionAtCamera(&camera);
        

                for (int i = 0; i < RENDER_REGION_TOTAL_CHUNKS; ++i) {
                    for (int objIdx = 0; objIdx < chunks[i]._backPtr; objIdx += 1) {
                        // print("drawing");
                        if (chunks[i]._objects[objIdx].objectInstanceFlags & OBJECTINSTANCE_RENDER_TO_GIZMO_LAYER) 
                            continue;
                        drawModel(&chunks[i]._objects[objIdx]);
                    }
                }
                    swapBuffer();
                setStateToWaitingForInput(1);
                swapBuffer();

                

                while(gameState != GAMESTATE_ACTIVE_WAITING_TO_RENDER) {
                    int action = poll();
                    if (action == 1) {
                        return 0;
                    }
                }
                ctr += 1;
                
                if (gameStatus == GAME_STATUS_OVER) {
                    print("Game is over!\n");
                    // get the input state to clear the button flag
                    getInputState();
                    
                    // wait for button press, then reset
                    while (1) {
                        int action = poll();
                        if (action == 1) return 0;
                        InputState state = getInputState();
                        if (state.pressedButton) {
                            break;
                        }
                    }
                    break;
                }

            } 
        
            // clean up canvas and destory pieces
            DestroyPieces();
            clearDepthBuffer();
            clearGizmoLayer(&textLayer);
            clearGizmoLayer(&gizmoLayer);
            fill(0);
    }
}


