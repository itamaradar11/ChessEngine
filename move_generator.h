#ifndef GENERATOR
#define GENERATOR

#include "types.h"
#include "board.h"

class MoveGenerator{
public:
    MoveGenerator(const Board& board);
    bool step(const Move& next_move);
private:
    enum Stage {CAPTURES, QUIETS};
    Stage stage; //in what stage our generator is currently in
    const Board& board;
    Move moves[MAX_MOVES];
    uint8_t count = 0; //amount of moves generated
    uint8_t current_index = 0; //in what move are we so far
};
    
#endif