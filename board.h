#ifndef BOARD
#define BOARD

#include "types.h"

class Board {
public:
    Board();
    void setup_start_position();
    ColoredPieceType get_piece_at(Square sq);
    ColoredPieceType get_piece_at(Square sq, Color color);
    void load_fen(const std::string& fen);
    void print_board();
    void make_move(const Move& move, StateInfo& state);
    void unmake_move(const Move& move, const StateInfo& state);
    Move search_best_move();
    int search_max(int depth, int alpha, int beta);
    int search_min(int depth, int alpha, int beta);

    friend class MoveGenerator;

private:
    Bitboard piece_boards[12]; //6 piece types X 2 colors
    Bitboard colored_boards[2]; //boards specifiying if each square is occupied by a white/black piece
    Bitboard occupied_board; //a single board specifiying if each square is occupied by a piece (does not care about the color)
    Color side_to_move;
    //tells which castling move is leagal (i.e. both rook and king did not move yet)
    //we use only the 4 LSBs of castling_rights (in line below x means "garbage value/unused")
    //castling_rights = [x,x,x,x, WHITE-KINGSIDE, WHITE_QUEENSIDE, BLACK-KINGSIDE, BLACK_QUEENSIDE]
    uint8_t castling_rights;
    Square en_passant_square;
    int half_moves;
    int full_moves;
    Color engine_color;
    StateInfo states[MAX_DEPTH];
};

#endif