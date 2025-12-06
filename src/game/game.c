#include "game.h"

Game game;

// Array to easily refer to the models for each piece
Model* models_map[AMOUNT_PIECES_TYPES];

#define NO_PIECE ((ObjectHandle*)0x00)
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

#define NO_POS 0xFF

uint8 gameState=GAMESTATE_UNITIALIZED;
uint8 gameStatus; 
uint8 hoveredPos1 = 0;
uint8 hoveredPos2 = 1;
uint8 selectedPos1 = NO_POS;
uint8 selectedPos2 = NO_POS;

uint8 pieces_ptr = 0;

uint8 getBoardIndex(uint8 x, uint8 y) {
    return y*BOARD_LEN + x;
}

void gameInit(uint8 player1color, uint8 player2color) {

    gameStatus = GAME_STATUS_IN_PROGRESS;
    
    pieces_ptr = 0;

    COLOR_ARR[0] = player1color;
    COLOR_ARR[1] = player2color;
    
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
        .pos = {BOARD_LEFT_CORNER_X,N_FP(0,0x1000),BOARD_LEFT_CORNER_Y},
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
    
    // init the piece array with null pointers
    for (int i = 0; i < AMOUNT_PIECES; i++) {
        pices_handles[i] = NO_PIECE;
    }
    
    draw_string("Player 1 Turn!", 30,30, 0xFF);
    
    updateBoardWithPieces();
    print("Initialized game!\n");
}

void updateBoardWithPieces()
{
    // delete existing pieces
    DestroyPieces();
    int current_piece_index = 0;

    // create pieces
    for (uint32 pieceIdx = 0; pieceIdx < AMOUNT_PIECES_TYPES; ++pieceIdx)
    {
        for (uint32 squareIdx = 0; squareIdx < AMOUNT_SQUARES; ++squareIdx)
        {
            for (uint32 sideIdx = 0; sideIdx < AMOUNT_SIDES; ++sideIdx)
            {
                if (game.positions.StatePerPiece[sideIdx * AMOUNT_PIECES_TYPES + pieceIdx] & (1ULL << squareIdx))
                {

                    // there was a piece here, create it
                    int32 boardX = squareIdx % BOARD_LEN;
                    int32 boardY = squareIdx / BOARD_LEN;
                    int32 xWorldPos = BOARD_LEFT_CORNER_X + fpMul(FP(boardX, 0), SQUARE_SIZE_PIECES);
                    int32 yWorldPos = BOARD_LEFT_CORNER_Y + fpMul(FP(boardY, 0), SQUARE_SIZE_PIECES);

                    Transform t = {
                        .pos = {xWorldPos, 0, yWorldPos},
                        .rotation = {0, 0, 0},
                        .scale = {FP(1, 0), FP(1, 0), FP(1, 0)}};
                    pices_handles[current_piece_index] = createObjectInstance(models_map[pieceIdx], t, COLOR_ARR[sideIdx]);
                    current_piece_index += 1;
                }
            }
        }
    }
    pieces_ptr = current_piece_index;
}

void DestroyPieces()
{
    for (int i = 0; i < pieces_ptr; i++)
    {
        if (pices_handles[i] != NO_PIECE)
        {
            destroyObject(pices_handles[i]);
            pices_handles[i] = NO_PIECE;
        }
    }
}

void setSelectedSquare(uint8 selected) {

    if (gameState != GAMESTATE_ACTIVE_WAITING_FOR_MOVE) {
        return;
    }
    
    ObjectHandle* selectorToMove;
    if (selectedPos1 == NO_POS) {
        selectorToMove = selector1SquareHandle;
        hoveredPos1 = selected;
    } else {
        selectorToMove = selector2SquareHandle;
        hoveredPos2 = selected;
    }

    // print("clearing gizmo layer\n");
    clearGizmoLayer(&gizmoLayer);
    
    // print("moving and rendering\n");
    uint8 boardY = selected / 8;
    uint8 boardX = selected % 8;
    int32 xWorldPos = BOARD_LEFT_CORNER_X + fpMul(FP(boardX,0),SQUARE_SIZE);
    int32 yWorldPos = BOARD_LEFT_CORNER_Y + fpMul(FP(boardY,0),SQUARE_SIZE);
    Vec3 newPos = {xWorldPos, N_FP(0,0x3000), yWorldPos};
    setObjectPosition(selectorToMove, &newPos);
    if (selectedPos1 != NO_POS) drawModel(selector1SquareHandle->objectInstance);
    drawModel(selectorToMove->objectInstance);
    swapBuffer();
    // print("jsdaiods");
}

void confirmSelectedSquare() {
    if (gameState != GAMESTATE_ACTIVE_WAITING_FOR_MOVE) 
        return;

    if (selectedPos1 == NO_POS) {
        selectedPos1 = hoveredPos1;
        hoveredPos2 = hoveredPos1;
        selector2SquareHandle->objectInstance->objectInstanceFlags &= ~OBJECTINSTANCE_FLAG_HIDDEN;
        setSelectedSquare(hoveredPos2);
        swapBuffer();
    } else {
        selectedPos2 = hoveredPos2;

        if (game.gamestate == Check) {
            print("CHECK!\n");
        } 
        int res = make_move(&game, 1ULL<<selectedPos1, 1ULL<<selectedPos2);

        if (res == 0) {
            print("move was illegal!\n");

            clearGizmoLayer(&textLayer);
            draw_string("Move was ILLEGAL!", 200, 30, 0xFF);
            swapBuffer();

            setStateToWaitingForInput(1);
            return;
        }

        print("performing move...\n");

        // check for special conditions
        if (game.gamestate == Promote) {
            gameState = GAMESTATE_WAITING_FOR_PROMOTION_INPUT;

            // get the input state to clear the button flag
            getInputState();
            
            draw_string("PROMOTION TIME! :D", 140, 25, 0xFF);
            draw_string("USE SWITCHES 0-1 TO SELECT,", 140, 35, 0xFF);
            draw_string("AND THEN PRESS BUTTON", 140, 45, 0xFF);
            draw_string("00: BISHOP", 140, 55, 0xFF);
            draw_string("01: KNIGHT", 140, 65, 0xFF);
            draw_string("10: ROOK", 140, 75, 0xFF);
            draw_string("11: OUEEN", 140, 85, 0xFF);
            swapBuffer();
            
            while (1) {
                int action = poll();
                if (action == 1) return 0;
                InputState state = getInputState();
                
                if (state.pressedButton) {
                    // use only bottom three three switches and add one
                    // so that the input is converted to piece identifiers
                    int32 inputPieceId = (state.switches & 0x3) + 1; 
                    int promoteRes = promote(&game, 1ULL<<selectedPos2, inputPieceId);
                    
                    if (promoteRes == 0) {
                        print("no success promoting!\n");
                    }
                    break;
                }
            }
            
        }

        if (game.gamestate == CheckMate) {
            gameStatus = GAME_STATUS_OVER;
            draw_string("CHECKMATE! :D", 140, 25, 0xFF);
            draw_string("PRESS BUTTON TO", 140, 35, 0xFF);
            draw_string("RETURN TO MENU", 140, 45, 0xFF);
            print("CHECK_MATE!\n");
            swapBuffer();
        }
        
        updateBoardWithPieces();

        gameState = GAMESTATE_ACTIVE_WAITING_TO_RENDER;
        swapBuffer();
        return;
    }
}

void printStatusText() {
    if (game.turn % 2 == 0) {
        draw_string("PLAYER 1 TURN!", 30, 30, 0xFF);
    } else {
        draw_string("PLAYER 2 TURN!", 30, 30, 0xFF);
    }

    if (game.gamestate == Check) {
        draw_string("CHEK :D", 140, 15, 0xFF);
    }
}

void setStateToWaitingForInput(uint8 draw) {
    hoveredPos1 = hoveredPos1;
    hoveredPos2 = hoveredPos1;
    selectedPos1 = NO_POS;
    selectedPos2 = NO_POS;

    printStatusText(); 
    swapBuffer();

    selector2SquareHandle->objectInstance->objectInstanceFlags |= OBJECTINSTANCE_FLAG_HIDDEN;
    if (draw) {
        clearGizmoLayer(&gizmoLayer);
        drawModel(selector1SquareHandle->objectInstance);
        swapBuffer();
    }
    gameState = GAMESTATE_ACTIVE_WAITING_FOR_MOVE;
}