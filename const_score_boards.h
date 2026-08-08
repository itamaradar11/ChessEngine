#ifndef SCORE_BOARDS
#define SCORE_BOARDS

#include <array>
#include "types.h"

//WHITE BOARDS
constexpr int MG_WHITE_PAWN_TABLE[64] = {
    0, -35, -26, -27, -14, -6, 98, 0,
    0, -1, -4, -2, 13, 7, 134, 0,
    0, -20, -4, -5, 6, 26, 61, 0,
    0, -23, -10, 12, 21, 31, 95, 0,
    0, -15, 3, 17, 23, 65, 68, 0,
    0, 24, 3, 6, 12, 56, 126, 0,
    0, 38, 33, 10, 17, 25, 34, 0,
    0, -22, -12, -25, -23, -20, -11, 0
};

constexpr int MG_WHITE_KNIGHT_TABLE[64] = {
    -105, -29, -23, -13, -9, -47, -73, -167,
    -21, -53, -9, 4, 17, 60, -41, -89,
    -58, -12, 12, 16, 19, 37, 72, -34,
    -33, -3, 10, 13, 53, 65, 36, -49,
    -17, -1, 19, 28, 37, 84, 23, 61,
    -28, 18, 17, 19, 69, 129, 62, -97,
    -19, -14, 25, 21, 18, 73, 7, -15,
    -23, -19, -16, -8, 22, 44, -17, -107
};
 
constexpr int MG_WHITE_BISHOP_TABLE[64] = {
    -33, 4, 0, -6, -4, -16, -26, -29,
    -3, 15, 15, 13, 5, 37, 16, 4,
    -14, 16, 15, 13, 19, 43, -18, -82,
    -21, 0, 15, 26, 50, 40, -13, -37,
    -13, 7, 14, 34, 37, 35, 30, -25,
    -12, 21, 27, 12, 37, 50, 59, -42,
    -39, 33, 18, 10, 7, 37, 18, 7,
    -21, 1, 10, 4, -2, -2, -47, -8
};
 
constexpr int MG_WHITE_ROOK_TABLE[64] = {
    -19, -44, -45, -36, -24, -5, 27, 32,
    -13, -16, -25, -26, -11, 19, 32, 42,
    1, -20, -16, -12, 7, 26, 58, 32,
    17, -9, -17, -1, 26, 36, 62, 51,
    16, -1, 3, 9, 24, 17, 80, 63,
    7, 11, 0, -7, 35, 45, 67, 9,
    -37, -6, -5, 6, -8, 61, 26, 31,
    -26, -71, -33, -23, -20, 16, 44, 43
};
 
constexpr int MG_WHITE_QUEEN_TABLE[64] = {
    -1, -35, -14, -9, -27, -13, -24, -28,
    -18, -8, 2, -26, -27, -17, -39, 0,
    -9, 11, -11, -9, -16, 7, -5, 29,
    10, 2, -2, -10, -16, 8, 1, 12,
    -15, 8, -5, -2, -1, 29, -16, 59,
    -25, 15, 2, -4, 17, 56, 57, 44,
    -31, -3, 14, 3, -2, 47, 28, 43,
    -50, 1, 5, -3, 1, 57, 54, 45
};
 
constexpr int MG_WHITE_KING_TABLE[64] = {
    -15, 1, -14, -49, -17, -9, 29, -65,
    36, 7, -14, -1, -20, 24, -1, 23,
    12, -8, -22, -27, -12, 2, -20, 16,
    -54, -64, -46, -39, -27, -16, -7, -15,
    8, -43, -44, -46, -30, -20, -8, -56,
    -28, -16, -30, -44, -25, 6, -4, -34,
    24, 9, -15, -33, -14, 22, -38, 2,
    14, 8, -27, -51, -36, -22, -29, 13
};


constexpr int EG_WHITE_PAWN_TABLE[64] = {
    0, 13, 4, 13, 32, 94, 178, 0,
    0, 8, 7, 9, 24, 100, 173, 0,
    0, 8, -6, -3, 13, 85, 158, 0,
    0, 10, 1, -7, 5, 67, 134, 0,
    0, 13, 0, -7, -2, 56, 147, 0,
    0, 0, -5, -8, 4, 53, 132, 0,
    0, 2, -1, 3, 17, 82, 165, 0,
    0, -7, -8, -1, 17, 84, 187, 0
};
 
constexpr int EG_WHITE_KNIGHT_TABLE[64] = {
    -29, -42, -23, -18, -17, -24, -25, -58,
    -51, -20, -3, -6, 3, -20, -8, -38,
    -23, -10, -1, 16, 22, 10, -25, -13,
    -15, -5, 15, 25, 22, 9, -2, -28,
    -22, -2, 10, 16, 22, -1, -9, -31,
    -18, -20, -3, 17, 11, -9, -25, -27,
    -50, -23, -20, 4, 8, -19, -24, -63,
    -64, -44, -22, -18, -18, -41, -52, -99
};
 
constexpr int EG_WHITE_BISHOP_TABLE[64] = {
    -23, -14, -12, -6, -3, 2, -8, -14,
    -9, -18, -3, 3, 9, -8, -4, -21,
    -23, -7, 8, 13, 12, 0, 7, -11,
    -5, -1, 10, 19, 9, -1, -12, -8,
    -9, 4, 13, 7, 14, -2, -3, -7,
    -16, -9, 3, 10, 10, 6, -13, -9,
    -5, -15, -7, -3, 3, 0, -4, -17,
    -17, -27, -15, -9, 2, 4, -14, -24
};
 
constexpr int EG_WHITE_ROOK_TABLE[64] = {
    -9, -6, -4, 3, 4, 7, 11, 13,
    2, -6, 0, 5, 3, 7, 13, 10,
    3, 0, -5, 8, 13, 7, 13, 18,
    -1, 2, -1, 4, 1, 5, 11, 15,
    -5, -9, -7, -5, 2, 4, -3, 12,
    -13, -9, -12, -6, 1, -3, 3, 12,
    4, -11, -8, -8, -1, -5, 8, 8,
    -20, -3, -16, -11, 2, -3, 3, 5
};
 
constexpr int EG_WHITE_QUEEN_TABLE[64] = {
    -33, -22, -16, -18, 3, -20, -17, -9,
    -28, -23, -27, 28, 22, 6, 20, 22,
    -22, -30, 15, 19, 24, 9, 32, 22,
    -43, -16, 6, 47, 45, 49, 41, 27,
    -5, -16, 9, 31, 57, 47, 58, 27,
    -32, -23, 17, 34, 40, 35, 25, 19,
    -20, -36, 10, 39, 57, 19, 30, 10,
    -41, -32, 5, 23, 36, 9, 0, 20
};
 
constexpr int EG_WHITE_KING_TABLE[64] = {
    -53, -27, -19, -18, -8, 10, -12, -74,
    -34, -11, -3, -4, 22, 17, 17, -35,
    -21, 4, 11, 21, 24, 23, 14, -18,
    -11, 13, 21, 24, 27, 15, 17, -18,
    -28, 14, 23, 27, 26, 20, 17, -11,
    -14, 4, 16, 23, 33, 45, 38, 15,
    -24, -5, 7, 9, 26, 44, 23, 4,
    -43, -17, -9, -11, 3, 13, 11, -17
};


//BLACK BOARDS
constexpr std::array<int, 64> flip_board(const int (&white_board)[64]){
    std::array<int, 64> black_board = {};

    for(int row = 0; row < 8; row++){
        for(int file = 0; file < 8; file++){
            //For B7 - row=6 (7), file=1 (B)
            int white_idx = file*8 + row; //B7 = 1*8 + 6 = 14
            int black_idx = file*8 + (7-row); //B2 = 1*8 + (7-6) = 9
            black_board[black_idx] = white_board[white_idx];
        }
    }

    return black_board;
}

constexpr auto MG_BLACK_PAWN_TABLE = flip_board(MG_WHITE_PAWN_TABLE);
constexpr auto MG_BLACK_KNIGHT_TABLE = flip_board(MG_WHITE_KNIGHT_TABLE);
constexpr auto MG_BLACK_BISHOP_TABLE = flip_board(MG_WHITE_BISHOP_TABLE);
constexpr auto MG_BLACK_ROOK_TABLE = flip_board(MG_WHITE_ROOK_TABLE);
constexpr auto MG_BLACK_QUEEN_TABLE = flip_board(MG_WHITE_QUEEN_TABLE);
constexpr auto MG_BLACK_KING_TABLE = flip_board(MG_WHITE_KING_TABLE);
constexpr auto EG_BLACK_PAWN_TABLE = flip_board(EG_WHITE_PAWN_TABLE);
constexpr auto EG_BLACK_KNIGHT_TABLE = flip_board(EG_WHITE_KNIGHT_TABLE);
constexpr auto EG_BLACK_BISHOP_TABLE = flip_board(EG_WHITE_BISHOP_TABLE);
constexpr auto EG_BLACK_ROOK_TABLE = flip_board(EG_WHITE_ROOK_TABLE);
constexpr auto EG_BLACK_QUEEN_TABLE = flip_board(EG_WHITE_QUEEN_TABLE);
constexpr auto EG_BLACK_KING_TABLE = flip_board(EG_WHITE_KING_TABLE);


//TABLE
constexpr const int* MG_TABLES[] = {
    MG_WHITE_PAWN_TABLE, MG_WHITE_KNIGHT_TABLE, MG_WHITE_BISHOP_TABLE,
    MG_WHITE_ROOK_TABLE, MG_WHITE_QUEEN_TABLE, MG_WHITE_KING_TABLE,
    MG_BLACK_PAWN_TABLE.data(), MG_BLACK_KNIGHT_TABLE.data(), MG_BLACK_BISHOP_TABLE.data(),
    MG_BLACK_ROOK_TABLE.data(), MG_BLACK_QUEEN_TABLE.data(), MG_BLACK_KING_TABLE.data(),
};

constexpr const int* EG_TABLES[] = {
    EG_WHITE_PAWN_TABLE, EG_WHITE_KNIGHT_TABLE, EG_WHITE_BISHOP_TABLE,
    EG_WHITE_ROOK_TABLE, EG_WHITE_QUEEN_TABLE, EG_WHITE_KING_TABLE,
    EG_BLACK_PAWN_TABLE.data(), EG_BLACK_KNIGHT_TABLE.data(), EG_BLACK_BISHOP_TABLE.data(),
    EG_BLACK_ROOK_TABLE.data(), EG_BLACK_QUEEN_TABLE.data(), EG_BLACK_KING_TABLE.data(),
};

#endif