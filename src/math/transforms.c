// Author: Isac Zobec

#include "transforms.h"

void applyTransform(Vec3* v, Vec3* res, const Transform* transform) {

    // apply scale
    res->x = fpMul(v->x, transform->scale.x);
    res->y = fpMul(v->y, transform->scale.y);
    res->z = fpMul(v->z, transform->scale.z);
    
    
    // apply x rotation
    int32 sinX = fpSin(transform->rotation.x);
    int32 cosX = fpCos(transform->rotation.x);

    int32 orig_y = res->y;
    int32 orig_z = res->z;

    res->x = res->x;
    res->y = fpMul(orig_y, cosX) - fpMul(orig_z, sinX);
    res->z = fpMul(orig_y, sinX) + fpMul(orig_z, cosX);
    
    // apply y rotation
    int32 sinY = fpSin(transform->rotation.y);
    int32 cosY = fpCos(transform->rotation.y);

    int32 orig_x = res->x;
    orig_z = res->z;
    
    res->x = fpMul(orig_x, cosY)           + fpMul(orig_z, sinY);
    res->y = res->y;
    res->z = fpMul(fpMul(orig_x, sinY),FP(-1,0)) + fpMul(orig_z, cosY);
    
    // apply z rotation
    int32 sinZ = fpSin(transform->rotation.z);
    int32 cosZ = fpCos(transform->rotation.z);

    orig_x = res->x;
    orig_y = res->y;

    res->x = fpMul(orig_x, cosZ) - fpMul(orig_y, sinZ);
    res->y = fpMul(orig_x, sinZ) + fpMul(orig_y, cosZ);
    res->z = res->z;
    
    // apply position
    res->x = res->x + transform->pos.x;
    res->y = res->y + transform->pos.y;
    res->z = res->z + transform->pos.z;
}