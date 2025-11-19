#include "camera.h"

Camera camera = {
    .position = {FP(0,0),FP(0,0),FP(0,0)},
    .rotation = {FP(0,0),FP(0,0),FP(0,0)},
    .cameraTransform.scale = {FP(1,0),FP(1,0),FP(1,0)},
    .cameraTransform.rotation = {FP(0,0),FP(0,0),FP(0,0)},
    .cameraTransform.pos = {FP(0,0),FP(0,0),FP(0,0)},

    .nearClip = FP(0,0x1000),
    .farClip = FP(512,0x0000)
};

void updateCameraTransform(Camera* camera) {
    mulVectorScalar(&camera->position, FP(-1,0), &camera->cameraTransform.pos);
    mulVectorScalar(&camera->rotation, FP(-1,0), &camera->cameraTransform.rotation);
}

void centerRenderRegionAtCamera(Camera* camera) {
    int32 chunkX = worldPosToChunkCoord(fpMul(camera->cameraTransform.pos.x,FP(-1,0)));
    int32 chunkY = worldPosToChunkCoord(fpMul(camera->cameraTransform.pos.z,FP(-1,0)));
    int32 deltaX = chunkX - chunkDisplacementX;
    int32 deltaY = chunkY - chunkDisplacementY;
    if (deltaX != 0 || deltaY != 0) {
        moveChunks(deltaX, deltaY);
    }
}
