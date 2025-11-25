#pragma once
#include "../math/math.h"
#include "../types.h"
#include "../math/transforms.h"
#include "../drawing/draw.h"

#define VERTEX_DATA_TOTAL_SIZE_BYTES 8192 // how many bytes are allocated to vertex data for models
#define INDEX_DATA_TOTAL_SIZE_BYTES 8192 // how many bytes are allocated to vertex data for models
#define MODEL_COUNT 7 // how many bytes are allocated to vertex data for models

#define OBJECT_HANDLE_IN_CHUNK 0
#define OBJECT_HANDLE_IN_DENSEARRAY 1

#define OBJECT_STATUS_ISALIVE 0
#define OBJECT_STATUS_ISDESTROYED 1

typedef struct {
    Vec3* vertexBuffer;
    uint32 vertexCount;

    uint16* indexBuffer;
    uint32 indexBufferCount;
} Model;


typedef struct {
    Model* model;
    Transform transform;
    int16 _handleIndex; // the index of the current object handle.
} ObjectInstance;

typedef struct {
    int8 objectLocation; // can be either `OBJECT_HANDLE_IN_CHUNK` or `OBJECT_HANDLE_IN_DENSEARRAY` 
    int32 index;
    int32 chunkIndex;
    ObjectInstance* objectInstance;
    uint8 status;
} ObjectHandle;

extern Model models[MODEL_COUNT];
extern Vec3 vertexData[VERTEX_DATA_TOTAL_SIZE_BYTES]; // a buffer of all vertex data of all models. `Model.vertexBuffer` points to each models data.
extern uint16 indexData[INDEX_DATA_TOTAL_SIZE_BYTES]; // a buffer of all index tuples of all models. `Model.indexBuffer` points to each models data.

extern Vec3 lightVec;
extern int32 minLightLevel;

extern int32 depthBuffer[TOTAL_PIXELS]; // stores the depth of all pixels of the screen.
// sets all pixels of the depth buffer to the max possible value.
void clearDepthBuffer();
int32 getDepth(uint16 i, uint16 j);
void setDepth(uint16 i, uint16 j, int32 depth);

extern ObjectInstance testObject;
extern ObjectInstance testObject2;

void initRasterization();
void drawModel(const ObjectInstance* object);