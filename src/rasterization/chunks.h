#pragma once
#include "../math/fixed_point.h"
#include "../math/math.h"
#include "../rasterization/rasterization.h"

#define MAX_OBJECTS_GLOBAL 4096

#define CHUNK_SIZE 32
#define MAX_OBJECTS_PER_CHUNK 64
#define AMOUNT_CHUNKS_SQRT ((1 << 16)/CHUNK_SIZE) // 2^16 is largest fixed point integer we can store
#define AMOUNT_CHUNKS (AMOUNT_CHUNKS_SQRT*AMOUNT_CHUNKS_SQRT)

#define RENDER_DISTANCE_CHUNKS (5)
#define RENDER_REGION_SIDE_LENGTH_CHUNKS (RENDER_DISTANCE_CHUNKS*2+1)
#define TOTAL_CHUNKS (RENDER_REGION_SIDE_LENGTH_CHUNKS*RENDER_REGION_SIDE_LENGTH_CHUNKS)

#define RENDER_REGION_TOTAL_CHUNKS (RENDER_REGION_SIDE_LENGTH_CHUNKS*RENDER_REGION_SIDE_LENGTH_CHUNKS)

// TODO: Optimize this for faster searching?
extern ObjectInstance _objectsDense[MAX_OBJECTS_GLOBAL];
extern int32 _objectsDenseBackPtr; // back of the objects map

extern ObjectHandle _objectHandles[MAX_OBJECTS_GLOBAL];
// extern int32 _objectHandlesBackPtr; // back of the objects map

extern int32 totalObjects;

typedef struct {
    ObjectInstance _objects[MAX_OBJECTS_PER_CHUNK];

    /* first free index into `_objects` */
    int16 _backPtr;
} Chunk;

extern Chunk chunks[TOTAL_CHUNKS];
extern int32 chunkDisplacementX; // how many chunk steps we have displaced the render region on the x axis
extern int32 chunkDisplacementY; // how many chunk steps we have displaced the render region on the y axis

/*
Creates an object with the given model and transform.
Returns a pointer to an object handle, which is used
to refernce the object (it is passed to relevant functions).
sets the handles status to `OBJECT_STATUS_ALIVE`
*/
ObjectHandle* createObjectInstance(Model* model, Transform transform);

/*
Destroys an object associated with the given objectHandle.
Sets the handles status to `OBJECT_STATUS_ISDESTROYED`. 
*/
void destroyObject(ObjectHandle* objectHandle);

/* Moves an object to a chunk, updating its object handle. 
does not clear the object data in its old location. */
void moveObjectToChunk(int16 chunkIndex, ObjectInstance* objectInstance);

/* Moves an object from a chunk to the dense array, updating its object handle. 
does not clear the object data in its old location. */
void moveObjectToDenseArray(ObjectInstance* objectInstance);

/*
 removes an object from the dense array, but does not delete
 its object handle.
 copies over the back element and changes that objects handle.
*/
int removeObjectFromChunk(int16 chunkIndex, int16 objectIndex);

/*
 removes an object from the dense array, but does not delete
 its object handle.
 copies over the back element and changes that objects handle.
*/
int removeObjectFromDenseArray(int16 objectIndex);

int16 getChunkIndexFromIJ(int16 relX, int16 relY);

int16 getChunkIndexFromWorldXY(int16 x, int16 y);

void moveChunks(int16 x, int16 y);

int32 worldPosToChunkCoord(int32 worldX);

uint8 isInRenderDistance(int32 chunkX, int32 chunkY);

void initChunking();