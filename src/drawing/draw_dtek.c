#define __DTEK__
#ifdef __DTEK__

#include "../types.h"
#include "draw.h"

// VGA control register
volatile int* const VGA_CONTROL = (volatile int*)0x04000100;

// memory mapped front and back buffers
volatile char* buffer = (volatile char*)0x08000000;
volatile char* back_buffer = (volatile char*)0x08000000 + PIXEL_BUFFER_Y_SIZE * PIXEL_BUFFER_X_SIZE * sizeof(char);

static uint8* _drawBuffer;

const uint32 PIXEL_BUFFER_X_SIZE = 320;
const uint32 PIXEL_BUFFER_Y_SIZE = 240;

inline void draw(uint16 x, uint16 y, uint8 rgb) {
    int offset = x + y * PIXEL_BUFFER_X_SIZE;
    *(_drawBuffer + offset) = rgb;
}


void swapBuffer() {
    *(VGA_CONTROL+0) = 0; // request swap
}

#endif