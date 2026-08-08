#include "board.h"

int main(){
    Board board = Board();
    // board.load_fen("r3k2r/8/8/8/8/8/8/R3K2R b qKQ e4 143 17");
    board.play_game();
    return 0;
}