//Identifier for each piece
public const int32 PAWN = 0;
public const int32 BISHOP = 1;
public const int32 KNIGHT = 2;
public const int32 ROOK = 3;
public const int32 QUEEN = 4;
public const int32 KING = 5;

//Identifier for each colour
public const int32 WHITE = 0;
public const int32 BLACK = 1;
public const int32 BOTH = 2;

//Identifier for gamestate

public const int32 InProgress = 0;
public const int32 GameOver = 1;

//The starting states of a standard chess board

public const uint64 InitialStatePerPiece[] = [
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
]

public const uint InitialStatePerSide[] = [
    0xFF00000000000000, //White side
    0x00000000000000FF, //Black side
    0xFF000000000000FF  //Both sides
]

typedef struct{
    uint64 StatePerPiece[12];
    uint64 StatePerSide[3];
} Positions

typedef struct{ 
    int32 gamestate;
    Positions positions;
    int32 turn;
    int32 castling[4];
} Game
