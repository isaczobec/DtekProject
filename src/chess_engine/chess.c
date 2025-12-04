#include "chess.h"
#include "../io/io.h"

//shorthand for checking one square in each direction
const int32 UP = -8;
const int32 DOWN = 8;
const int32 RIGHT = 1;
const int32 LEFT = -1;
const int32 RUP = -7;
const int32 LUP = -9;
const int32 RDOWN = 9;
const int32 LDOWN = 7;

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
    if (shift >= 0) {
        return value << shift;
    } else {
        return value >> -shift;
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

int not_check_check(Game *game, uint64 curpos) {
    for (int i = 0; i < *(&all_pieces+1) - all_pieces; i++) {
        if(i != 5) {
            if(check_legal_moves(game, curpos, i)&game->positions.StatePerPiece[6*!game->turn+i]) {
                return 0;
            }
        }
    } 
    return 1;
}

int moves_out_of_check(Game *game, uint64 curpos, uint64 newpos, uint64 current_piece) {
    Game temp_game = *game;
    if (game->gamestate == Check) {
        temp_game.gamestate = InProgress;
        temp_game.positions.StatePerPiece[current_piece] += newpos;
        temp_game.positions.StatePerSide[temp_game.turn] += newpos;
        temp_game.positions.StatePerSide[2]+=newpos;
        temp_game.positions.StatePerPiece[current_piece] -= curpos;
        temp_game.positions.StatePerSide[temp_game.turn] -= curpos;
        temp_game.positions.StatePerSide[2] -= curpos;
        int opp_piece = 0;
        for (int i = 0; i < 6; i++) {
            if(temp_game.positions.StatePerPiece[i+!temp_game.turn*6]&newpos) {
                opp_piece = i+6*!temp_game.turn;
                break;
        }
        if (temp_game.positions.StatePerSide[!temp_game.turn]&newpos) {
            temp_game.positions.StatePerSide[opp_piece] -= newpos;
            temp_game.positions.StatePerSide[!temp_game.turn] -= newpos;
        }
    }
        if(not_check_check(&temp_game, temp_game.positions.StatePerPiece[6*temp_game.turn+KING])) {
            return 1;
        }
            return 0;
    }
    return 1;
}



int takes_piece(Game *game, uint64 check_pos) {
    if(game->positions.StatePerSide[!game->turn]&check_pos) {
        return 1;
    }
    return 0;
}

uint64 check_direction(Game *game, uint64 curpos, int32 direction, int32 is_recursive, int32 has_recursed, int32 current_piece) {
    uint64 legal_moves = 0;
    if (not_blocked_by_piece(game, shift(curpos, direction)) && not_outside(curpos, direction)) {

        if (!has_recursed) {
            if(!not_revealed_check_check(game, curpos, direction, current_piece)) {
                return legal_moves;
        }}

        if (moves_out_of_check(game, curpos, shift(curpos, direction), current_piece)) {
            legal_moves += shift(curpos, direction);
        }

        if (takes_piece(game, shift(curpos, direction))) {
            return legal_moves;
        }
        if(is_recursive && shift(curpos, direction)) {
        legal_moves += check_direction(game, shift(curpos, direction), direction, is_recursive, 1, current_piece);
    }
    }
    return legal_moves;
}

int not_revealed_check_check(Game *game, uint64 curpos, uint64 direction, int32 current_piece) {
    for (int i = 0; i < *(&ANY+1) - ANY; i++) {
        if (i != direction && -i != direction) {
            if (i < 4 && 
                check_direction(game, curpos, i, 0, 1, current_piece)&(game->positions.StatePerPiece[6*!game->turn+ROOK]|game->positions.StatePerPiece[6*!game->turn+QUEEN]) &&
                check_direction(game, curpos, -i, 0, 1, current_piece)&shift(game->positions.StatePerPiece[6*game->turn+KING], i) && not_outside(game->positions.StatePerPiece[6*game->turn+KING], i)) {
                    return 0;
            } else if (i > 3 &&
                check_direction(game, curpos, i, 0, 1, current_piece)&(game->positions.StatePerPiece[6*!game->turn+BISHOP]|game->positions.StatePerPiece[6*!game->turn+QUEEN]) &&
                check_direction(game, curpos, -i, 0, 1, current_piece)&shift(game->positions.StatePerPiece[6*game->turn+KING], i) && not_outside(game->positions.StatePerPiece[6*game->turn+KING], i)) {
                    return 0;
            }
        }
    }
    return 1;
}

uint64 check_king_moves(Game *game, uint64 curpos) {
    uint64 legal_moves = 0;
    Game temp_game = *game;

    if(game->castling[game->turn]) {
        if(not_check_check(game, curpos) && not_check_check(game, shift(curpos, RIGHT+2*game->turn*LEFT))
            && not_check_check(game, shift(curpos, 2*(RIGHT+2*game->turn*LEFT))) &&
            !(game->positions.StatePerSide[2]&shift(curpos, RIGHT+2*game->turn*LEFT)) &&
            !(game->positions.StatePerSide[2]&shift(curpos, 2*(RIGHT+2*game->turn*LEFT)))) {
                legal_moves += shift(curpos, 2*(RIGHT+2*game->turn*LEFT));
            }
    }
    if(game->castling[game->turn+2]) {
            if(not_check_check(game, curpos) && not_check_check(game, shift(curpos, LEFT+2*game->turn*RIGHT))
            && not_check_check(game, shift(curpos, 2*(LEFT+2*game->turn*RIGHT))) &&
            !(game->positions.StatePerSide[2]&shift(curpos, LEFT+2*game->turn*RIGHT)) &&
            !(game->positions.StatePerSide[2]&shift(curpos, 2*(LEFT+2*game->turn*RIGHT))) &&
            !(game->positions.StatePerSide[2]&shift(curpos, 3*(LEFT+2*game->turn*RIGHT)))) {
                legal_moves += shift(curpos, 2*(LEFT+2*game->turn*RIGHT));
            }
    }
    temp_game.positions.StatePerPiece[game->turn+KING] -= curpos;
    temp_game.positions.StatePerSide[game->turn] -= curpos;
    temp_game.positions.StatePerSide[2] -= curpos;
    for (int i = 0; i < *(&ANY+1) - ANY; i++) {
        if(not_check_check(&temp_game, shift(curpos, ANY[i]))) {
            legal_moves += check_direction(game, curpos, ANY[i], 0, 0, 6*game->turn+KING);
        }
    }
    return legal_moves;
}

uint64 check_queen_moves(Game *game, uint64 curpos) {
    uint64 legal_moves = 0;
    for (int i = 0; i < *(&ANY+1) - ANY; i++) {
        legal_moves += check_direction(game, curpos, ANY[i], 1, 0, 6*game->turn+QUEEN);
    }
    return legal_moves;
}

uint64 check_rook_moves(Game *game, uint64 curpos) {
    uint64 legal_moves = 0;
    for (int i = 0; i < *(&STRAIGHT+1) - STRAIGHT; i++) {
        legal_moves += check_direction(game, curpos, STRAIGHT[i], 1, 0, 6*game->turn+ROOK);
    }
    return legal_moves;
}

uint64 check_bishop_moves(Game *game, uint64 curpos) {
    uint64 legal_moves = 0;
    for (int i = 0; i < *(&DIAGONAL+1) - DIAGONAL; i++) {
        legal_moves += check_direction(game, curpos, DIAGONAL[i], 1, 0, 6*game->turn+BISHOP);
    }
    return legal_moves;
}

uint64 check_pawn_moves(Game *game, uint64 curpos) {
    uint64 legal_moves = 0;
    if (not_blocked_by_piece(game, shift(curpos, 2*game->turn*DOWN+UP)) &&
        !(game->positions.StatePerSide[!game->turn]&shift(curpos, 2*game->turn*DOWN+UP)) &&
        not_revealed_check_check(game, curpos, 2*game->turn*DOWN+UP, 6*game->turn+PAWN)) {
        if(moves_out_of_check(game, curpos, shift(curpos, 2*game->turn*DOWN+UP), 6*game->turn+PAWN)) {
            legal_moves += shift(curpos, 2*game->turn*DOWN+UP);
        }


        if((file_2&curpos || file_7&curpos) && not_blocked_by_piece(game, shift(curpos, (2*game->turn*DOWN+UP)*2)) &&
            !(game->positions.StatePerSide[!game->turn]&shift(curpos, (2*game->turn*DOWN+UP)*2)) &&
            moves_out_of_check(game, curpos, shift(curpos, (2*game->turn*DOWN+UP)*2), 6*game->turn+PAWN)) {
            legal_moves += shift(curpos, (2*game->turn*DOWN+UP)*2);
        }
    }
    for (int i = 0; i < *(&RUPLUP+1) - RUPLUP; i++) {
        if((game->positions.StatePerSide[!game->turn]&shift(curpos, -2*game->turn*RUPLUP[i]+RUPLUP[i]) ||
           game->positions.EnPassantPerSide[!game->turn]&shift(curpos, -2*game->turn*RUPLUP[i]+RUPLUP[i])) &&
           not_outside(curpos, RUPLUP[i]) &&
            not_revealed_check_check(game, curpos, -2*game->turn*RUPLUP[i]+RUPLUP[i], 6*game->turn+PAWN) &&
            moves_out_of_check(game, curpos, shift(curpos, -2*game->turn*RUPLUP[i]+RUPLUP[i]), 6*game->turn+PAWN)) {
            legal_moves += shift(curpos, -2*game->turn*RUPLUP[i]+RUPLUP[i]);
        }
    }
    return legal_moves;
}

uint64 check_knight_moves(Game *game, uint64 curpos) {
    uint64 legal_moves = 0;
    for (int i = 0; i < *(&HORSE+1) - HORSE; i++) {
        if(horse_block(curpos, HORSE[i]) &&
            not_revealed_check_check(game, curpos, HORSE[i], 6*game->turn+KNIGHT) &&
            moves_out_of_check(game, curpos, shift(curpos, HORSE[i]), 6*game->turn+KNIGHT)) {
            legal_moves += shift(curpos, HORSE[i]);
        }
    }
    return legal_moves;
}

uint64 check_legal_moves(Game *game, uint64 curpos, int current_piece) {

    if(&current_piece == NULL) {
    for (int i = 0; i < 6; i++) {
        if(game->positions.StatePerPiece[i+game->turn*6]&curpos) {
            current_piece = i;
            break;
        }
    }
    } else {
        current_piece -= 6*game->turn;
    }
    
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
            current_piece = i+6*game->turn;
            break;
        }
    }
    Game temp_game = *game;
    if (newpos&check_legal_moves(game, curpos, current_piece)) {
        temp_game.positions.StatePerSide[game->turn] -= curpos;
        temp_game.positions.StatePerPiece[current_piece] -=curpos;
        temp_game.positions.StatePerSide[game->turn] += newpos;
        temp_game.positions.StatePerPiece[current_piece] +=newpos;
        temp_game.positions.StatePerSide[2] -=curpos;
        temp_game.positions.StatePerSide[2] += newpos;


        if(current_piece == PAWN && shift(curpos, 2*(2*game->turn*DOWN+UP))&newpos) {
            temp_game.positions.EnPassantPerSide[game->turn] = shift(curpos, 2*game->turn*DOWN+UP);
        } else if(current_piece == PAWN && 
                  newpos&temp_game.positions.EnPassantPerSide[!game->turn]) {
            temp_game.positions.StatePerPiece[6*!game->turn+PAWN] -= shift(newpos, (2*game->turn*UP+DOWN));
            temp_game.positions.StatePerSide[!game->turn] -= shift(newpos, (2*game->turn*UP+DOWN));
            temp_game.positions.StatePerSide[2] -= shift(newpos, (2*game->turn*UP+DOWN));   

        } else if(current_piece == KING && newpos&shift(curpos, 2*(2*LEFT*game->turn+RIGHT))) {
            temp_game.positions.StatePerSide[game->turn] -= shift((uint64)1, 63*!game->turn);
            temp_game.positions.StatePerSide[2] -= shift((uint64)1, 63*!game->turn);
            temp_game.positions.StatePerPiece[6*game->turn+ROOK] -= shift((uint64)1, 63*!game->turn);
            temp_game.positions.StatePerSide[game->turn] += shift((uint64)1, 59*!game->turn+2);
            temp_game.positions.StatePerSide[2] += shift((uint64)1, 59*!game->turn+2);
            temp_game.positions.StatePerPiece[6*game->turn+ROOK] += shift((uint64)1, 59*!game->turn+2);

        } else if(current_piece == KING && newpos&shift(curpos, 2*(2*RIGHT*game->turn+LEFT))) {
            temp_game.positions.StatePerSide[game->turn] -= shift((uint64)1, 49*!game->turn+7);
            temp_game.positions.StatePerSide[2] -= shift((uint64)1, 49*!game->turn+7);
            temp_game.positions.StatePerPiece[6*game->turn+ROOK] -= shift((uint64)1, 49*!game->turn+7);
            temp_game.positions.StatePerSide[game->turn] += shift((uint64)1, 55*!game->turn+4);
            temp_game.positions.StatePerSide[2] += shift((uint64)1, 55*!game->turn+4);
            temp_game.positions.StatePerPiece[6*game->turn+ROOK] += shift((uint64)1, 55*!game->turn+4);

        } else if(current_piece == ROOK && curpos == shift((uint64)1, 63*!game->turn)) {
            temp_game.castling[game->turn] = 0;
        } else if(current_piece == ROOK && curpos == shift((uint64)1, 49*!game->turn+7)) {
            temp_game.castling[game->turn+2] = 0;
        } else if(current_piece == KING && curpos == shift((uint64)1, 56*!game->turn+4)) {
            temp_game.castling[game->turn] = 0;
            temp_game.castling[game->turn+2] = 0;
        }
        temp_game.positions.EnPassantPerSide[!game->turn] = (uint64)0;
        for (int i = 0; i < 6; i++) {
            if (temp_game.positions.StatePerPiece[i+!game->turn*6]&newpos) {
                temp_game.positions.StatePerPiece[i+!game->turn*6] -= newpos;
                temp_game.positions.StatePerSide[!game->turn] -= newpos;
                temp_game.positions.StatePerSide[2] -=newpos;
                break;
            }}
        if(not_check_check(&temp_game, temp_game.positions.StatePerPiece[6*game->turn+KING])) {
            temp_game.gamestate = InProgress;
        } else {
            temp_game.gamestate = Check;
        }
        temp_game.turn = !temp_game.turn;
        if(!not_check_check(&temp_game, temp_game.positions.StatePerPiece[6*temp_game.turn+KING])) {
            printf("gets here");
            uint64 moves = 0;
            for(int i = 0; i < 64; i++) {
                moves|check_legal_moves(&temp_game, shift(1, i), NULL);
            }
            printf("possible moves: %llx", moves);
            if (!moves) {
                temp_game.gamestate = CheckMate;
            }
        }
            temp_game.turn = !temp_game.turn;

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
            printf("%c", dig);
        }
        printf("\n");
    }
}