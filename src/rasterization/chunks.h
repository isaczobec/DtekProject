#pragma once
#include "../math/fixed_point.h"
#include "../math/math.h"
#include "../rasterization/rasterization.h"

#define MAX_OBJECTS_GLOBAL 1024

#define CHUNK_SIZE 128
#define MAX_OBJECTS_PER_CHUNK 128
#define AMOUNT_CHUNKS_SQRT ((1 << 16)/CHUNK_SIZE) // 2^16 is largest fixed point integer we can store
#define AMOUNT_CHUNKS (AMOUNT_CHUNKS_SQRT*AMOUNT_CHUNKS_SQRT)

#define RENDER_DISTANCE_CHUNKS (1)
#define RENDER_REGION_SIDE_LENGTH_CHUNKS (RENDER_DISTANCE_CHUNKS*2+1)
#define TOTAL_CHUNKS (RENDER_REGION_SIDE_LENGTH_CHUNKS*RENDER_REGION_SIDE_LENGTH_CHUNKS)

#define OBJECT_MAP_SIZE (AMOUNT_CHUNKS*MAX_OBJECTS_PER_CHUNK)


extern ObjectInstance _objectsDense[MAX_OBJECTS_GLOBAL];
extern int32 _objectsDenseBackPtr; // back of the objects map

extern ObjectHandle _objectHandles[MAX_OBJECTS_GLOBAL];
extern int32 _objectsMapBackPtr; // back of the objects map

typedef struct {
    ObjectInstance _objects[MAX_OBJECTS_PER_CHUNK];

    /* first free index into `_objects` */
    int16 _backPtr;
} Chunk;

extern Chunk chunks[TOTAL_CHUNKS];

ObjectHandle* createObjectInstance(Model* model, Transform transform);

/* Moves an object to a chunk, updating its object handle. 
does not clear the object data in its old location. */
void moveObjectToChunk(int16 chunkIndex, ObjectInstance* objectInstance);

int16 getChunkIndex(int16 relX, int16 relY);

void initChunks();