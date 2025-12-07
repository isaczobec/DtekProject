// Author: Isac Zobec

#pragma once
#include "../types.h"
#include "../math/fixed_point.h"
#include "../math/math.h"

typedef struct {
    Vec3 pos;
    Vec3 scale;
    Vec3 rotation;
    
} Transform;

// applies scale, rotation, and position transform to a vector,
// in that order
void applyTransform(Vec3* v, Vec3* res, const Transform* transform);

void chainTransforms(Transform* t1, Transform* t2, const Transform* res);