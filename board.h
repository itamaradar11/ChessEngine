#ifndef BOARD
#define BOARD

#include "types.h"

class Board {
public:
    Board();
    void setup_start_position();
    ColoredPieceType get_peice_at(Square sq);
    void load_fen(const std::string& fen);
    void print_board();

private:
    Bitboard piece_boards[12]; //6 piece types X 2 colors
    Color side_to_move;
    //tells which castling move is leagal (i.e. both rook and king did not move yet)
    //we use only the 4 LSBs of castling_rights (in line below x means "garbage value/unused")
    //castling_rights = [x,x,x,x, WHITE-KINGSIDE, WHITE_QUEENSIDE, BLACK-KINGSIDE, BLACK_QUEENSIDE]
    uint8_t castling_rights;
    Square en_passant_square;
    int half_moves;
    int full_moves;
};

#endif