#pragma once
#include "../types.h"

#define SHIFT_AMOUNT 16
#define HALF_SHIFT_AMOUNT 8
#define FP_INTEGER(X) X << SHIFT_AMOUNT

extern inline int32 fpInt(int16 integerPart, uint16 decimalPart);
extern inline int32 fpAdd(int32 x, int32 y);
extern inline int32 fpSub(int32 x, int32 y);
extern inline int32 fpMul(int32 x, int32 y);
extern inline int32 fpDiv(int32 x, int32 y);
