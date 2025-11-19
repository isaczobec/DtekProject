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
                int16 oldChunkIndex = handle->chunkIndex;
                int16 oldObjectIndex = handle->index;
                moveObjectToChunk(newChunkIndex, handle->objectInstance);
                int res = removeObjectFromChunk(oldChunkIndex, oldObjectIndex);
                if (res == -1) {
                    print("error in setObjectPosition case 1\n");
                    print_dec(newChunkY); print(" "); print_dec(newChunkX); print("\n");
                    print_dec(oldChunkY); print(" "); print_dec(oldChunkX); print("\n");
                    print_dec(oldObjectIndex); print(" "); print_dec(oldChunkIndex); print("\n");
                }

            } else {
                int16 oldObjectIndex = handle->index;
                moveObjectToChunk(newChunkIndex, handle->objectInstance);
                int res = removeObjectFromDenseArray(oldObjectIndex);
                if (res == -1) {print("error in setObjectPosition case 2\n");}
            }
        } else {
            if (handle->objectLocation == OBJECT_HANDLE_IN_CHUNK) {
                int16 oldChunkIndex = getChunkIndexFromWorldXY(oldChunkX, oldChunkY);
                int16 oldObjectIndex = handle->index;
                moveObjectToDenseArray(handle->objectInstance);
                int res = removeObjectFromChunk(oldChunkIndex, oldObjectIndex);
                if (res == -1) {print("error in setObjectPosition case 3\n");}
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
