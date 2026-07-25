#include "types.h"

ColoredPieceType char_to_piece(char c) {
    switch (c) {
        case 'P': return WHITE_PAWN;
        case 'N': return WHITE_KNIGHT;
        case 'B': return WHITE_BISHOP;
        case 'R': return WHITE_ROOK;
        case 'Q': return WHITE_QUEEN;
        case 'K': return WHITE_KING;
        
        case 'p': return BLACK_PAWN;
        case 'n': return BLACK_KNIGHT;
        case 'b': return BLACK_BISHOP;
        case 'r': return BLACK_ROOK;
        case 'q': return BLACK_QUEEN;
        case 'k': return BLACK_KING;

        default: return NONE_PEICE;
    }
}

/*
Given file (letterd row) and rank (numerical row), returns the corresponding enum Square
*/
Square calc_square(int file, int rank){
    return static_cast<Square>(file*8 + rank);
}

/*
Given string represnting a square (e.g. e1, g5), returns the corresponding enum Square
*/
Square calc_square(const std::string& sq){
    if(sq=="-")
        return NONE_SQUARE;

    char file = sq[0];
    char rank = sq[1];

    //'a' -> 0, 'b' -> 1, ..., 'h' -> 7
    int file_num = file - 'a';

    //'1' - >0, '2' -> 1, ..., '8' -> 7
    int rank_num = rank - '1';

    int sq_num = file_num*8 + rank_num;
    return static_cast<Square>(sq_num);
}