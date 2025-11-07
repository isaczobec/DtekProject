#include "fixed_point.h"

inline int32 fpInt(int16 integerPart, uint16 fractionalPart) {
    return (int32)(((int32)integerPart) << SHIFT_AMOUNT | (uint32) fractionalPart);
}

inline int32 fpAdd(int32 x, int32 y) {
    return x + y;
}
inline int32 fpSub(int32 x, int32 y) {
    return x - y;
}
inline int32 fpMul(int32 x, int32 y) {
    int64 resi = (int64) x * (int64) y;
    int32 res = (int32)(resi >> (SHIFT_AMOUNT));
    return res;
}
inline int32 fpDiv(int32 x, int32 y) {
    int64 resi = ((int64) x << SHIFT_AMOUNT) / (int64) y;
    return (int32)resi;
}
