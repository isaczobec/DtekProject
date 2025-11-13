#include "camera.h"

Camera camera = {
    .position = {FP(0,0),FP(0,0),FP(0,0)},
    .rotation = {FP(0,0),FP(0,0),FP(0,0)},
    .cameraTransform.scale = {FP(1,0),FP(1,0),FP(1,0)},
    .cameraTransform.rotation = {FP(0,0),FP(0,0),FP(0,0)},
    .cameraTransform.pos = {FP(0,0),FP(0,0),FP(0,0)},

    .nearClip = FP(0,0x1000),
    .farClip = FP(256,0x0000)
};

void updateCameraTransform(Camera* camera) {
    mulVectorScalar(&camera->position, FP(-1,0), &camera->cameraTransform.pos);
    mulVectorScalar(&camera->rotation, FP(-1,0), &camera->cameraTransform.rotation);
}