#pragma once
#include "../types.h"
#include "fixed_point.h"

#define SIN_SAMPLES 64      // how many points in the range [0, pi/2] of the sine function we sample
#define SIN_TAYLOR_TERMS 6  // how many taylor terms we use to calculate each sample of the sine function

extern const int32 pi;
extern const int32 pi_half;
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

#define VEC3INIT (Vec3){0, 0, 0}

typedef struct {
    int32 vals[12];
} Mat3x4;
int32 get(Mat3x4* mat, int8 i, int8 j);
void set(Mat3x4* mat, int8 i, int8 j, int32 val);

typedef struct {
    int32 x;
    int32 y;
    int32 z;
    int32 w;
} Vec4;

// long long __divdi3(long long a, long long b);

void initMath();

// vector math
void dotProduct(Vec3* v1, Vec3* v2, int32* res);
void addVector(Vec3* v1, Vec3* v2, Vec3* res);
void subVector(Vec3* v1, Vec3* v2, Vec3* res);
void mulVector(Vec3* v1, Vec3* v2, Vec3* res);
void mulVectorScalar(Vec3* v1, int32 m, Vec3* res);
void crossProduct(Vec3* v1, Vec3* v2, Vec3* res);
int32 vec3Len(Vec3* v1);
void rotateXVec3(Vec3* v1, int32 angle);
void rotateYVec3(Vec3* v1, int32 angle);
void rotateZVec3(Vec3* v1, int32 angle);
// calculates the determinant of the matrix given by the x,y components of v1,v2
void detXY(Vec3* v1, Vec3* v2, int32* res); 

void switchRows(Mat3x4* M, int8 row1, int8 row2);
// reduces M to reduced row echeclon form
void reduce(Mat3x4* M);

// trig
int32 fpSin(int32 angle);
int32 fpCos(int32 angle);

// Uses fixed point for both arguments. assumes exponent is an integer, and only works that way.
int32 fpPow(int32 base, int32 exponent); 
// asumes f >= 0. Uses fixed point.
int32 fpFactorial(int32 f); 
int32 fpSqrt(int32 f);
void normalize(Vec3* v, Vec3* res);

int32 max(int32 x, int32 y);
int32 min(int32 x, int32 y);
int32 abs(int32 x);
int32 absInteger(int32 x);
