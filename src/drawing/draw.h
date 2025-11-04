#pragma once
#include "../platforms.h"
#include "../types.h"

/* The current back buffer that pixels 
should be drawn to while rendering a frame.
swap using `swapBuffer().`*/
extern uint8* _drawBuffer;
extern const uint32 PIXEL_BUFFER_X_SIZE;
extern const uint32 PIXEL_BUFFER_Y_SIZE;

/* Initialize everything needed to draw pixels to the screen.*/
void initDrawing();

/* Draw a pixel to `x,y`, coloring it `rgb`.*/
void draw(uint16 x, uint16 y, uint8 rgb);

/* swap front and back buffers. */
void swapBuffer();