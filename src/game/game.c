#include "game.h"

Game game;

// Array to easily refer to the models for each piece
Model* models_map[AMOUNT_PIECES_TYPES];
ObjectHandle* pices_handles[AMOUNT_PIECES];

Vec3 boardVertexData[128*2];
uint16 boardIndexData[192*2];

Model whiteChessBoardModel;
Model blackChessBoardModel;
ObjectHandle* whiteBoardHandle;
ObjectHandle* blackBoardHandle;

uint8 selectedSquare;
Vec3 selectorSquareVertexData[4];
uint16 selectorSquareIndexData[6];
Model selectorSquareModel;
ObjectHandle* selector1SquareHandle;
ObjectHandle* selector2SquareHandle;

uint8 gameState=GAMESTATE_UNITIALIZED;

void gameInit() {

    whiteChessBoardModel = createChessBoardModel(boardVertexData, boardIndexData, 0);
    blackChessBoardModel = createChessBoardModel(&boardVertexData[128], &boardIndexData[192], 1);
    selectorSquareModel = createChessBoardSquare(selectorSquareVertexData, selectorSquareIndexData);

    // initialize models map
    models_map[PAWN] = m_pawn;
    models_map[BISHOP] = m_bishop;
    models_map[KNIGHT] = m_knight;
    models_map[ROOK] = m_rook;
    models_map[QUEEN] = m_queen;
    models_map[KING] = m_king;

    // initialize game state
    game = GAMEINIT;    
    selectedSquare = 0;

    // create the board
    Transform t = {
        .pos = {BOARD_LEFT_CORNER_X,N_FP(0,0x3000),BOARD_LEFT_CORNER_Y},
        .rotation = {0,0,0},
        .scale = {FP(1,0), FP(1,0), FP(1,0)}
    };
    whiteBoardHandle = createObjectInstance(&whiteChessBoardModel, t, COLOR_ARR[0]);
    blackBoardHandle = createObjectInstance(&blackChessBoardModel, t, COLOR_ARR[1]);

    // Make theese objects always render behind other objects
    whiteBoardHandle->objectInstance->objectInstanceFlags |= OBJECTINSTANCE_FLAG_BOTTOM_DEPTH_CHECK;
    blackBoardHandle->objectInstance->objectInstanceFlags |= OBJECTINSTANCE_FLAG_BOTTOM_DEPTH_CHECK;

    selector1SquareHandle = createObjectInstance(&selectorSquareModel, t, 0x0F);
    selector1SquareHandle->objectInstance->objectInstanceFlags |= OBJECTINSTANCE_ONLY_RENDER_EDGES;
    selector1SquareHandle->objectInstance->objectInstanceFlags |= OBJECTINSTANCE_RENDER_TO_GIZMO_LAYER;
    selector2SquareHandle = createObjectInstance(&selectorSquareModel, t, 0x1C);
    selector2SquareHandle->objectInstance->objectInstanceFlags |= OBJECTINSTANCE_ONLY_RENDER_EDGES;
    selector2SquareHandle->objectInstance->objectInstanceFlags |= OBJECTINSTANCE_RENDER_TO_GIZMO_LAYER;
    selector2SquareHandle->objectInstance->objectInstanceFlags |= OBJECTINSTANCE_FLAG_HIDDEN;

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
                    pices_handles[current_piece_ptr] = createObjectInstance(models_map[pieceIdx], t, COLOR_ARR[sideIdx]);
                    current_piece_ptr += 1;
    
                }
            }
        }
    }
    print("Initialized game, setting gamestate to GAMESTATE_ACTIVE!");
    gameState = GAMESTATE_ACTIVE;
}

void setSelectedSquare(uint8 selected) {
    if (gameState != GAMESTATE_ACTIVE) {
        return;
    }
    print("clearing gizmo layer\n");
    clearGizmoLayer(&gizmoLayer);
    
    print("moving and rendering\n");
    uint8 boardY = selected / 8;
    uint8 boardX = selected % 8;
    int32 xWorldPos = BOARD_LEFT_CORNER_X + fpMul(FP(boardX,0),SQUARE_SIZE);
    int32 yWorldPos = BOARD_LEFT_CORNER_Y + fpMul(FP(boardY,0),SQUARE_SIZE);
    Vec3 newPos = {xWorldPos, N_FP(0,0x3000), yWorldPos};
    setObjectPosition(selector1SquareHandle, &newPos);
    drawModel(selector1SquareHandle->objectInstance);
    print("jsdaiods");
}