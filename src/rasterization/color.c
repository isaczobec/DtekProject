#include "color.h"

uint8 multiplyColor(uint8 color, int32 val) {
    int32 r = FP(color >> 5, 0);
    int32 g = FP((color >> 2) & 7, 0);
    int32 b = FP(color & 3,0);
    r = max(min(fpMul(r, val),FP(7,0)),0);
    g = max(min(fpMul(g, val),FP(7,0)),0);
    b = max(min(fpMul(b, val),FP(3,0)),0);

    uint8 r_bits = (uint8)(FP_TOINT(r)); 
    uint8 g_bits = (uint8)(FP_TOINT(g)); 
    uint8 b_bits = (uint8)(FP_TOINT(b)); 

    uint8 finalColor = r_bits << 5 | g_bits << 2 | b_bits;

    // print_hex32(finalColor); print("\n");

    return finalColor;
}
