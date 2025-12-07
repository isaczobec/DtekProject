// Author: Isac Zobec

#include "chessboard_model.h"

Model createChessBoardModel(Vec3* vertexDataPtr, uint16* indexDataPtr, uint8 startAtIndex) {

    Vec3* vP = vertexDataPtr;
    uint16* ip = indexDataPtr;

    int32 halfSquareSize = fpDiv(SQUARE_SIZE,FP(2,0));

    uint16 vertexCount = 0;
    uint16 indexCount = 0;
    for (uint8 i = startAtIndex; i < 64; i+=2) {
        uint8 y = i / 8;
        uint8 x = (i+y) % 8;

        Vec3 v1 = {
            fpMul(FP(x,0),SQUARE_SIZE)-halfSquareSize, 0,
            fpMul(FP(y,0),SQUARE_SIZE)-halfSquareSize
        };
        Vec3 v2 = {
            fpMul(FP(x+1,0),SQUARE_SIZE)-halfSquareSize, 0,
            fpMul(FP(y,0),SQUARE_SIZE)-halfSquareSize
        };
        Vec3 v3 = {
            fpMul(FP(x,0),SQUARE_SIZE)-halfSquareSize, 0,
            fpMul(FP(y+1,0),SQUARE_SIZE)-halfSquareSize
        };
        Vec3 v4 = {
            fpMul(FP(x+1,0),SQUARE_SIZE)-halfSquareSize, 0,
            fpMul(FP(y+1,0),SQUARE_SIZE)-halfSquareSize
        };

        *(vP)   = v1;
        *(vP+1) = v2;
        *(vP+2) = v3;
        *(vP+3) = v4;
        vertexCount += 4;
        vP += 4;
        
        uint16 baseIndex = vertexCount - 4;
        *(ip)   = baseIndex+2;
        *(ip+1) = baseIndex+1;
        *(ip+2) = baseIndex+0;
        *(ip+3) = baseIndex+2;
        *(ip+4) = baseIndex+3;
        *(ip+5) = baseIndex+1;
        ip += 6;
        indexCount += 6;
    }

    Model model = {
        .vertexBuffer = vertexDataPtr,
        .indexBuffer = indexDataPtr,
        .vertexCount = vertexCount,
        .indexBufferCount = indexCount
    };
    return model;
}

Model createChessBoardSquare(Vec3* vertexDataPtr, uint16* indexDataPtr) {
    Vec3* vP = vertexDataPtr;
    uint16* ip = indexDataPtr;

    int32 halfSquareSize = fpDiv(SQUARE_SIZE,FP(2,0));

    Vec3 v1 = {
        -halfSquareSize, 0,
        -halfSquareSize
    };
    Vec3 v2 = {
        halfSquareSize, 0,
        -halfSquareSize
    };
    Vec3 v3 = {
        -halfSquareSize, 0,
        halfSquareSize
    };
    Vec3 v4 = {
        halfSquareSize, 0,
        halfSquareSize
    };

    *(vP)   = v1;
    *(vP+1) = v2;
    *(vP+2) = v3;
    *(vP+3) = v4;
    
    *(ip)   = 2;
    *(ip+1) = 1;
    *(ip+2) = 0;
    *(ip+3) = 2;
    *(ip+4) = 3;
    *(ip+5) = 1;

    Model model = {
        .vertexBuffer = vertexDataPtr,
        .indexBuffer = indexDataPtr,
        .vertexCount = 4,
        .indexBufferCount = 6
    };
    return model;
}