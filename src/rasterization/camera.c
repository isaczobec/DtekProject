#include "camera.h"

Camera camera = {
    .position = {FP(0,0),FP(0,0),FP(0,0)},
    .rotation = {FP(0,0),FP(0,0),FP(0,0)},
    .cameraPositionTransform.scale = {FP(1,0),FP(1,0),FP(1,0)},
    .cameraPositionTransform.rotation = {FP(0,0),FP(0,0),FP(0,0)},
    .cameraPositionTransform.pos = {FP(0,0),FP(0,0),FP(0,0)},
    
    .cameraRotationTransform.scale = {FP(1,0),FP(1,0),FP(1,0)},
    .cameraRotationTransform.rotation = {FP(0,0),FP(0,0),FP(0,0)},
    .cameraRotationTransform.pos = {FP(0,0),FP(0,0),FP(0,0)},

    .nearClip = FP(0,0x1000),
    .farClip = FP(512,0x0000)
};

void updateCameraTransform(Camera* camera) {
    mulVectorScalar(&camera->position, FP(-1,0), &camera->cameraPositionTransform.pos);
    mulVectorScalar(&camera->rotation, FP(-1,0), &camera->cameraRotationTransform.rotation);
}

void centerRenderRegionAtCamera(Camera* camera) {
    int32 chunkX = worldPosToChunkCoord(fpMul(camera->cameraPositionTransform.pos.x,FP(-1,0)));
    int32 chunkY = worldPosToChunkCoord(fpMul(camera->cameraRotationTransform.pos.z,FP(-1,0)));
    int32 deltaX = chunkX - chunkDisplacementX;
    int32 deltaY = chunkY - chunkDisplacementY;
    if (deltaX != 0 || deltaY != 0) {
        moveChunks(deltaX, deltaY);
    }
}

void applyCameraTransform(Vec3* v, Vec3* res, Camera* camera) {
    Vec3 resPos;
    applyTransform(v, &resPos, &camera->cameraPositionTransform);
    applyTransform(&resPos, res, &camera->cameraRotationTransform);
}