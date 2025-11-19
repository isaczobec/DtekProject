#pragma once
#include "../math/math.h"
#include "../math/transforms.h"
#include "chunks.h"

typedef struct {

    int32 nearClip;
    int32 farClip;

    Transform cameraPositionTransform;
    Transform cameraRotationTransform;
    Vec3 position;
    Vec3 rotation;
} Camera;

extern Camera camera;

void updateCameraTransform(Camera* camera);

void centerRenderRegionAtCamera(Camera* camera);

void applyCameraTransform(Vec3* v, Vec3* res, Camera* camera); 