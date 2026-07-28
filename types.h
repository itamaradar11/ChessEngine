#ifndef TYPES
#define TYPES

#include <string>
#include <cstdint>

constexpr int MAX_MOVES = 256;

using Bitboard = std::uint64_t;

enum Color : std::uint8_t {WHITE, BLACK};
//enum PieceType {PAWN, KNIGHT, BISHOP, QUEEN, KING, NONE};
enum ColoredPieceType : std::uint8_t {WHITE_PAWN, WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, WHITE_KING, BLACK_PAWN, BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, BLACK_KING, NONE_PEICE};
enum Square : std::uint8_t {
    A1, A2, A3, A4, A5, A6, A7, A8,
    B1, B2, B3, B4, B5, B6, B7, B8,
    C1, C2, C3, C4, C5, C6, C7, C8,
    D1, D2, D3, D4, D5, D6, D7, D8,
    E1, E2, E3, E4, E5, E6, E7, E8,
    F1, F2, F3, F4, F5, F6, F7, F8,
    G1, G2, G3, G4, G5, G6, G7, G8,
    H1, H2, H3, H4, H5, H6, H7, H8,
    NONE_SQUARE
};
enum MoveFlag : std::uint8_t {QUIET, DOUBLE_PAWN_PUSH, KING_CASTLE, QUEEN_CASTLE, CAPTURE, EN_PASSANT, PROMOTION, CAPTURE_PROMOTION};

struct Move
{
    Square from; //piece currently in 'from'
    Square to; //move it to 'to'
    ColoredPieceType promotion_piece; //if move is a promotion, to what should promote
    MoveFlag flag; //flag for special moves
};

ColoredPieceType char_to_piece(char c);
Square calc_square(int file, int rank);
Square calc_square(const std::string& sq);
#endif
