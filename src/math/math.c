#include "math.h"

void dotProduct(Vec3* v1, Vec3* v2, int32* res) {
    *res = (fpMul(v1->x,v2->x)
          + fpMul(v1->y,v2->y)
          + fpMul(v1->z,v2->z));
};

void addVector(Vec3* v1, Vec3* v2, Vec3* res) {
    res->x = fpAdd(v1->x,v2->x);
    res->y = fpAdd(v1->y,v2->y);
    res->z = fpAdd(v1->z,v2->z);
}

void subVector(Vec3* v1, Vec3* v2, Vec3* res) {
    res->x = fpSub(v1->x,v2->x);
    res->y = fpSub(v1->y,v2->y);
    res->z = fpSub(v1->z,v2->z);
}

void mulVector(Vec3* v1, Vec3* v2, Vec3* res) {
    res->x = fpMul(v1->x,v2->x);
    res->y = fpMul(v1->y,v2->y);
    res->z = fpMul(v1->z,v2->z);
}

void mulVectorScalar(Vec3* v1, int32 m, Vec3* res) {
    res->x = fpMul(v1->x,m);
    res->y = fpMul(v1->y,m);
    res->z = fpMul(v1->z,m);
}

void crossProduct(Vec3* v1, Vec3* v2, Vec3* res) {
    res->x = fpAdd(fpMul(v1->y,v2->z),fpMul(v1->z,v2->y));
    res->y = fpAdd(fpMul(v1->z,v2->x),fpMul(v1->x,v2->z));
    res->z = fpAdd(fpMul(v1->x,v2->y),fpMul(v1->y,v2->x));
}
