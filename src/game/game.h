#pragma once
#include "../chess_engine/chess.h"
#include "../math/math.h"
#include "../math/fixed_point.h"
#include "../io/io.h"
#include "../rasterization/rasterization.h"
#include "../rasterization/camera.h"
#include "../rasterization/chunks.h"
#include "../rasterization/object_modification.h"
#include "../rasterization/color.h"
#include "../rasterization/models.h"

static const int32 SQUARE_SIZE = FP(1,0x5CCC);
static const int32 BOARD_LEFT_CORNER_X = N_FP(5,0);
static const int32 BOARD_LEFT_CORNER_Y = FP(1,0);

extern Game game;
extern ObjectHandle* pices_handles[AMOUNT_PIECES];

void gameInit();
