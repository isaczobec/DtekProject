#include "chess.h"

//shorthand for checking one square in each direction
const int32 UP = 8;
const int32 DOWN = -8;
const int32 RIGHT = 1;
const int32 LEFT = -1;
const int32 RUP = 9;
const int32 LUP = 7;
const int32 RDOWN = -7;
const int32 LDOWN = -9;

//shorthand for a group of squares

const int32 GOES_RIGHT[] = {RUP, RIGHT, RDOWN};
const int32 GOES_LEFT[] = {LUP, LEFT, LDOWN};
const int32 STRAIGHT[] = {UP, DOWN, RIGHT, LEFT};
const int32 DIAGONAL[] = {RUP, LUP, RDOWN, LDOWN};
const int32 ANY[] = {UP, DOWN, RIGHT, LEFT, RUP, LUP, RDOWN, LDOWN};
const int32 HORSE[] = {15, 17, 10, 6, -15, -17, -10, -6};
const int32 SHORT_RIGHT_HORSE[] = {17, -15}

//list of all pieces

const int32 all_pieces[] = {PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING};

//important ranks and files

const uint64 rank_h = 0x8080808080808080;
const uint64 rank_g = 0x4040404040404040;
const uint64 rank_a = 0x0101010101010101;
const uint64 rank_b = 0x0202020202020202;
const uint64 file_1 = 0xFF0000000000000000FF;