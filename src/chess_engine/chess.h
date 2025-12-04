#pragma once
#include "../types.h"
// #define __WINDOWS__
#if defined(__WINDOWS__)
#define SDL_DISABLE_OLD_NAMES 0
#include "SDL3/SDL.h"
#include <stdio.h>
#endif

//Identifier for each piece
static const int32 PAWN = 0;
static const int32 BISHOP = 1;
static const int32 KNIGHT = 2;
static const int32 ROOK = 3;
static const int32 QUEEN = 4;
static const int32 KING = 5;

#define AMOUNT_PIECES_TYPES 6
#define AMOUNT_SQUARES 64
#define BOARD_LEN 8
#define AMOUNT_SIDES 2
#define AMOUNT_PIECES 32


//Identifier for each colour
static const int32 WHITE = 0;
static const int32 BLACK = 1;
static const int32 BOTH = 2;

//Identifier for gamestate

static const int32 InProgress = 0;
static const int32 GameOver = 1;
static const int32 Check = 2;

//The starting states of a standard chess board

static const uint64 InitialStatePerPiece[] = {
    0x00FF000000000000, //White Pawns
    0x2400000000000000, //White Bishops
    0x4200000000000000, //White Knights
    0x8100000000000000, //White Rooks
    0x0800000000000000, //White Queen
    0x1000000000000000, //White King
    0x000000000000FF00, //Black Pawns
    0x0000000000000024, //Black Bishops
    0x0000000000000042, //Black Knights
    0x0000000000000081, //Black Rooks
    0x0000000000000008, //Black Queen
    0x0000000000000010  //Black King
};

static const uint64 InitialStatePerSide[] = {
    0xFFFF000000000000, //White side
    0x000000000000FFFF, //Black side
    0xFFFF00000000FFFF  //Both sides
};

typedef struct{
    uint64 StatePerPiece[12]; //See Initial State for order of pieces
    uint64 StatePerSide[3]; //See Initial State for order of sides
    uint64 EnPassantPerSide[2];
} Positions;

typedef struct{ 
    int32 gamestate;
    Positions positions;
    int32 turn; //White = 0, Black = 1
    int32 castling[4];
} Game;

#define POSINIT (Positions){{\
InitialStatePerPiece[0],\
InitialStatePerPiece[1],\
InitialStatePerPiece[2],\
InitialStatePerPiece[3],\
InitialStatePerPiece[4],\
InitialStatePerPiece[5],\
InitialStatePerPiece[6],\
InitialStatePerPiece[7],\
InitialStatePerPiece[8],\
InitialStatePerPiece[9],\
InitialStatePerPiece[10],\
InitialStatePerPiece[11]\
}, {\
InitialStatePerSide[0],\
InitialStatePerSide[1],\
InitialStatePerSide[2]\
}, {\
(uint64)0, (uint64)0\
}}

#define POS0 (Positions){{\
0,\
0,\
0,\
0,\
0,\
0,\
0,\
0,\
0,\
0,\
0,\
0,\
}, {\
0, 0, 0}, {0, 0}\
}

#define GAMEINIT (Game){\
(int32)InProgress,\
POSINIT,\
(int32)0,\
{1, 1, 1, 1}\
}


int make_move(Game *game, uint64 curpos, uint64 newpos); //returns 1 when move is successful, otherwise 0
uint64 check_legal_moves(Game *game, uint64 curpos, int current_piece); //returns an integer where every 1 is a possible move for the piece on curpos
void print_board(uint64 position); //prints out position in 8x8 grid
