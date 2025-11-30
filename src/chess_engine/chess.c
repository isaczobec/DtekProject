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
const int32 RUPLUP[] = {RUP, LUP};

//list of all pieces

const int32 all_pieces[] = {PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING};

//important ranks and files

const uint64 rank_h = 0x8080808080808080;
const uint64 rank_g = 0x4040404040404040;
const uint64 rank_a = 0x0101010101010101;
const uint64 rank_b = 0x0202020202020202;
const uint64 file_1_8 = 0xFF000000000000FF;
const uint64 file_2 = 0x00FF000000000000;
const uint64 file_7 = 0x000000000000FF00;

uint64 shift(uint64 value, int shift) {
    if (shift > 0) {
        return value >> shift;
    } else {
        return value << -shift;
    }
}


int not_blocked_by_piece(Game *game, uint64 check_pos) {
    if (game->positions.StatePerSide[game->turn]&check_pos) {
        return 0;
    }
    return 1;
}

int not_outside(uint64 curpos, int direction) {
    if(curpos&rank_a && (direction == LEFT || direction == LUP || direction == LDOWN) || curpos&rank_h && (direction == RIGHT || direction == RUP || direction == RDOWN)) {
        return 0;
    }
    return 1;
}

int horse_block(uint64 curpos, int direction) {
    if (curpos&rank_a && (direction == 10 || direction == -10 || direction == 17 || -17) ||
        curpos&rank_h && (direction == 6 || direction == -6 || direction == 15 || -15) ||
        curpos&rank_b && (direction == 10 || direction == -10) ||
        curpos&rank_g && (direction == 6 || direction == -6)) {
        return 0;
    }
    return 1;
}

int takes_piece(Game *game, uint64 check_pos) {
    if(game->positions.StatePerSide[!game->turn]&check_pos) {
        return 0;
    }
    return 1;
}

uint64 check_direction(Game *game, uint64 curpos, int32 direction, int32 is_recursive) {
    uint64 legal_moves = 0;
    if (not_blocked_by_piece(game, shift(curpos, direction)) && not_outside(curpos, direction)) {
        legal_moves += shift(curpos, direction);
        is_recursive = is_recursive*(game, legal_moves);
        if(is_recursive && shift(curpos, direction)) {
        legal_moves += check_direction(game, shift(curpos, direction), direction, is_recursive);
    }
    }
    return legal_moves;
}

uint64 check_king_moves(Game *game, uint64 curpos) {
    uint64 legal_moves = 0;
    for (int i = 0; i < *(&ANY+1) - ANY; i++) {
        legal_moves += check_direction(game, curpos, ANY[i], 0);
    }
    return legal_moves;
}

uint64 check_queen_moves(Game *game, uint64 curpos) {
    uint64 legal_moves = 0;
    for (int i = 0; i < *(&ANY+1) - ANY; i++) {
        legal_moves += check_direction(game, curpos, ANY[i], 1);
    }
    return legal_moves;
}

uint64 check_rook_moves(Game *game, uint64 curpos) {
    uint64 legal_moves = 0;
    for (int i = 0; i < *(&STRAIGHT+1) - STRAIGHT; i++) {
        legal_moves += check_direction(game, curpos, STRAIGHT[i], 1);
    }
    return legal_moves;
}

uint64 check_bishop_moves(Game *game, uint64 curpos) {
    uint64 legal_moves = 0;
    for (int i = 0; i < *(&DIAGONAL+1) - DIAGONAL; i++) {
        legal_moves += check_direction(game, curpos, DIAGONAL[i], 1);
    }
    return legal_moves;
}

uint64 check_pawn_moves(Game *game, uint64 curpos) {
    uint64 legal_moves = 0;
    if (not_blocked_by_piece(game, shift(curpos, 2*game->turn*DOWN+UP))) {
        legal_moves += shift(curpos, 2*game->turn*DOWN+UP);
        printf("\n%d\n", 2*game->turn*DOWN+UP);
        if(file_2&curpos || file_7&curpos && not_blocked_by_piece(game, shift(curpos, (2*game->turn*DOWN+UP)*2))) {
            legal_moves += shift(curpos, (2*game->turn*DOWN+UP)*2);
        }
    }
    for (int i = 0; i < *(&RUPLUP+1) - RUPLUP; i++) {
        if(game->positions.StatePerSide[!game->turn]&shift(curpos, -2*game->turn*RUPLUP[i]+RUPLUP[i]) && not_outside(curpos, RUPLUP[i])) {
            legal_moves += shift(curpos, -2*game->turn*RUPLUP[i]+RUPLUP[i]);
        }
    }
    return legal_moves;
}

uint64 check_knight_moves(Game *game, uint64 curpos) {
    uint64 legal_moves = 0;
    for (int i = 0; i < *(&HORSE+1) - HORSE; i++) {
        if(horse_block(curpos, HORSE[i]) && not_blocked_by_piece(game, shift(curpos, HORSE[i]))) {
            legal_moves += shift(curpos, HORSE[i]);
        }
    }
    return legal_moves;
}

uint64 check_legal_moves(Game *game, uint64 curpos, int current_piece) {

    switch (current_piece) {
        case 0:
            return check_pawn_moves(game, curpos);
            break;
        case 1:
            return check_bishop_moves(game, curpos);
            break;
        case 2:
            return check_knight_moves(game, curpos);
            break;
        case 3:
            return check_rook_moves(game, curpos);
            break;
        case 4:
            return check_queen_moves(game, curpos);
            break;
        case 5:
            return check_king_moves(game, curpos);
            break;
    }
}

int make_move(Game *game, uint64 curpos, uint64 newpos) {
    int current_piece = 0;
    for (int i = 0; i < 6; i++) {
        if(game->positions.StatePerPiece[i+game->turn*6]&curpos) {
            current_piece = i;
            break;
        }
    }
    Game temp_game = *game;

    if (newpos&check_legal_moves(game, curpos, current_piece)) {
        temp_game.positions.StatePerSide[game->turn] -= curpos;
        temp_game.positions.StatePerPiece[current_piece+game->turn*6] -=curpos;
        temp_game.positions.StatePerSide[game->turn] += newpos;
        temp_game.positions.StatePerPiece[current_piece+game->turn*6] +=newpos;
        temp_game.positions.StatePerSide[2] -=curpos;
        temp_game.positions.StatePerSide[2] += newpos;

        for (int i = 0; i < 6; i++) {
            if (temp_game.positions.StatePerPiece[i+!game->turn*6]&newpos) {
                temp_game.positions.StatePerPiece[i+!game->turn*6] -= newpos;
                temp_game.positions.StatePerSide[!game->turn] -= newpos;
                temp_game.positions.StatePerSide[2] -=newpos;
            }}
        *game = temp_game;
        game->turn = !game->turn;
        return 1;
    }

    return 0;
}

void print_board(uint64 position) {
    printf("\n");
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            char dig = ' ';
            uint64 mask = (uint64)1 << ((rank*8) + file);
            if (position&mask) {
                dig = '1';
            } else {
                dig = '0';
            }
            printf("%c ", dig);
        }
        printf("\n");
    }
}