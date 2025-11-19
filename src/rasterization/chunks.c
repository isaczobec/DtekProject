#include "chunks.h"

ObjectHandle _objectHandles[MAX_OBJECTS_GLOBAL];
int32 _objectHandlesBackPtr = 0; // back of the objects map

ObjectInstance _objectsDense[MAX_OBJECTS_GLOBAL];
int32 _objectsDenseBackPtr = 0; // back of the objects map

int32 totalObjects = 0;

Chunk chunks[TOTAL_CHUNKS];
int32 chunkDisplacementX = 0; // how many chunk steps we have displaced the render region on the x axis
int32 chunkDisplacementY = 0; // how many chunk steps we have displaced the render region on the y axis

int16 getChunkIndexFromIJ(int16 relX, int16 relY) {
    return relY * RENDER_REGION_SIDE_LENGTH_CHUNKS + relX;
}




int16 getChunkIndexingCoord(int16 chunkWorldCoord) {
    int16 mod = chunkWorldCoord % RENDER_REGION_SIDE_LENGTH_CHUNKS;
    if (mod < 0) mod += RENDER_REGION_SIDE_LENGTH_CHUNKS; // guard agains negative modulos
    return mod;
}

int16 getChunkIndexFromWorldXY(int16 x, int16 y) {
    return getChunkIndexingCoord(y) * RENDER_REGION_SIDE_LENGTH_CHUNKS + getChunkIndexingCoord(x);
}

uint8 isInRenderDistance(int32 chunkX, int32 chunkY) {
    return chunkX <= chunkDisplacementX + RENDER_DISTANCE_CHUNKS
        && chunkX >= chunkDisplacementX - RENDER_DISTANCE_CHUNKS
        && chunkY <= chunkDisplacementY + RENDER_DISTANCE_CHUNKS
        && chunkY >= chunkDisplacementY - RENDER_DISTANCE_CHUNKS;
}

ObjectHandle* createObjectInstance(Model* model, Transform transform) {

    // copy the values to the dense objects array
    if (_objectsDenseBackPtr >= MAX_OBJECTS_GLOBAL) {
        print("Object limit exceeded!");
        return (ObjectHandle*) NULL;
    }

    // check if the created object should go in a chunk or the dense array
    int32 chunkX = worldPosToChunkCoord(transform.pos.x);
    int32 chunkY = worldPosToChunkCoord(transform.pos.z);

    // print("\n coords: \n");
    // print_dec(chunkX);
    // print("\n");
    // print_dec(chunkY);
    // print("\n");

    ObjectHandle* handle;
    if (isInRenderDistance(chunkX, chunkY)) {
        // the case where we add it directly to a chunk


        // get the chunk index
        int32 chunkIndexingX = getChunkIndexingCoord(chunkX);
        int32 chunkIndexingY = getChunkIndexingCoord(chunkY);
        int32 chunkIndex = getChunkIndexFromIJ(chunkIndexingX, chunkIndexingY);

        if (chunks[chunkIndex]._backPtr >= MAX_OBJECTS_PER_CHUNK) {
            print("\nObject limit exceeded!\n");
            return (ObjectHandle*) NULL;
        }
        
        chunks[chunkIndex]._objects[chunks[chunkIndex]._backPtr].model = model;
        chunks[chunkIndex]._objects[chunks[chunkIndex]._backPtr].transform = transform;
        chunks[chunkIndex]._backPtr += 1;

        _objectHandles[_objectHandlesBackPtr].objectInstance = &chunks[chunkIndex]._objects[chunks[chunkIndex]._backPtr-1];
        _objectHandles[_objectHandlesBackPtr].objectLocation = OBJECT_HANDLE_IN_CHUNK;
        _objectHandles[_objectHandlesBackPtr].index = chunks[chunkIndex]._backPtr-1;
        _objectHandles[_objectHandlesBackPtr].chunkIndex = chunkIndex;
        handle = &_objectHandles[_objectHandlesBackPtr];
        _objectHandlesBackPtr += 1;
        
        // set the object handle pointer
        chunks[chunkIndex]._objects[chunks[chunkIndex]._backPtr-1]._handleIndex = _objectHandlesBackPtr-1;
        

    } else {
        // copy the values to the dense objects array
        _objectsDense[_objectsDenseBackPtr].model = model;
        _objectsDense[_objectsDenseBackPtr].transform = transform;
        _objectsDenseBackPtr += 1;
        
        // create an object handle
        _objectHandles[_objectHandlesBackPtr].objectInstance = &_objectsDense[_objectsDenseBackPtr-1];
        _objectHandles[_objectHandlesBackPtr].objectLocation = OBJECT_HANDLE_IN_DENSEARRAY;
        _objectHandles[_objectHandlesBackPtr].index = _objectsDenseBackPtr-1;
        handle = &_objectHandles[_objectHandlesBackPtr];
        _objectHandlesBackPtr += 1;
    
        // set the object handle pointer
        _objectsDense[_objectsDenseBackPtr-1]._handleIndex = _objectHandlesBackPtr-1;
    }

    totalObjects += 1;
    return handle;

}

void moveObjectToChunk(int16 chunkIndex, ObjectInstance* objectInstance) {
    chunks[chunkIndex]._objects[chunks[chunkIndex]._backPtr] = *objectInstance;
    chunks[chunkIndex]._backPtr += 1;

    // update what the handle is pointing to
    ObjectHandle* handle = &_objectHandles[objectInstance->_handleIndex];
    handle->objectInstance = &chunks[chunkIndex]._objects[chunks[chunkIndex]._backPtr-1];
    handle->objectLocation = OBJECT_HANDLE_IN_CHUNK;
    handle->chunkIndex = chunkIndex;
    handle->index = chunks[chunkIndex]._backPtr - 1;

}

void moveObjectToDenseArray(ObjectInstance* objectInstance) {
    _objectsDense[_objectsDenseBackPtr] = *objectInstance;
    _objectsDenseBackPtr += 1;
    
    // update what the handle is pointing to
    ObjectHandle* handle = &_objectHandles[objectInstance->_handleIndex];
    handle->objectInstance = &_objectsDense[_objectsDenseBackPtr-1];
    handle->objectLocation = OBJECT_HANDLE_IN_DENSEARRAY;
    handle->index = _objectsDenseBackPtr - 1;

}  

/*
 removes an object from the dense array, but does not delete
 its object handle.
 copies over the back element and changes that objects handle.
*/
void removeObjectFromDenseArray(int16 objectIndex) {
    _objectsDense[objectIndex] = _objectsDense[_objectsDenseBackPtr-1];
    _objectsDenseBackPtr -= 1;

    // change the copied back objects handle
    if (_objectsDenseBackPtr > 0) {
        ObjectHandle* handle = &_objectHandles[_objectsDense[objectIndex]._handleIndex];
        handle->objectInstance = &_objectsDense[objectIndex];
        handle->index = objectIndex;
    }
}

/*
 removes an object from the dense array, but does not delete
 its object handle.
 copies over the back element and changes that objects handle.
*/
void removeObjectFromChunk(int16 chunkIndex, int16 objectIndex) {
    chunks[chunkIndex]._objects[objectIndex] = chunks[chunkIndex]._objects[chunks[chunkIndex]._backPtr-1];
    chunks[chunkIndex]._backPtr -= 1;

    // change the copied back objects handle
    if (chunks[chunkIndex]._backPtr > 0) {
        ObjectHandle* handle = &_objectHandles[chunks[chunkIndex]._objects[objectIndex]._handleIndex];
        handle->objectInstance = &chunks[chunkIndex]._objects[objectIndex];
        handle->index = objectIndex;
    }
}



int32 worldPosToChunkCoord(int32 worldPos) {
    int32 worldChunk = fpDiv(worldPos, FP(CHUNK_SIZE,0));
    worldChunk = fpSub(worldChunk,FP(0,0x8000)); // subtract 1/2
    int32 chunkPos = FP_TOINT(worldChunk) + 1; // throw away decimal part
    // print("\nchunkpos is\n");
    // print_dec(chunkPos);
    return chunkPos;
}

void processMovingChunk(int16 chunkX, int16 chunkY)
{
    int16 chunkToMoveI = (chunkX) % RENDER_REGION_SIDE_LENGTH_CHUNKS;
    if (chunkToMoveI < 0)
        chunkToMoveI += RENDER_REGION_SIDE_LENGTH_CHUNKS; // guard against negative modulo values
    int16 chunkToMoveJ = (chunkY) % RENDER_REGION_SIDE_LENGTH_CHUNKS;
    if (chunkToMoveJ < 0)
        chunkToMoveJ += RENDER_REGION_SIDE_LENGTH_CHUNKS; // guard against negative modulo values
    int16 chunkToMoveIndex = getChunkIndexFromIJ(chunkToMoveI, chunkToMoveJ);

    // move all current objects in the chunk to the dense array
    for (int k = 0; k < chunks[chunkToMoveIndex]._backPtr; k++)
    {
        moveObjectToDenseArray(&chunks[chunkToMoveIndex]._objects[k]);
    }
    chunks[chunkToMoveIndex]._backPtr = 0;

    // move all objects in the dense array into the new chunk, that need to be there
    int k = 0;
    while (k < _objectsDenseBackPtr)
    {
        int32 objChunkX = worldPosToChunkCoord(_objectsDense[k].transform.pos.x);
        int32 objChunkY = worldPosToChunkCoord(_objectsDense[k].transform.pos.z);

        if (chunkX == objChunkX && chunkY == objChunkY)
        {
            moveObjectToChunk(chunkToMoveIndex, &_objectsDense[k]);
            removeObjectFromDenseArray(k);
        }
        else
        {
            k += 1;
        }
    }
}

void moveChunks(int16 delta_x, int16 delta_y) {
    
    // calculate abs and sign
    int32 delta_x_abs = absInteger(delta_x);
    int32 delta_y_abs = absInteger(delta_y);
    int32 x_sign = 1;
    if (delta_x < 0) x_sign = -1;
    int32 y_sign = 1;
    if (delta_y < 0) y_sign = -1;

    // print("x:\n");
    // print_dec(x_sign); print("\n");
    // print_dec(delta_x_abs); print("\n");
    // print("y:\n");
    // print_dec(y_sign); print("\n");
    // print_dec(delta_y_abs); print("\n");

    // loop over all x
    print("NEW");
    for (int16 i = x_sign; absInteger(i) <= delta_x_abs; i += x_sign) {
        for (int16 j = chunkDisplacementY - RENDER_DISTANCE_CHUNKS; 
            j <= chunkDisplacementY + RENDER_DISTANCE_CHUNKS; j += 1) {

            // print("Doing a loop in\n");

            int16 chunkX = chunkDisplacementX + x_sign * RENDER_DISTANCE_CHUNKS + i;
            int16 chunkY = j;        

            processMovingChunk(chunkX, chunkY);
        }
    }
    
    // loop over all y
    for (int16 j = y_sign; absInteger(j) <= delta_y_abs; j += y_sign) {
        for (int16 i = chunkDisplacementX - RENDER_DISTANCE_CHUNKS; 
            i <= chunkDisplacementX + RENDER_DISTANCE_CHUNKS; i += 1) {

            // print("Doing a loop in\n");

            int16 chunkY = chunkDisplacementY + y_sign * RENDER_DISTANCE_CHUNKS + j;
            int16 chunkX = i;        

            processMovingChunk(chunkX, chunkY);
        }
    }

    chunkDisplacementX += delta_x;
    chunkDisplacementY += delta_y;
}



void destroyObject(ObjectHandle* objectHandle) {
    // remove the object from its location
    if (objectHandle->objectLocation == OBJECT_HANDLE_IN_DENSEARRAY) {
        removeObjectFromDenseArray(objectHandle->index);
    } else if (objectHandle->objectLocation == OBJECT_HANDLE_IN_CHUNK) {
        removeObjectFromChunk(objectHandle->chunkIndex, objectHandle->index);
    }

    // remove the objecthandle by copying back the last element
    int32 handleIndex = objectHandle->objectInstance->_handleIndex;
    _objectHandles[handleIndex] = _objectHandles[_objectHandlesBackPtr-1];
    _objectHandles[handleIndex].objectInstance->_handleIndex = handleIndex;

    _objectHandlesBackPtr -= 1;

    totalObjects -= 1;
}

void initChunks() {
    for (int i = 0; i < TOTAL_CHUNKS; i++) {
        chunks[i]._backPtr = 0;
    }
}