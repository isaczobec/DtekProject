// Author: Isac Zobec

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
#include "../rasterization/chessboard_model.h"
#include "../rasterization/font.h"
#include "../chess_engine/chess.h"

#define GAMESTATE_UNITIALIZED 1
#define GAMESTATE_ACTIVE_WAITING_FOR_MOVE 2
#define GAMESTATE_ACTIVE_WAITING_TO_RENDER 3
#define GAMESTATE_WAITING_FOR_PROMOTION_INPUT 4

#define GAME_STATUS_IN_PROGRESS 1
#define GAME_STATUS_OVER 2

static const int32 SQUARE_SIZE = FP(1,0x5CCC);
static const int32 SQUARE_SIZE_PIECES = FP(1,0x4ECC); // Other offset for pieces because they for some reason are not aligned
static const int32 BOARD_LEFT_CORNER_X = N_FP(4,0x8000);
static const int32 BOARD_LEFT_CORNER_Y = FP(1,0);

static uint8 COLOR_ARR[AMOUNT_SIDES] = {0xFF, 0xF0};

extern Game game;
extern ObjectHandle* pices_handles[AMOUNT_PIECES];
extern uint8 gameState; // Used for game control to determine what to do next. Takes values of the `GAME_STATE_[...]` macros.
extern uint8 gameStatus; // if the game is in progress or over. Takes values of the `GAME_STATUS_[...]` macros.

void setStateToWaitingForInput(uint8 draw);
void gameInit(uint8 player1color, uint8 player2color);
void UpdateBoardWithPieces();
void DestroyPieces();
void setSelectedSquare(uint8 selected);
void confirmSelectedSquare();