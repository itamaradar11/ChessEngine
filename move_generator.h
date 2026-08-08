#ifndef GENERATOR
#define GENERATOR

#include "types.h"
#include "board.h"
#include "bitboard.h"

class MoveGenerator{
public:
    MoveGenerator(const Board& board);
    bool step(Move& next_move);
    bool is_square_attacked(Square sq, Color attacked_by);
    void init();

private:
    enum Stage {NONE_STAGE, CAPTURES, QUIETS};
    Stage stage; //in what stage our generator is currently in
    const Board& board;
    Move moves[MAX_MOVES];
    uint8_t count = 0; //amount of moves generated
    uint8_t current_index = 0; //in what move are we so far

    static Bitboard ROOK_ATTACK_TABLES[64][64];
    static Bitboard ROOK_RAY_CASTS[64][4];
    static Bitboard BISHOP_ATTACK_TABLES[64][64];
    static Bitboard BISHOP_RAY_CASTS[64][4];

    void add_move(Square from, Square to, MoveFlag flag);
    void add_move(Square from, Square to, MoveFlag flag, ColoredPieceType promotion);
    void add_move(Bitboard board, int8_t offset, MoveFlag flag);
    void add_move(Bitboard board, Square from, MoveFlag flag);
    bool get_next_move_if_exists(Move& next_move);
    Bitboard get_rook_attacks(Square sq, Bitboard occupied);
    Bitboard get_bishop_attacks(Square sq, Bitboard occupied);
    void generate_capture_moves();
    void generate_capture_moves_pawn(Color color);
    void generate_capture_moves_rook(Color color);
    void generate_capture_moves_bishop(Color color);
    void generate_capture_moves_knight(Color color);
    void generate_capture_moves_queen(Color color);
    void generate_capture_moves_king(Color color);
    void generate_quiet_moves();
    void generate_quiet_moves_pawn(Color color);
    void generate_quiet_moves_rook(Color color);
    void generate_quiet_moves_bishop(Color color);
    void generate_quiet_moves_knight(Color color);
    void generate_quiet_moves_queen(Color color);
    void generate_quiet_moves_king(Color color);
};
    
#endif