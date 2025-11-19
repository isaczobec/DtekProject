#include "object_modification.h"

void setObjectPosition(ObjectHandle* handle, Vec3* newPosition) {
    int32 oldChunkX = worldPosToChunkCoord(handle->objectInstance->transform.pos.x);
    int32 oldChunkY = worldPosToChunkCoord(handle->objectInstance->transform.pos.z);
    
    int32 newChunkX = worldPosToChunkCoord(newPosition->x);
    int32 newChunkY = worldPosToChunkCoord(newPosition->z);

    // if the chunk has moved, handle moving where the object is stored
    if (newChunkX != oldChunkX || newChunkY != oldChunkY) {
        if (isInRenderDistance(newChunkX, newChunkY)) {

            int16 newChunkIndex = getChunkIndexFromWorldXY(newChunkX, newChunkY);
            
            if (handle->objectLocation == OBJECT_HANDLE_IN_CHUNK) {
                int16 oldChunkIndex = getChunkIndexFromWorldXY(oldChunkX, oldChunkY);
                int16 oldObjectIndex = handle->index;
                moveObjectToChunk(newChunkIndex, handle->objectInstance);
                removeObjectFromChunk(oldChunkIndex, oldObjectIndex);
            } else {
                int16 oldObjectIndex = handle->index;
                moveObjectToChunk(newChunkIndex, handle->objectInstance);
                removeObjectFromDenseArray(oldObjectIndex);
            }
        } else {
            if (handle->objectLocation == OBJECT_HANDLE_IN_CHUNK) {
                int16 oldChunkIndex = getChunkIndexFromWorldXY(oldChunkX, oldChunkY);
                int16 oldObjectIndex = handle->index;
                moveObjectToDenseArray(handle->objectInstance);
                removeObjectFromChunk(oldChunkIndex, oldObjectIndex);
            }
        }
    }

    // move the object in world space
    handle->objectInstance->transform.pos = *newPosition;
}

void setObjectRotation(ObjectHandle* handle, Vec3* newRotation) {
    handle->objectInstance->transform.rotation = *newRotation;
}
void setObjectScale(ObjectHandle* handle, Vec3* newScale) {
    handle->objectInstance->transform.scale = *newScale;
}
