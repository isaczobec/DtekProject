#include "chunks.h"

ObjectHandle _objectHandles[MAX_OBJECTS_GLOBAL];
int32 _objectsMapBackPtr; // back of the objects map

ObjectInstance _objectsDense[MAX_OBJECTS_GLOBAL];
int32 _objectsDenseBackPtr; // back of the objects map

Chunk chunks[TOTAL_CHUNKS];

int16 getChunkIndex(int16 relX, int16 relY) {
    return relY * RENDER_REGION_SIDE_LENGTH_CHUNKS + relX;
}

ObjectHandle* createObjectInstance(Model* model, Transform transform) {

    // copy the values to the dense objects array
    if (_objectsDenseBackPtr >= MAX_OBJECTS_GLOBAL) {
        print("Object limit exceeded!");
        return (ObjectHandle*) NULL;
    }
    _objectsDense[_objectsDenseBackPtr].model = model;
    _objectsDense[_objectsDenseBackPtr].transform = transform;
    _objectsDenseBackPtr += 1;
    
    // create an object handle
    if (_objectsMapBackPtr >= MAX_OBJECTS_GLOBAL) {
        print("Object limit exceeded!");
        return (ObjectHandle*) NULL;
    }
    _objectHandles[_objectsDenseBackPtr].objectInstance = &_objectsDense[_objectsDenseBackPtr];
    _objectsMapBackPtr += 1;

    // set the object handle pointer
    _objectsDense[_objectsDenseBackPtr-1]._handleIndex = _objectsDenseBackPtr-1;
}

void moveObjectToChunk(int16 chunkIndex, ObjectInstance* objectInstance) {
    chunks[chunkIndex]._objects[chunks[chunkIndex]._backPtr] = *objectInstance;

    // update what the handle is pointing to
    ObjectHandle* handle = &_objectHandles[objectInstance->_handleIndex];
    handle->objectInstance = &chunks[chunkIndex]._objects[chunks[chunkIndex]._backPtr];
}


void initChunks() {
    for (int i = 0; i < TOTAL_CHUNKS; i++) {
        chunks[i]._backPtr = 0;
    }
}