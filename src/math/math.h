#include "../types.h"
#include "fixed_point.h"

extern const uint32 UNIT;

typedef struct {
    int32 x;
    int32 y;
} Vec2;

typedef struct {
    int32 x;
    int32 y;
    int32 z;
} Vec3;

void dotProduct(Vec3* v1, Vec3* v2, int32* res);

void addVector(Vec3* v1, Vec3* v2, Vec3* res);

void subVector(Vec3* v1, Vec3* v2, Vec3* res);

void mulVector(Vec3* v1, Vec3* v2, Vec3* res);

void mulVectorScalar(Vec3* v1, int32 m, Vec3* res);

void crossProduct(Vec3* v1, Vec3* v2, Vec3* res);