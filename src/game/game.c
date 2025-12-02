#include "game.h"

Game game;

// Array to easily refer to the models for each piece
Model* models_map[AMOUNT_PIECES_TYPES];
ObjectHandle* pices_handles[AMOUNT_PIECES];


void gameInit() {

    // initialize models map
    models_map[PAWN] = m_pawn;
    models_map[BISHOP] = m_bishop;
    models_map[KNIGHT] = m_knight;
    models_map[ROOK] = m_rook;
    models_map[QUEEN] = m_queen;
    models_map[KING] = m_king;

    // initialize game state
    game = GAMEINIT;    

    int32 current_piece_ptr = 0;
    // create pieces
    for (uint32 pieceIdx = 0; pieceIdx < AMOUNT_PIECES_TYPES; ++pieceIdx) {
        for (uint32 squareIdx = 0; squareIdx < AMOUNT_SQUARES; ++squareIdx) {
            for (uint32 sideIdx=0; sideIdx < AMOUNT_SIDES; ++sideIdx) {

                if (game.positions.StatePerPiece[sideIdx*AMOUNT_PIECES_TYPES + pieceIdx] & (1ULL << squareIdx)) {
                    // there was a piece here, create it
                    int32 boardX = squareIdx % BOARD_LEN;
                    int32 boardY = squareIdx / BOARD_LEN;
                    int32 xWorldPos = BOARD_LEFT_CORNER_X + fpMul(FP(boardX,0),SQUARE_SIZE);
                    int32 yWorldPos = BOARD_LEFT_CORNER_Y + fpMul(FP(boardY,0),SQUARE_SIZE);
    
                    print("\nboard x is"); print_dec(boardX);
                    print("\nboard y is"); print_dec(boardY);
                    print("\nworld x is"); print_dec(xWorldPos);
                    print("\nworld y is"); print_dec(yWorldPos);
    
                    Transform t = {
                        .pos = {xWorldPos, 0, yWorldPos},
                        .rotation = {0, 0, 0},
                        .scale = {FP(1,0), FP(1,0), FP(1,0)}
                    };
                    pices_handles[current_piece_ptr] = createObjectInstance(models_map[pieceIdx], t);
                    current_piece_ptr += 1;
    
                }
            }
        }
    }
}
