// Author: Isac Zobec
// ! Not included in final DTEK-binary !

#if defined(__WINDOWS__)


#include "../types.h"
#include "draw.h"
#include "../io/io.h"
#include "SDL3/SDL.h"
#include <stdlib.h>
#include <stdio.h>
#include "../game/game.h"

uint8* _drawBuffer;
const uint32 PIXEL_BUFFER_X_SIZE = 320;
const uint32 PIXEL_BUFFER_Y_SIZE = 240;

SDL_Window* win;
SDL_Renderer* ren;
SDL_Texture* tex;

GizmoLayer gizmoLayer;
GizmoLayer textLayer;

/* Initialize everything needed to draw pixels to the screen.*/
void initDrawing() {
    SDL_Init(SDL_INIT_VIDEO);
    win = SDL_CreateWindow("DtekGAME",1920, 1080, 0);
    ren = SDL_CreateRenderer(win, NULL);
    tex = SDL_CreateTexture(
        ren, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STREAMING, 
        PIXEL_BUFFER_X_SIZE, PIXEL_BUFFER_Y_SIZE);
    SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);

    _drawBuffer = (uint8*)malloc(PIXEL_BUFFER_X_SIZE * PIXEL_BUFFER_Y_SIZE * sizeof(uint8));

    print("drawing initialized!");

    initGizmoLayer(&gizmoLayer);
    initGizmoLayer(&textLayer);
}

/* Draw a pixel to `x,y`, coloring it `rgb`.*/
// void draw(uint16 x, uint16 y, uint8 rgb) {
//     int offset = x + y * PIXEL_BUFFER_X_SIZE;
//     *(_drawBuffer + offset) = rgb;
// }

/* swap front and back buffers. */
void swapBuffer() {
    SDL_UpdateTexture(tex, NULL, _drawBuffer, PIXEL_BUFFER_X_SIZE * sizeof(uint8));
    SDL_RenderClear(ren);
    SDL_RenderTexture(ren, tex, NULL, NULL);
    SDL_RenderPresent(ren);
}

void exitDrawing() {
    // free the draw buffer
    free(_drawBuffer);
}

void initGizmoLayer(GizmoLayer* layer) {
    // set all pixel colors in the gizmo stack to the 'transparent color'
    layer->gizmoTopPtr = 0;
}
void clearGizmoLayer(GizmoLayer* layer) {
    for (int i = layer->gizmoTopPtr - 1; i >= 0; i--) {
        _drawBuffer[layer->gizmoStack[i].index] = layer->gizmoStack[i].prevColor;
    }
    layer->gizmoTopPtr = 0;
}
#endif