#include "math.h"

const int32 pi = 0x3243F;
int32 _sin_samples[SIN_SAMPLES];

// long long __divdi3(long long a, long long b) {
//     return a / b;
// }

void initMath() {
    
    int32 testDiv = FP_TOINT(fpDiv(FP(74,0),FP(2,0))); 
    print("test div yields "); print_dec(testDiv); print("\n");

    int32 testMul = FP_TOINT(fpMul(FP(74,0),FP(2,0))); 
    print("test div yields "); print_dec(testMul); print("\n");

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

    int8 negative = angle < 0;
    angle = abs(angle);

    // normalize angle
    int32 cycleSize = fpMul(pi, FP(2,0));
    int32 cycles = FP_TOINT(fpDiv(angle, cycleSize));
    angle = fpSub(angle, fpMul(FP(cycles,0),cycleSize));

    int32 quadrantLength = fpDiv(pi,FP(2,0));
    int32 quadrant = FP_TOINT(fpDiv(angle, quadrantLength)) & 3;
    int8 reverse = quadrant % 2 == 1;

    int32 angleInsideQuadrant = fpSub(angle, fpMul(quadrantLength,FP(quadrant,0)));
    int32 fractionAngle = fpDiv(angleInsideQuadrant, quadrantLength);

    int32 res;
    int32 sampleFrom = FP_TOINT(fpMul(fractionAngle, FP(SIN_SAMPLES,0)));
    if (reverse) {
        sampleFrom = SIN_SAMPLES - 1 - sampleFrom;
    }
    int32 sample = _sin_samples[sampleFrom];
    if (negative)
        sample = fpMul(sample, FP(-1,0));
    if (quadrant > 1)
        sample = fpMul(sample, FP(-1,0));
    
    return sample;
}

int32 fpCos(int32 angle) {
    return fpSin(fpSub(fpDiv(pi,FP(2,0)),angle));
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

int32 vec3Len(Vec3* len) {
    //first need implementation of inv sqrt
    return 0;
}

void rotateXVec3(Vec3* v1, int32 angle) {
    int32 oldY = v1->y;
    int32 oldZ = v1->z;

    v1->y = fpCos(angle)*oldY - fpSin(angle)*oldZ;
    v1->z = fpSin(angle)*oldY + fpCos(angle)*oldZ;
}

void rotateYVec3(Vec3* v1, int32 angle) {
    int32 oldX = v1->x;
    int32 oldZ = v1->z;

    v1->x = fpCos(angle)*oldX+fpSin(angle)*oldZ;
    v1->z = fpCos(angle)*oldZ-fpSin(angle)*oldX;
}

void rotateZVec3(Vec3* v1, int32 angle) {
    int32 oldX = v1->x;
    int32 oldY = v1->y;

    v1->x = fpCos(angle)*oldX-fpSin(angle)*oldY;
    v1->y = fpSin(angle)*oldX+fpCos(angle)*oldY;
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

int32 absInteger(int32 x) {
    if (x < 0) {
        x = -1 * x;
    }
    return x;
}

int32 get(Mat3x4* mat, int8 i, int8 j) {
    uint8 index = i*4 + j;
    return mat->vals[index];
}
void set(Mat3x4* mat, int8 i, int8 j, int32 val) {
    uint8 index = i*4 + j;
    mat->vals[index] = val;
}

void switchRows(Mat3x4* M, int8 row1, int8 row2) {
    for (int i = 0; i < 4; i++) {
        int32 tmp = get(M, row1, i);
        set(M, row1, i, get(M, row2, i));
        set(M, row2, i, tmp);
    }
}

// pseudocode from https://www.chegg.com/homework-help/questions-and-answers/implement-pseudo-code-gaussian-elimination-pivoting-working-matlab-code-q34725272
void reduce(Mat3x4* M) {
    for (int8 j = 0; j < 3; j++) {
        
        // find a pivot if M_jj is 0
        if (get(M, j, j) == 0) {
            for (int8 k = j + 1; k < 3; k++) {
                if (get(M, k, j) != 0) {
                    switchRows(M, k, j);
                    break;
                }
            }
            if (get(M, j, j) == 0) {
                return;
            }
        }
        
        for (int8 i = j+1; i < 3; i++) {
            int32 div = get(M,j,j);
            if (div == 0) continue;
            int32 multiplier = fpDiv(get(M,i,j),div);

            // subtract multiplier 
            for (int8 col = 0; col < 4; col++) {
                int32 newVal = fpSub(get(M,i,col),fpMul(get(M,j,col),multiplier));
                set(M,i,col,newVal);
            }
        }   
    }

    // do jordan elimination
    for (int8 j = 2; j > 0; j--) {
        for (int8 k = j - 1; k >= 0; k--) {

            int32 div = get(M,j,j);
            if (div == 0) continue;
            int32 multiplier = fpDiv(get(M,k,j),div);
            for (int8 col = j; col < 4; col++) {
                int32 newVal = fpSub(get(M,k,col),fpMul(get(M,j,col),multiplier));
                set(M,k,col,newVal);
            }
        }
    }

    // normalize all elements
    for (int8 j = 0; j < 3; j++) {
        int32 div = get(M,j,j);
        if (div == 0) continue;
        set(M,j,3,fpDiv(get(M,j,3),div));
        set(M,j,j,FP(1,0));
    }


}