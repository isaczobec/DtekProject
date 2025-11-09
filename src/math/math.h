#pragma once
#include "../types.h"
#include "fixed_point.h"

#define SIN_SAMPLES 64      // how many points in the range [0, pi/2] of the sine function we sample
#define SIN_TAYLOR_TERMS 16 // how many taylor terms we use to calculate each sample of the sine function

extern const int32 pi;
extern int32 _sin_samples[SIN_SAMPLES];

typedef struct {
    int32 x;
    int32 y;
} Vec2;

typedef struct {
    int32 x;
    int32 y;
    int32 z;
} Vec3;

void initMath();

// vector math
void dotProduct(Vec3* v1, Vec3* v2, int32* res);
void addVector(Vec3* v1, Vec3* v2, Vec3* res);
void subVector(Vec3* v1, Vec3* v2, Vec3* res);
void mulVector(Vec3* v1, Vec3* v2, Vec3* res);
void mulVectorScalar(Vec3* v1, int32 m, Vec3* res);
void crossProduct(Vec3* v1, Vec3* v2, Vec3* res);
// calculates the determinant of the matrix given by the x,y components of v1,v2
void detXY(Vec3* v1, Vec3* v2, int32* res); 

// trig
int32 fpSin(int32 angle);
int32 fpCos(int32 angle);

// Uses fixed point for both arguments. assumes exponent is an integer, and only works that way.
int32 fpPow(int32 base, int32 exponent); 
// asumes f >= 0. Uses fixed point.
int32 fpFactorial(int32 f); 

int32 max(int32 x, int32 y);
int32 min(int32 x, int32 y);