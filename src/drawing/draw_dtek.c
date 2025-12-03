#if defined(__DTEK__)

#include "../types.h"
#include "draw.h"
#include "../../build_dtek/dtekv-lib.h"

/* The current back buffer that pixels 
should be drawn to while rendering a frame.
swap using `swapBuffer().`*/
uint8* _drawBuffer;
const uint32 PIXEL_BUFFER_X_SIZE = 320;
const uint32 PIXEL_BUFFER_Y_SIZE = 240;

// VGA control register
volatile uint32* const VGA_CONTROL = (volatile uint32*)0x04000100;

// memory mapped front and back buffers
volatile uint8* buffer = (volatile uint8*)0x08000000;
volatile uint8* back_buffer = (volatile uint8*)0x08000000 + PIXEL_BUFFER_Y_SIZE * PIXEL_BUFFER_X_SIZE * sizeof(char);

GizmoLayer gizmoLayer;

void initDrawing() {
    print("Hello from DTEK-V board!\n");

    // make the back buffer register point at the back buffer
    *(VGA_CONTROL+1) = (int)back_buffer;
    _drawBuffer = (uint8*)buffer;
    // _drawBuffer = (uint8*)back_buffer;

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

    initGizmoLayer(&gizmoLayer);

}


void swapBuffer() {

    // could not get switching the buffers to work for the life of me
    // would be nice to fix

    // unsigned int buffer_ptr = *(VGA_CONTROL+0);
    // unsigned int back_buffer_ptr = *(VGA_CONTROL+1);
    // print("Buffer address is ");
    // print_hex32(buffer_ptr);
    // print("\nBackBuffer address is ");
    // print_hex32(back_buffer_ptr);
    // print("\n");

    // _drawBuffer = (uint8*)buffer_ptr; // swap which buffer we are drawing to

    // while (*(VGA_CONTROL+3) & 1) {/*if the buffers have not yet swapped, do nothing*/}
    // *(VGA_CONTROL+0) = 0; // request swap

}

void exitDrawing() {
    // we dont need to do anything on the dtek board
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

