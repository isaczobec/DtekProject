#pragma once
#include "../math/math.h"
#include "../math/transforms.h"
#include "chunks.h"

typedef struct {

    int32 nearClip;
    int32 farClip;

    Transform cameraTransform;
    Vec3 position;
    Vec3 rotation;
} Camera;

extern Camera camera;

void updateCameraTransform(Camera* camera);

void centerRenderRegionAtCamera(Camera* camera);