#include "math.h"

void dotProduct(Vec3* v1, Vec3* v2, uint32* res) {
    *res = (v1->x*v2->x+v1->y*v2->y+v1->z*v2->z);
};

void addVector(Vec3* v1, Vec3* v2, Vec3* res) {
    res->x = v1->x+v2->x;
    res->y = v1->y+v2->y;
    res->z = v1->z+v2->z;
}

void subVector(Vec3* v1, Vec3* v2, Vec3* res) {
    res->x = v1->x-v2->x;
    res->y = v1->y-v2->y;
    res->z = v1->z-v2->z;
}

void mulVector(Vec3* v1, Vec3* v2, Vec3* res) {
    res->x = v1->x*v2->x;
    res->y = v1->y*v2->y;
    res->z = v1->z*v2->z;
}

void mulVectorScalar(Vec3* v1, uint32 m, Vec3* res) {
    res->x = v1->x * m;
    res->y = v1->y * m;
    res->z = v1->z * m;
}

void crossProduct(Vec3* v1, Vec3* v2, Vec3* res) {
    res->x = v1->y * v2->z - v1->z * v2->y;
    res->y = v1->z * v2->x - v1->x * v2->z;
    res->z = v1->x * v2->y - v1->y * v2->x;
}
