#include "move_generator.h"

MoveGenerator::MoveGenerator(const Board& board): board(board), stage(NONE_STAGE){}

//Init attack tables
void MoveGenerator::init(){
    //Go over all possible centers
    for(Square center_sq = A1; center_sq <= H8; center_sq = static_cast<Square>(center_sq+1)){
        //1) Generate ROOK attack tables
        //Go over all directions - DOWN (-1), UP (1), LEFT (-8), RIGHT (8)
        int Rook_Directions[] = {-1, 1, -8, 8};
        Bitboard Rook_Edge_Masks[] = {RANK_1_MASK, RANK_8_MASK, FILE_A_MASK, FILE_H_MASK};

        for(int i=0; i<4; i++){
            int dir = Rook_Directions[i];
            Bitboard mask = Rook_Edge_Masks[i];

            Bitboard ray_cast = 0ULL;
            Square sq = center_sq;

            //1) Go from center_square outside. At each step we will add a square into the ray_cast mask
            //while did not reach the edge of the board, keep updating ray_cast
            while(!((1ULL << sq) & mask)){
                sq = static_cast<Square>(sq + dir); //next square in this direction
                ray_cast |= (1ULL << sq); //add this square into the ray_cast mask
                ROOK_ATTACK_TABLES[center_sq][sq] = ray_cast; //set current ray_cast as the value in the attcking tables
            }
            ROOK_RAY_CASTS[center_sq][i] = ray_cast;
        }

        //2) Generate BISHOP attack tables
        //Go over all directions - UP-RIGHT (9), UP-LEFT (-7), DOWN-LEFT (-9), DOWN-RIGHT (7)
        int Bishop_Directions[] = {9, -7, -9, 7};
        Bitboard Bishop_Edge_Masks[] = {RANK_8_MASK | FILE_H_MASK, RANK_8_MASK | FILE_A_MASK, RANK_1_MASK | FILE_A_MASK, RANK_1_MASK | FILE_H_MASK};

        for(int i=0; i<4; i++){
            int dir = Bishop_Directions[i];
            Bitboard mask = Bishop_Edge_Masks[i];

            Bitboard ray_cast = 0ULL;
            Square sq = center_sq;

            //1) Go from center_square outside. At each step we will add a square into the ray_cast mask
            //while did not reach the edge of the board, keep updating ray_cast
            while(!((1ULL << sq) & mask)){
                sq = static_cast<Square>(sq + dir); //next square in this direction
                ray_cast |= (1ULL << sq); //add this square into the ray_cast mask
                BISHOP_ATTACK_TABLES[center_sq][sq] = ray_cast; //set current ray_cast as the value in the attcking tables
            }
            BISHOP_RAY_CASTS[center_sq][i] = ray_cast;
        }
    }
}


//If exists unused moves in the `moves[]` array, use and return true. Else, return false.
bool MoveGenerator::get_next_move_if_exists(Move& next_move){
    if(current_index < count){
        next_move = moves[current_index];
        ++current_index;
        return true;
    }

    return false;
}

void MoveGenerator::add_move(Square from, Square to, MoveFlag flag){
    Move move = Move{from, to, NONE_PEICE, flag};
    moves[count] = move;
    ++count;
}

void MoveGenerator::add_move(Square from, Square to, MoveFlag flag, ColoredPieceType promotion){
    Move move = Move{from, to, promotion, flag};
    moves[count] = move;
    ++count;
}

void MoveGenerator::add_move(Bitboard board, int8_t offset, MoveFlag flag){
    while(board){
            Square to = get_lsb(board); //get to square
            Square from = static_cast<Square>(to + offset); //get from square
            Move move = Move{from, to, NONE_PEICE, flag};
            moves[count] = move;
            ++count;
        }
}

void MoveGenerator::generate_capture_moves_pawn(Color color){
    if(color == WHITE){
        Bitboard white_pawns = board.piece_boards[WHITE_PAWN];
        Bitboard black_pieces = board.colored_boards[BLACK];
        //calc capturing masks
        Bitboard left_captures = black_pieces & (white_pawns >> 7);
        Bitboard right_captures = black_pieces & (white_pawns << 9);

        while(left_captures){
            Square to = get_lsb(left_captures); //where captured piece is
            Square from = static_cast<Square>(to + 7); //where pawn is
            if(to % 8 == 7){ //a PROMOTION and CAPTURE
                add_move(from, to, CAPTURE_PROMOTION, WHITE_QUEEN);
                add_move(from, to, CAPTURE_PROMOTION, WHITE_ROOK);
                add_move(from, to, CAPTURE_PROMOTION, WHITE_BISHOP);
                add_move(from, to, CAPTURE_PROMOTION, WHITE_KNIGHT);
            }else{
                add_move(from, to, CAPTURE);
            }
        }
        while(right_captures){
            Square to = get_lsb(right_captures); //where captured piece is
            Square from = static_cast<Square>(to - 9); //where pawn is
            if(to % 8 == 7){ //a PROMOTION and CAPTURE
                add_move(from, to, CAPTURE_PROMOTION, WHITE_QUEEN);
                add_move(from, to, CAPTURE_PROMOTION, WHITE_ROOK);
                add_move(from, to, CAPTURE_PROMOTION, WHITE_BISHOP);
                add_move(from, to, CAPTURE_PROMOTION, WHITE_KNIGHT);
            }else{
                add_move(from, to, CAPTURE);
            }
        }

        //white en pasasnt
        Square en_passant = board.en_passant_square;
        if(en_passant == NONE_SQUARE)
            return;
        uint8_t en_passant_file = en_passant / 8; //the file (i.e. lettered-row)
        if(en_passant_file < 7 && (white_pawns & (1ULL << (en_passant+7))))
            add_move(static_cast<Square>(en_passant+7), en_passant, EN_PASSANT);
        if(en_passant_file > 0 && (white_pawns & (1ULL << (en_passant-9))))
            add_move(static_cast<Square>(en_passant-9), en_passant, EN_PASSANT);

    }else{
        Bitboard black_pawns = board.piece_boards[BLACK_PAWN];
        Bitboard white_pieces = board.colored_boards[WHITE];
        //calc capturing masks
        Bitboard left_captures = white_pieces & (black_pawns >> 9);
        Bitboard right_captures = white_pieces & (black_pawns << 7);

        while(left_captures){
            Square to = get_lsb(left_captures); //where captured piece is
            Square from = static_cast<Square>(to + 9); //where pawn is
            if(to % 8 == 0){ //a PROMOTION and CAPTURE
                add_move(from, to, CAPTURE_PROMOTION, BLACK_QUEEN);
                add_move(from, to, CAPTURE_PROMOTION, BLACK_ROOK);
                add_move(from, to, CAPTURE_PROMOTION, BLACK_BISHOP);
                add_move(from, to, CAPTURE_PROMOTION, BLACK_KNIGHT);
            }else{
                add_move(from, to, CAPTURE);
            }
        }
        while(right_captures){
            Square to = get_lsb(right_captures); //where captured piece is
            Square from = static_cast<Square>(to - 7); //where pawn is
            if(to % 8 == 0){ //a PROMOTION and CAPTURE
                add_move(from, to, CAPTURE_PROMOTION, BLACK_QUEEN);
                add_move(from, to, CAPTURE_PROMOTION, BLACK_ROOK);
                add_move(from, to, CAPTURE_PROMOTION, BLACK_BISHOP);
                add_move(from, to, CAPTURE_PROMOTION, BLACK_KNIGHT);
            }else{
                add_move(from, to, CAPTURE);
            }
        }
    
        //black en pasasnt
        Square en_passant = board.en_passant_square;
        if(en_passant == NONE_SQUARE)
            return;
        uint8_t en_passant_file = en_passant / 8; //the file (i.e. lettered-row)
        if(en_passant_file < 7 && (black_pawns & (1ULL << (en_passant+9))))
            add_move(static_cast<Square>(en_passant+9), en_passant, EN_PASSANT);
        if(en_passant_file > 0 && (black_pawns & (1ULL << (en_passant-7))))
            add_move(static_cast<Square>(en_passant-7), en_passant, EN_PASSANT);

    }
}

void MoveGenerator::generate_capture_moves_knight(Color color){
    Bitboard knights = color == WHITE ? board.piece_boards[WHITE_KNIGHT] : board.piece_boards[BLACK_KNIGHT];
    Bitboard enemy_pieces = color == WHITE ? board.colored_boards[BLACK] : board.colored_boards[WHITE];
    //calc capturing masks (with clock direction)
    Bitboard captures_1 = enemy_pieces & (knights >> 10) & NOT_RANK_78_MASK;
    Bitboard captures_2 = enemy_pieces & (knights >> 17) & NOT_RANK_8_MASK;
    Bitboard captures_3 = enemy_pieces & (knights >> 15) & NOT_RANK_1_MASK;
    Bitboard captures_4 = enemy_pieces & (knights >> 6) & NOT_RANK_12_MASK;
    Bitboard captures_5 = enemy_pieces & (knights << 10) & NOT_RANK_12_MASK;
    Bitboard captures_6 = enemy_pieces & (knights << 17) & NOT_RANK_1_MASK;
    Bitboard captures_7 = enemy_pieces & (knights << 15) & NOT_RANK_8_MASK;
    Bitboard captures_8 = enemy_pieces & (knights << 6) & NOT_RANK_78_MASK;
    //add moves
    add_move(captures_1, 10, CAPTURE);
    add_move(captures_2, 17, CAPTURE);
    add_move(captures_3, 15, CAPTURE);
    add_move(captures_4, 6, CAPTURE);
    add_move(captures_5, -10, CAPTURE);
    add_move(captures_6, -17, CAPTURE);
    add_move(captures_7, -15, CAPTURE);
    add_move(captures_8, -6, CAPTURE);
}

void MoveGenerator::generate_capture_moves_king(Color color){
    Bitboard king = color == WHITE ? board.piece_boards[WHITE_KING] : board.piece_boards[BLACK_KING];
    Bitboard enemy_pieces = color == WHITE ? board.colored_boards[BLACK] : board.colored_boards[WHITE];
    //calc capturing masks (with clock direction)
    Bitboard captures_1 = enemy_pieces & (king >> 1) & NOT_RANK_8_MASK; //up
    Bitboard captures_2 = enemy_pieces & (king >> 9) & NOT_RANK_8_MASK; //up right
    Bitboard captures_3 = enemy_pieces & (king >> 8); //right
    Bitboard captures_4 = enemy_pieces & (king >> 7) & NOT_RANK_1_MASK; //down right
    Bitboard captures_5 = enemy_pieces & (king << 1) & NOT_RANK_1_MASK; //down
    Bitboard captures_6 = enemy_pieces & (king << 9) & NOT_RANK_1_MASK; //down left
    Bitboard captures_7 = enemy_pieces & (king << 8); //left
    Bitboard captures_8 = enemy_pieces & (king << 7) & NOT_RANK_8_MASK; //up left
    //add moves
    add_move(captures_1, 1, CAPTURE);
    add_move(captures_2, 9, CAPTURE);
    add_move(captures_3, 8, CAPTURE);
    add_move(captures_4, 7, CAPTURE);
    add_move(captures_5, -1, CAPTURE);
    add_move(captures_6, -9, CAPTURE);
    add_move(captures_7, -8, CAPTURE);
    add_move(captures_8, -7, CAPTURE);
}

void MoveGenerator::generate_capture_moves(){
    Color side = board.side_to_move;
}

bool MoveGenerator::step(Move& next_move){
    //if there exists unused moves
    if(get_next_move_if_exists(next_move)){
        return true;
    }

    if(stage == NONE_STAGE){
        generate_capture_moves();
        //try to return a capture move (will only fail if could not generare any capturing moves)
        if(get_next_move_if_exists(next_move)){
            return true;
        }
    }
    if(stage == CAPTURE){
        generate_quiet_moves();
        //try to return a quiet move (will only fail if could not generare any quiet moves)
        if(get_next_move_if_exists(next_move)){
            return true;
        }
    }
    return false; //no moves left to generate
}