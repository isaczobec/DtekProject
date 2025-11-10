#pragma once
#include "../types.h"
#include "../io/io.h"

#define SHIFT_AMOUNT 16
#define FP(i,f) (int32)(((int32)i) << SHIFT_AMOUNT | (uint32) f)
#define FP_TOINT(f) (f >> SHIFT_AMOUNT)

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
static inline int32 fpDiv(int32 x, int32 y) {
    int64 resi = ((int64) x << SHIFT_AMOUNT) / (int64) y;
    return (int32)resi;
}