#pragma once
#include "../math/math.h"
#include "../types.h"
#include "../math/transforms.h"
#include "../drawing/draw.h"

#define VERTEX_DATA_TOTAL_SIZE_BYTES 2048 // how many bytes are allocated to vertex data for models
#define INDEX_DATA_TOTAL_SIZE_BYTES 2048 // how many bytes are allocated to vertex data for models
#define MODEL_COUNT 1 // how many bytes are allocated to vertex data for models

typedef struct {
    Vec3* vertexBuffer;
    uint32 vertexCount;

    uint16* indexBuffer;
    uint32 indexBufferCount;
} Model;

typedef struct {
    Model* model;
    Transform transform;
} ObjectInstance;

extern Model models[MODEL_COUNT];
extern Vec3 vertexData[VERTEX_DATA_TOTAL_SIZE_BYTES]; // a buffer of all vertex data of all models. `Model.vertexBuffer` points to each models data.
extern uint16 indexData[INDEX_DATA_TOTAL_SIZE_BYTES]; // a buffer of all index tuples of all models. `Model.indexBuffer` points to each models data.

extern int32 depthBuffer[TOTAL_PIXELS]; // stores the depth of all pixels of the screen.
// sets all pixels of the depth buffer to the max possible value.
void clearDepthBuffer();
int32 getDepth(uint16 i, uint16 j);
void setDepth(uint16 i, uint16 j, int32 depth);

extern ObjectInstance testObject;

void initRasterization();
void drawModel(const ObjectInstance* object);