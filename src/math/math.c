#include "math.h"

const int32 pi = 0x3243F;
int32 _sin_samples[SIN_SAMPLES];

void initMath() {
    
    // calculate sine samples
    int32 step = fpDiv(fpDiv(pi, FP(2,0)),FP(SIN_SAMPLES,0));
    
    for (uint32 stepIndex = 0; stepIndex < SIN_SAMPLES; stepIndex++) {
        int32 res = 0;
        for (uint32 j = 0; j < SIN_TAYLOR_TERMS; j++) {
            int32 x = fpMul(step, FP(stepIndex,0));
            int32 abs = fpDiv(fpPow(x, FP(j*2 + 1,0)), fpFactorial(FP(j*2 + 1,0)));
            if (j % 2 == 0) {
                res += abs;
            } else {
                res -= abs;
            }
        }
        _sin_samples[stepIndex] = res;
    }

    print("test val is ");
    print_dec(_sin_samples[SIN_SAMPLES/2]);
    print("\n");

}

int32 fpSin(int32 angle) {
    int32 reg = abs(fpDiv(angle,fpDiv(pi, FP(2,0))));
}


int32 fpPow(int32 base, int32 exponent) {
    int32 exp_int = FP_TOINT(exponent);
    int32 res = FP(1,0);
    if (exp_int > 0) {
        while (exp_int > 0) {
            res = fpMul(res, base);
            exp_int -= 1;
        }
    } else {
        while (exp_int < 0) {
            res = fpDiv(res, base);
            exp_int += 1;
        }
    }
    return res;
}

int32 fpFactorial(int32 f) {
    if (FP_TOINT(f) == 0 || FP_TOINT(f) == 1) 
        return FP(1,0);
    int32 res = f;
    while(FP_TOINT(f) > 2) {
        f = f - FP(1,0);
        res = fpMul(res, f);
    }
    return res;
}

void dotProduct(Vec3* v1, Vec3* v2, int32* res) {
    *res = (fpMul(v1->x,v2->x)
          + fpMul(v1->y,v2->y)
          + fpMul(v1->z,v2->z));
}

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

void detXY(Vec3* v1, Vec3* v2, int32* res) {
    *res = fpSub(fpMul(v1->x, v2->y), fpMul(v2->x, v1->y));
}

int32 max(int32 x, int32 y) {
    if (x > y) return x;
    else return y;
}

int32 min(int32 x, int32 y) {
    if (x < y) return x;
    else return y;
}

int32 abs(int32 x) {
    if (x < 0) {
        x = fpMul(x, FP(-1,0));
    }
    return x;
}