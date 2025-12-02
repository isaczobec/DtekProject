#pragma once
#include "../platforms.h"
#include "../types.h"
#include "../math/fixed_point.h"
#include "../math/math.h"


/* The current back buffer that pixels 
should be drawn to while rendering a frame.
swap using `swapBuffer().`*/
extern uint8* _drawBuffer;
extern const uint32 PIXEL_BUFFER_X_SIZE;
extern const uint32 PIXEL_BUFFER_Y_SIZE;
#define TOTAL_PIXELS 320*240
#define ASPECT_RATIO fpDiv(FP(PIXEL_BUFFER_X_SIZE,0), FP(PIXEL_BUFFER_Y_SIZE,0))

typedef struct {
    uint32 index;
    uint8 prevColor;
} GizmoPixel;

#define GIZMOLAYER_NO_PREVIOUS
typedef struct {
    GizmoPixel gizmoStack[TOTAL_PIXELS];
    uint16 gizmoTopPtr;
} GizmoLayer;

extern GizmoLayer gizmoLayer;

/* Initialize everything needed to draw pixels to the screen.*/
void initDrawing();

/* Draw a pixel to `x,y`, coloring it `rgb`.*/
void draw(uint16 x, uint16 y, uint8 rgb);

/* swap front and back buffers. */
void swapBuffer();

/* free all resources associated with drawing. */
void exitDrawing();


void initGizmoLayer(GizmoLayer* layer);
void drawToGizmoLayer(GizmoLayer* layer, uint16 x, uint16 y, uint8 color);
void clearGizmoLayer(GizmoLayer* layer);