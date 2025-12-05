#pragma once
#include "../types.h"
#include "../io/io.h"

#define SHIFT_AMOUNT 16
#define FP(i,f) (int32)(((int32)i) << SHIFT_AMOUNT | (uint32) f)
#define FP_TOINT(f) (f >> SHIFT_AMOUNT)

#define FP_MUL(x,y) ((int32)((int64) x * (int64) y >> (SHIFT_AMOUNT)))
#define N_FP(i,f) FP_MUL(FP(i,f),FP(-1,0))

int32 fpInt(int16 integerPart, uint16 fractionalPart);
int32 fpAdd(int32 x, int32 y);
int32 fpSub(int32 x, int32 y);
int32 fpMul(int32 x, int32 y);
int32 nfpInt(int16 integerPart, uint16 fractionalPart);

#if defined(__DTEK__)
int32 fpDiv(int32 x, int32 y);
#else
int32 fpDiv(int32 x, int32 y);
#endif

