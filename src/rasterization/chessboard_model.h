#pragma once
#include "../game/game.h"

Model createChessBoardModel(Vec3* vertexDataPtr, uint16* indexDataPtr, uint8 startAtIndex);
Model createChessBoardSquare(Vec3* vertexDataPtr, uint16* indexDataPtr);