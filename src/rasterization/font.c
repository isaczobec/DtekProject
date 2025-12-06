#include "font.h"

typedef struct {
    uint8 advanceWidth;

    uint8 xOffset;
    uint8 yOffset;

    /* A pointer to the bitmap of
    this glyph.
    */
    uint8* bitmapPtr;
    /*the amount of 8 bit uints
    that this glyph has.*/
    uint8 bitmapRows; 
} BBFGlyph;


uint8 bitmaps[606] = {
0x80,
0x80,
0x80,
0x80,
0x80,
0x00,
0x80,
0x90,
0x90,
0x90,
0x6C,
0xFE,
0xFE,
0x6C,
0xFE,
0xFE,
0x6C,
0x10,
0x7C,
0x7C,
0x7C,
0xE0,
0x1C,
0xF0,
0x10,
0x8C,
0x0C,
0x0C,
0x10,
0x60,
0x60,
0x62,
0x70,
0x80,
0x72,
0x72,
0x72,
0x8C,
0x72,
0x80,
0x80,
0x80,
0x60,
0x80,
0x80,
0x80,
0x80,
0x80,
0x60,
0x80,
0x60,
0x60,
0x60,
0x60,
0x60,
0x80,
0x90,
0x60,
0x90,
0x90,
0x60,
0x60,
0xF0,
0x60,
0x60,
0x60,
0x80,
0xF0,
0x80,
0x02,
0x0C,
0x0C,
0x10,
0x60,
0x60,
0x80,
0x70,
0x8C,
0x8C,
0x8C,
0x8C,
0x8C,
0x70,
0xE0,
0x60,
0x60,
0x60,
0x60,
0x60,
0x60,
0xF0,
0x0C,
0x70,
0x70,
0x70,
0x80,
0xFC,
0xF0,
0x0C,
0x0C,
0x0C,
0x70,
0x0C,
0xF0,
0x10,
0x70,
0x90,
0x90,
0x90,
0xFC,
0x10,
0xFC,
0x80,
0xF0,
0xF0,
0xF0,
0x0C,
0xF0,
0x70,
0x80,
0x80,
0x80,
0xF0,
0x8C,
0x70,
0xFC,
0x0C,
0x10,
0x60,
0x60,
0x60,
0x60,
0x70,
0x8C,
0x70,
0x70,
0x70,
0x8C,
0x70,
0x70,
0x8C,
0x7C,
0x0C,
0x0C,
0x0C,
0x70,
0x80,
0x80,
0x00,
0x80,
0x80,
0x80,
0x80,
0x00,
0x80,
0x80,
0x80,
0x10,
0x00,
0x60,
0x80,
0x60,
0x00,
0x10,
0xF0,
0xF0,
0x00,
0xF0,
0xF0,
0x80,
0x60,
0x60,
0x10,
0x60,
0x00,
0x80,
0xF0,
0x0C,
0x70,
0x70,
0x70,
0x00,
0x60,
0x7C,
0x82,
0x82,
0x9E,
0x92,
0x92,
0x7C,
0x70,
0x8C,
0x8C,
0x8C,
0x8C,
0xFC,
0x8C,
0xF0,
0x8C,
0xF0,
0xF0,
0xF0,
0x8C,
0xF0,
0x70,
0x80,
0x80,
0x80,
0x80,
0x80,
0x70,
0xF0,
0x8C,
0x8C,
0x8C,
0x8C,
0x8C,
0xF0,
0xF0,
0x80,
0xF0,
0xF0,
0xF0,
0x80,
0xF0,
0xF0,
0x80,
0xF0,
0xF0,
0x80,
0x80,
0x80,
0x7C,
0x80,
0x80,
0x9C,
0x8C,
0x8C,
0x7C,
0x8C,
0x8C,
0x8C,
0xFC,
0x8C,
0x8C,
0x8C,
0xF0,
0x60,
0x60,
0x60,
0x60,
0x60,
0xF0,
0x1C,
0x0C,
0x0C,
0x0C,
0x8C,
0x8C,
0x70,
0x8C,
0x90,
0x90,
0xE0,
0x90,
0x80,
0x8C,
0x80,
0x80,
0x80,
0x80,
0x80,
0x80,
0xF0,
0x82,
0xEE,
0xEE,
0x92,
0x82,
0x82,
0x82,
0x8C,
0xEC,
0xEC,
0x9C,
0x8C,
0x8C,
0x8C,
0x70,
0x8C,
0x8C,
0x8C,
0x8C,
0x8C,
0x70,
0xF0,
0x8C,
0x8C,
0x8C,
0x8C,
0xF0,
0x80,
0x70,
0x8C,
0x8C,
0x8C,
0x8C,
0x8C,
0x70,
0x0C,
0xF0,
0x8C,
0x8C,
0x8C,
0x8C,
0xF0,
0x8C,
0x7C,
0x80,
0x70,
0x70,
0x70,
0x0C,
0xF0,
0xF0,
0x60,
0x60,
0x60,
0x60,
0x60,
0x60,
0x8C,
0x8C,
0x8C,
0x8C,
0x8C,
0x8C,
0x70,
0x8C,
0x8C,
0x8C,
0x90,
0x90,
0x90,
0x60,
0x82,
0x92,
0x92,
0x92,
0x92,
0x92,
0x6C,
0x8C,
0x8C,
0x8C,
0x70,
0x8C,
0x8C,
0x8C,
0x8C,
0x8C,
0x8C,
0x7C,
0x7C,
0x0C,
0x70,
0xF0,
0x10,
0x60,
0x60,
0x60,
0x80,
0xF0,
0xE0,
0x80,
0x80,
0x80,
0x80,
0x80,
0xE0,
0x80,
0x60,
0x60,
0x10,
0x0C,
0x0C,
0x02,
0xE0,
0x60,
0x60,
0x60,
0x60,
0x60,
0xE0,
0x60,
0x90,
0x90,
0xFC,
0x80,
0x60,
0x60,
0x7C,
0x8C,
0x8C,
0x8C,
0x8C,
0x7C,
0x80,
0xF0,
0x8C,
0x8C,
0x8C,
0x8C,
0xF0,
0x70,
0x70,
0x80,
0x80,
0x70,
0x70,
0x0C,
0x7C,
0x8C,
0x8C,
0x8C,
0x8C,
0x7C,
0x70,
0x70,
0x9C,
0xE0,
0xE0,
0x70,
0x10,
0x60,
0xF0,
0xF0,
0x60,
0x60,
0x60,
0x7C,
0x8C,
0x8C,
0x8C,
0x8C,
0x7C,
0x0C,
0x0C,
0x70,
0x80,
0xF0,
0xF0,
0x8C,
0x8C,
0x8C,
0x8C,
0x80,
0x00,
0x80,
0x80,
0x80,
0x80,
0x80,
0x60,
0x00,
0x60,
0x60,
0x60,
0x60,
0x60,
0x60,
0x80,
0x80,
0x80,
0x8C,
0x8C,
0x8C,
0xF0,
0xF0,
0x8C,
0x80,
0x80,
0x80,
0x80,
0x80,
0x80,
0x80,
0xFC,
0xFC,
0x92,
0x92,
0x92,
0x92,
0xF0,
0xF0,
0x8C,
0x8C,
0x8C,
0x8C,
0x70,
0x8C,
0x8C,
0x8C,
0x8C,
0x70,
0xF0,
0x8C,
0x8C,
0x8C,
0x8C,
0xF0,
0x80,
0x80,
0x80,
0x7C,
0x8C,
0x8C,
0x8C,
0x8C,
0x7C,
0x0C,
0x0C,
0x0C,
0x90,
0x90,
0xE0,
0x80,
0x80,
0x80,
0x7C,
0x7C,
0x7C,
0xE0,
0x1C,
0xF0,
0x60,
0xF0,
0xF0,
0xF0,
0x60,
0x60,
0x10,
0x8C,
0x8C,
0x8C,
0x8C,
0x8C,
0x7C,
0x8C,
0x8C,
0x8C,
0x90,
0x90,
0x60,
0x92,
0x92,
0x92,
0x6C,
0x6C,
0x6C,
0x90,
0x90,
0x90,
0x60,
0x60,
0x90,
0x8C,
0x8C,
0x8C,
0x8C,
0x8C,
0x7C,
0x0C,
0x70,
0x70,
0xFC,
0xFC,
0x10,
0x60,
0xFC,
0xFC,
0x70,
0x60,
0x60,
0x80,
0x60,
0x70,
0x70,
0x80,
0x80,
0x80,
0x80,
0x80,
0x80,
0x80,
0xE0,
0x60,
0x60,
0x10,
0x60,
0xE0,
0xE0,
0x6C,
0x90,
0x90,
0xF0,
};
BBFGlyph glyphs[256];


void init_glyphs() {
glyphs[32] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[0],
    .bitmapRows = 0
};
glyphs[33] = (BBFGlyph){
    .advanceWidth = 3,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[0],
    .bitmapRows = 7
};
glyphs[34] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 4,
    .bitmapPtr = &bitmaps[7],
    .bitmapRows = 3
};
glyphs[35] = (BBFGlyph){
    .advanceWidth = 8,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[10],
    .bitmapRows = 7
};
glyphs[36] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = -1,
    .bitmapPtr = &bitmaps[17],
    .bitmapRows = 8
};
glyphs[37] = (BBFGlyph){
    .advanceWidth = 8,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[25],
    .bitmapRows = 7
};
glyphs[38] = (BBFGlyph){
    .advanceWidth = 8,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[32],
    .bitmapRows = 7
};
glyphs[39] = (BBFGlyph){
    .advanceWidth = 3,
    .xOffset = 0,
    .yOffset = 4,
    .bitmapPtr = &bitmaps[39],
    .bitmapRows = 3
};
glyphs[40] = (BBFGlyph){
    .advanceWidth = 4,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[42],
    .bitmapRows = 7
};
glyphs[41] = (BBFGlyph){
    .advanceWidth = 4,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[49],
    .bitmapRows = 7
};
glyphs[42] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 3,
    .bitmapPtr = &bitmaps[56],
    .bitmapRows = 4
};
glyphs[43] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 1,
    .bitmapPtr = &bitmaps[60],
    .bitmapRows = 5
};
glyphs[44] = (BBFGlyph){
    .advanceWidth = 4,
    .xOffset = 0,
    .yOffset = -1,
    .bitmapPtr = &bitmaps[65],
    .bitmapRows = 2
};
glyphs[45] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 3,
    .bitmapPtr = &bitmaps[67],
    .bitmapRows = 1
};
glyphs[46] = (BBFGlyph){
    .advanceWidth = 3,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[68],
    .bitmapRows = 1
};
glyphs[47] = (BBFGlyph){
    .advanceWidth = 8,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[69],
    .bitmapRows = 7
};
glyphs[48] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[76],
    .bitmapRows = 7
};
glyphs[49] = (BBFGlyph){
    .advanceWidth = 4,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[83],
    .bitmapRows = 7
};
glyphs[50] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[90],
    .bitmapRows = 7
};
glyphs[51] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[97],
    .bitmapRows = 7
};
glyphs[52] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[104],
    .bitmapRows = 7
};
glyphs[53] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[111],
    .bitmapRows = 7
};
glyphs[54] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[118],
    .bitmapRows = 7
};
glyphs[55] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[125],
    .bitmapRows = 7
};
glyphs[56] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[132],
    .bitmapRows = 7
};
glyphs[57] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[139],
    .bitmapRows = 7
};
glyphs[58] = (BBFGlyph){
    .advanceWidth = 3,
    .xOffset = 0,
    .yOffset = 1,
    .bitmapPtr = &bitmaps[146],
    .bitmapRows = 5
};
glyphs[59] = (BBFGlyph){
    .advanceWidth = 3,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[151],
    .bitmapRows = 6
};
glyphs[60] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[157],
    .bitmapRows = 7
};
glyphs[61] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 1,
    .bitmapPtr = &bitmaps[164],
    .bitmapRows = 5
};
glyphs[62] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[169],
    .bitmapRows = 7
};
glyphs[63] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[176],
    .bitmapRows = 7
};
glyphs[64] = (BBFGlyph){
    .advanceWidth = 8,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[183],
    .bitmapRows = 7
};
glyphs[65] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[190],
    .bitmapRows = 7
};
glyphs[66] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[197],
    .bitmapRows = 7
};
glyphs[67] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[204],
    .bitmapRows = 7
};
glyphs[68] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[211],
    .bitmapRows = 7
};
glyphs[69] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[218],
    .bitmapRows = 7
};
glyphs[70] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[225],
    .bitmapRows = 7
};
glyphs[71] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[232],
    .bitmapRows = 7
};
glyphs[72] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[239],
    .bitmapRows = 7
};
glyphs[73] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[246],
    .bitmapRows = 7
};
glyphs[74] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[253],
    .bitmapRows = 7
};
glyphs[75] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[260],
    .bitmapRows = 7
};
glyphs[76] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[267],
    .bitmapRows = 7
};
glyphs[77] = (BBFGlyph){
    .advanceWidth = 8,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[274],
    .bitmapRows = 7
};
glyphs[78] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[281],
    .bitmapRows = 7
};
glyphs[79] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[288],
    .bitmapRows = 7
};
glyphs[80] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[295],
    .bitmapRows = 7
};
glyphs[81] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = -1,
    .bitmapPtr = &bitmaps[302],
    .bitmapRows = 8
};
glyphs[82] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[310],
    .bitmapRows = 7
};
glyphs[83] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[317],
    .bitmapRows = 7
};
glyphs[84] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[324],
    .bitmapRows = 7
};
glyphs[85] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[331],
    .bitmapRows = 7
};
glyphs[86] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[338],
    .bitmapRows = 7
};
glyphs[87] = (BBFGlyph){
    .advanceWidth = 8,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[345],
    .bitmapRows = 7
};
glyphs[88] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[352],
    .bitmapRows = 7
};
glyphs[89] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[359],
    .bitmapRows = 7
};
glyphs[90] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[366],
    .bitmapRows = 7
};
glyphs[91] = (BBFGlyph){
    .advanceWidth = 4,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[373],
    .bitmapRows = 7
};
glyphs[92] = (BBFGlyph){
    .advanceWidth = 8,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[380],
    .bitmapRows = 7
};
glyphs[93] = (BBFGlyph){
    .advanceWidth = 4,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[387],
    .bitmapRows = 7
};
glyphs[94] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 4,
    .bitmapPtr = &bitmaps[394],
    .bitmapRows = 3
};
glyphs[95] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[397],
    .bitmapRows = 1
};
glyphs[96] = (BBFGlyph){
    .advanceWidth = 4,
    .xOffset = 0,
    .yOffset = 4,
    .bitmapPtr = &bitmaps[398],
    .bitmapRows = 3
};
glyphs[97] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[401],
    .bitmapRows = 6
};
glyphs[98] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[407],
    .bitmapRows = 7
};
glyphs[99] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[414],
    .bitmapRows = 6
};
glyphs[100] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[420],
    .bitmapRows = 7
};
glyphs[101] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[427],
    .bitmapRows = 6
};
glyphs[102] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[433],
    .bitmapRows = 7
};
glyphs[103] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = -3,
    .bitmapPtr = &bitmaps[440],
    .bitmapRows = 9
};
glyphs[104] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[449],
    .bitmapRows = 7
};
glyphs[105] = (BBFGlyph){
    .advanceWidth = 3,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[456],
    .bitmapRows = 7
};
glyphs[106] = (BBFGlyph){
    .advanceWidth = 4,
    .xOffset = 0,
    .yOffset = -3,
    .bitmapPtr = &bitmaps[463],
    .bitmapRows = 10
};
glyphs[107] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[473],
    .bitmapRows = 7
};
glyphs[108] = (BBFGlyph){
    .advanceWidth = 3,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[480],
    .bitmapRows = 7
};
glyphs[109] = (BBFGlyph){
    .advanceWidth = 8,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[487],
    .bitmapRows = 6
};
glyphs[110] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[493],
    .bitmapRows = 6
};
glyphs[111] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[499],
    .bitmapRows = 6
};
glyphs[112] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = -3,
    .bitmapPtr = &bitmaps[505],
    .bitmapRows = 9
};
glyphs[113] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = -3,
    .bitmapPtr = &bitmaps[514],
    .bitmapRows = 9
};
glyphs[114] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[523],
    .bitmapRows = 6
};
glyphs[115] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[529],
    .bitmapRows = 6
};
glyphs[116] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[535],
    .bitmapRows = 7
};
glyphs[117] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[542],
    .bitmapRows = 6
};
glyphs[118] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[548],
    .bitmapRows = 6
};
glyphs[119] = (BBFGlyph){
    .advanceWidth = 8,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[554],
    .bitmapRows = 6
};
glyphs[120] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[560],
    .bitmapRows = 6
};
glyphs[121] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = -3,
    .bitmapPtr = &bitmaps[566],
    .bitmapRows = 9
};
glyphs[122] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[575],
    .bitmapRows = 6
};
glyphs[123] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[581],
    .bitmapRows = 7
};
glyphs[124] = (BBFGlyph){
    .advanceWidth = 3,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[588],
    .bitmapRows = 7
};
glyphs[125] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[595],
    .bitmapRows = 7
};
glyphs[126] = (BBFGlyph){
    .advanceWidth = 7,
    .xOffset = 0,
    .yOffset = 4,
    .bitmapPtr = &bitmaps[602],
    .bitmapRows = 3
};
glyphs[160] = (BBFGlyph){
    .advanceWidth = 6,
    .xOffset = 0,
    .yOffset = 0,
    .bitmapPtr = &bitmaps[605],
    .bitmapRows = 0
};


}

void draw_string(const char* s, uint16 i, uint16 j, uint8 color) {
    uint16 cursorX = i; 
    uint16 cursorY = j; 

    // print("\ndrawing a string: ");
    // print(s);
    // print("\n");
    

    int currIdx = 0;
    while(s[currIdx] != '\0') {

        // print("trying to write: ");
        // print(s);
        // print("\n");
        // print("drawing a pixel!: -");
        // char buf[2];
        // buf[0] = s[currIdx];
        // buf[1] = '\0';
        // print(buf);
        
        // print("-\n");
        // print("\n ascii code is: "); print_dec(s[currIdx]); print("\n");
        
        // get the correct glyph (cast to unsigned to avoid negative char indexing)
        BBFGlyph g = glyphs[(unsigned char)s[currIdx]];
        // print("bitmap rows: "); print_dec(g.bitmapRows); print("\n");

        // loop through all the bitmask rows in the glyph
        for (int rIdx = 0; rIdx < g.bitmapRows; ++rIdx) {
            uint8* bitmap = g.bitmapPtr + rIdx;

            for (uint8 cIdx = 0; cIdx < 8; cIdx++) {
                if ((0x80 >> cIdx) & (*bitmap)) {
                    uint16 x = cursorX + g.xOffset + cIdx;
                    uint16 y = cursorY + g.yOffset + rIdx;
                    drawToGizmoLayer(&textLayer, x, y, color);
                }
            }
        }
        cursorX += g.advanceWidth;

        currIdx += 1;
    }
}