#pragma once
#include "../types.h"
#include "../io/io.h"

#define SHIFT_AMOUNT 16
#define FP(i,f) (int32)(((int32)i) << SHIFT_AMOUNT | (uint32) f)
#define FP_TOINT(f) (f >> SHIFT_AMOUNT)

#define FP_MUL(x,y) ((int32)((int64) x * (int64) y >> (SHIFT_AMOUNT)))
#define N_FP(i,f) FP_MUL(FP(i,f),FP(-1,0))

static inline int32 fpInt(int16 integerPart, uint16 fractionalPart) {
    return (int32)(((int32)integerPart) << SHIFT_AMOUNT | (uint32) fractionalPart);
}

static inline int32 fpAdd(int32 x, int32 y) {
    return x + y;
}
static inline int32 fpSub(int32 x, int32 y) {
    return x - y;
}
static inline int32 fpMul(int32 x, int32 y) {
    int64 resi = (int64) x * (int64) y;
    int32 res = (int32)(resi >> (SHIFT_AMOUNT));
    return res;
}
static inline int32 nfpInt(int16 integerPart, uint16 fractionalPart) {
    return fpMul((int32)(((int32)integerPart) << SHIFT_AMOUNT | (uint32) fractionalPart),-1);
}

#if defined(__DTEK__)
static inline int32 fpDiv(int32 x, int32 y) {

    
    int32 sign = (x<0) == (y<0) ? 1 : -1;
    if (x < 0) x = fpMul(x, FP(-1,0));
    if (y < 0) y = fpMul(y, FP(-1,0));

    // put y in [0.5,1] so that it doesnt diverge
    int32 norm_factor = 0;
    while (y > FP(1,0)) {
        y >>= 1;
        norm_factor += 1; 
    }
    while (y < FP(0,0x8000)) {
        y <<= 1;
        norm_factor -= 1; 
    }

    // newtons method for computing 1/y
    int32 yn = FP(1,0);
    
    // three iterations
    yn = fpMul(yn,FP(2,0) - fpMul(y,yn)); 
    yn = fpMul(yn,FP(2,0) - fpMul(y,yn)); 
    yn = fpMul(yn,FP(2,0) - fpMul(y,yn)); 

    if (norm_factor > 0) yn >>= norm_factor;
    else yn <<= -norm_factor;

    // print("yn is ");
    // print_dec(yn);
    // print("\n");
    
    return fpMul(fpMul(x,yn),FP(sign,0));
}
#else
static inline int32 fpDiv(int32 x, int32 y) {
    int64 resi = ((int64) x << SHIFT_AMOUNT) / (int64) y;
    return (int32)resi;
}
#endif

