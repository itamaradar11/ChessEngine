#include "board.h"

int main(){
    Board board = Board();
    // board.setup_start_position();
    // board.print_board();
    board.load_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Kk - 0 1");
    board.load_fen("r3k2r/8/8/8/8/8/8/R3K2R b qKQ e4 143 17");
    board.print_board();

    return 0;
}