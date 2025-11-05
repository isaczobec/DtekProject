#include "../types.h"

extern const uint32 UNIT;

typedef struct {
    uint32 x;
    uint32 y;
} Vec2;

typedef struct {
    uint32 x;
    uint32 y;
    uint32 z;
} Vec3;

void dotProduct(Vec3* v1, Vec3* v2, uint32* res);

void addVector(Vec3* v1, Vec3* v2, Vec3* res);

void subVector(Vec3* v1, Vec3* v2, Vec3* res);

void mulVector(Vec3* v1, Vec3* v2, Vec3* res);

void mulVectorScalar(Vec3* v1, uint32 m, Vec3* res);

void crossProduct(Vec3* v1, Vec3* v2, Vec3* res);