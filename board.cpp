#include <algorithm>
#include <iostream>
#include <sstream>
#include <bitset>
#include "board.h"
#include "move_generator.h"

Board::Board(){
    //init all bitboards to 0
    std::fill(std::begin(piece_boards), std::end(piece_boards), 0);
    colored_boards[WHITE] = 0;
    colored_boards[BLACK] = 0;
    occupied_board = 0;

    side_to_move = WHITE;

    //init all 4 castling options to 1
    castling_rights = 0b00001111;

    en_passant_square = NONE_SQUARE; //init to an invalid square
    half_moves = 0;
    full_moves = 1; //chess games start at 1 and not 0

    engine_color = BLACK;

    //init attack boards
    MoveGenerator generator(*this);
    generator.init();
}

void Board::setup_start_position(){
    //default values (same as in the constructor)
    side_to_move = WHITE;
    castling_rights = 0b00001111;
    en_passant_square = NONE_SQUARE;
    half_moves = 0;
    full_moves = 1;

    //WHITE PAWNS
    piece_boards[WHITE_PAWN] = 0ULL;
    for(int square = A2; square <= H2; square += 8)
        piece_boards[WHITE_PAWN] |= (1ULL << square);

    //BLACK PAWNS
    piece_boards[BLACK_PAWN] = 0ULL;
    for(int square = A7; square <= H7; square += 8)
        piece_boards[BLACK_PAWN] |= (1ULL << square);

    //ROOKS
    piece_boards[WHITE_ROOK] = 0ULL | (1ULL << A1) | (1ULL << H1);
    piece_boards[BLACK_ROOK] = 0ULL | (1ULL << A8) | (1ULL << H8);

    //BISHOPS
    piece_boards[WHITE_BISHOP] = 0ULL | (1ULL << C1) | (1ULL << F1);
    piece_boards[BLACK_BISHOP] = 0ULL | (1ULL << C8) | (1ULL << F8);

    //KNIGHTS
    piece_boards[WHITE_KNIGHT] = 0ULL | (1ULL << B1) | (1ULL << G1);
    piece_boards[BLACK_KNIGHT] = 0ULL | (1ULL << B8) | (1ULL << G8);

    //KINGS
    piece_boards[WHITE_KING] = 0ULL | (1ULL << E1);
    piece_boards[BLACK_KING] = 0ULL | (1ULL << E8);

    //QUEENS
    piece_boards[WHITE_QUEEN] = 0ULL | (1ULL << D1);
    piece_boards[BLACK_QUEEN] = 0ULL | (1ULL << D8);

    //COLORED BOARDS
    colored_boards[WHITE] = piece_boards[WHITE_PAWN] | piece_boards[WHITE_BISHOP] | piece_boards[WHITE_ROOK] | piece_boards[WHITE_KNIGHT] | piece_boards[WHITE_QUEEN] | piece_boards[WHITE_KING];
    colored_boards[BLACK] = piece_boards[BLACK_PAWN] | piece_boards[BLACK_BISHOP] | piece_boards[BLACK_ROOK] | piece_boards[BLACK_KNIGHT] | piece_boards[BLACK_QUEEN] | piece_boards[BLACK_KING];

    //OCCUPIED BOARD
    occupied_board = colored_boards[WHITE] | colored_boards[BLACK];
}

ColoredPieceType Board::get_piece_at(Square sq){
    //1) check if this square is occupied at all
    //board >> sq, moves the relevan square to the LSB
    //by doing & 1ULL we keep only the LSB
    if(!((occupied_board >> sq) & 1ULL))
        return NONE_PEICE;
    
    //2) if got here, square is occupied. Now find with what color
    Color color = (colored_boards[WHITE] >> sq) & 1ULL ? WHITE : BLACK;

    //go over all peice types
    //check for each if it occupies sq
    for(int type = color*6; type < color*6 + 6; type++){ //from color*6 to color*6 + 6, we search only relevant colored peices        
        int is_occupied = (piece_boards[type] >> sq) & 1ULL;
        if(is_occupied) //if the square was occupied by this type, return it
            return static_cast<ColoredPieceType>(type);
    }

    //I don't think it is possible to get here, but just in case
    return NONE_PEICE;
}

ColoredPieceType Board::get_piece_at(Square sq, Color color){
    //go over all peice types
    //check for each if it occupies sq
    for(int type = color*6; type < color*6 + 6; type++){ //from color*6 to color*6 + 6, we search only relevant colored peices        
        int is_occupied = (piece_boards[type] >> sq) & 1ULL;
        if(is_occupied) //if the square was occupied by this type, return it
            return static_cast<ColoredPieceType>(type);
    }

    //I don't think it is possible to get here, but just in case
    return NONE_PEICE;
}

int Board::score_board(){
    int mg_score = 0;
    int eg_score = 0;

    //calc end-game & middle-game scores
    for(int piece_type = 0; piece_type < 12; piece_type++){
        Bitboard bb = piece_boards[piece_type];
        while(bb){
            Square sq = get_lsb(bb);
            mg_score += MG_TABLES[piece_type][sq];
            eg_score += EG_TABLES[piece_type][sq];
        }
        
    }

    //calc end-game percentage (calc in what phase are we)
    //each "special" piece gets a score (knight - 1, bishop - 1, rook - 2, queen - 4)
    //max score is 24. 24 is fully middlegame. 0 is fully endgame.
    int phase = (popcount(piece_boards[WHITE_KNIGHT]) + popcount(piece_boards[BLACK_KNIGHT])
            + popcount(piece_boards[WHITE_BISHOP]) + popcount(piece_boards[BLACK_BISHOP])
            + popcount(piece_boards[WHITE_ROOK]) * 2 + popcount(piece_boards[BLACK_ROOK]) * 2
            + popcount(piece_boards[WHITE_QUEEN]) * 4 + popcount(piece_boards[BLACK_QUEEN]) * 4) / 24;

    return (phase * mg_score + (24 - phase) * eg_score) / 24;
}

void Board::load_fen(const std::string& fen){
    std::stringstream ss(fen);

    std::string peices;
    std::string side_to_move;
    std::string castling_rights;
    std::string en_passant;
    int halfmoves;
    int fullmoves;

    //split the fen to its different parts
    ss >> peices >> side_to_move >> castling_rights >> en_passant >> halfmoves >> fullmoves;

    //set the side_to_move
    if(side_to_move=="w")
        this->side_to_move = WHITE;
    else
        this->side_to_move = BLACK;

    //set en passant
    this->en_passant_square = calc_square(en_passant);
    // std::cout << en_passant_square << "en passant" << std::endl;

    //set castling rights
    this->castling_rights = 0;
    for(char c : castling_rights){
        //set the relevant bit to 1
        switch (c)
        {
        case 'K':
            this->castling_rights |= (1 << 3);
            break;
        case 'Q':
            this->castling_rights |= (1 << 2);
            break;
        case 'k':
            this->castling_rights |= (1 << 1);
            break;
        case 'q':
            this->castling_rights |= (1 << 0);
            break;
        }
    }

    //set halfmoves and fullmoves
    // std::cout << halfmoves << " || " << fullmoves << std::endl;
    this->half_moves = halfmoves;
    this->full_moves = fullmoves;

    //set the peices
    //set all boards to 0
    std::fill(std::begin(piece_boards), std::end(piece_boards), 0ULL);
    int current_square = A8; //FEN starts at A8
    for(char c : peices){
        // std::cout << "Current square is " << current_square << std::endl;
        //empty squares, just update current_square
        if('1' <= c && c <= '8'){
            current_square += 8*(c - '0'); //multiply by 8 since FEN goes over the files
            continue;
        }
        
        //new row sign '/', should not update current_square
        if(c == '/'){
            current_square -= 65; //from H8 to A7 (or H3 to A2) is 51. another 8 is cause we will add extra 8 at the previous char.
            continue;
        }

        //a peice sign (e.g. 'q', 'K', etc..)
        ColoredPieceType peice = char_to_piece(c);
        piece_boards[peice] |= (1ULL << current_square);
        current_square+=8; //every char we move to a new square
    }

    //COLORED BOARDS
    colored_boards[WHITE] = piece_boards[WHITE_PAWN] | piece_boards[WHITE_BISHOP] | piece_boards[WHITE_ROOK] | piece_boards[WHITE_KNIGHT] | piece_boards[WHITE_QUEEN] | piece_boards[WHITE_KING];
    colored_boards[BLACK] = piece_boards[BLACK_PAWN] | piece_boards[BLACK_BISHOP] | piece_boards[BLACK_ROOK] | piece_boards[BLACK_KNIGHT] | piece_boards[BLACK_QUEEN] | piece_boards[BLACK_KING];

    //OCCUPIED BOARD
    occupied_board = colored_boards[WHITE] | colored_boards[BLACK];
}

void Board::print_board(){
    //in chess: files are lettered rows (e.g., A, B, C, etc..)
    //ranks are numerical rows (e.g., 1, 2, 3, etc..)
    for(int rank = 0; rank < 8; rank++){
        for(int file = 0; file < 8; file++){
            Square sq = calc_square(file, rank);
            ColoredPieceType peice = get_piece_at(sq);

            switch (peice)
            {
            case WHITE_PAWN:
                std::cout << "P";
                break;
            case BLACK_PAWN:
                std::cout << "p";
                break;
            case WHITE_BISHOP:
                std::cout << "B";
                break;
            case BLACK_BISHOP:
                std::cout << "b";
                break;
            case WHITE_ROOK:
                std::cout << "R";
                break;
            case BLACK_ROOK:
                std::cout << "r";
                break;
            case WHITE_KNIGHT:
                std::cout << "N";
                break;
            case BLACK_KNIGHT:
                std::cout << "n";
                break;
            case WHITE_KING:
                std::cout << "K";
                break;
            case BLACK_KING:
                std::cout << "k";
                break;
            case WHITE_QUEEN:
                std::cout << "Q";
                break;
            case BLACK_QUEEN:
                std::cout << "q";
                break;
            default: //NONE, not occupied square
                std::cout << " ";
            }
            
            std::cout << "|";
        }
        std::cout << std::endl;
        std::cout << "----------------" << std::endl;
    }

    std::cout << "Side to move: " << static_cast<int>(side_to_move) << " | Castling rights: " << std::bitset<4>(castling_rights) <<
    " | en passant: " << static_cast<int>(en_passant_square) << " | halfmoves: " << half_moves << " | fullmoves " << full_moves << std::endl;
}

void Board::make_move(const Move& move, StateInfo& state){
    //save state
    state.castling_rights = castling_rights;
    state.en_passant_square = en_passant_square;
    state.half_moves = half_moves;
    //wont save captured_piece by default (only for relevant move.flag)

    if(side_to_move==BLACK)
        ++full_moves;

    if(move.flag == DOUBLE_PAWN_PUSH){
        //update en passant
        en_passant_square = side_to_move == WHITE ? static_cast<Square>(move.to - 1) : static_cast<Square>(move.to + 1);
        //update pawn board
        ColoredPieceType pawn_type = side_to_move == WHITE ? WHITE_PAWN : BLACK_PAWN;
        piece_boards[pawn_type] = (piece_boards[pawn_type] & ~(1ULL << move.from)) | (1ULL << move.to);
        //update other boards
        colored_boards[side_to_move] = (colored_boards[side_to_move] & ~(1ULL << move.from)) | (1ULL << move.to);
        occupied_board = ((occupied_board) & ~(1ULL << move.from)) | (1ULL << move.to);
        //update side and half-moves
        side_to_move = static_cast<Color>(side_to_move ^ 1);
        half_moves = 0;
        return;
    }

    if(move.flag == EN_PASSANT){
        //calculate where is the piece we capture
        Square captured_square = side_to_move == WHITE ? static_cast<Square>(move.to - 1) : static_cast<Square>(move.to + 1);

        Color other_side = static_cast<Color>(side_to_move ^ 1); //side not moving
        ColoredPieceType captured_piece = get_piece_at(captured_square, other_side);
        ColoredPieceType move_piece = side_to_move == WHITE ? WHITE_PAWN : BLACK_PAWN;

        piece_boards[move_piece] = (piece_boards[move_piece] & ~(1ULL << move.from)) | (1ULL << move.to);
        colored_boards[side_to_move] = (colored_boards[side_to_move] & ~(1ULL << move.from)) | (1ULL << move.to);

        piece_boards[captured_piece] &= ~(1ULL << captured_square);
        colored_boards[other_side] &= ~(1ULL << captured_square);

        occupied_board = (occupied_board & ~(1ULL << move.from) & ~(1ULL << captured_square)) | (1ULL << move.to);
        
        //update side and half-moves
        side_to_move = static_cast<Color>(side_to_move ^ 1);
        half_moves = 0;
        en_passant_square = NONE_SQUARE;
        state.captured_piece = captured_piece;
        return;
    }

    if(move.flag == KING_CASTLE){
        if(side_to_move == WHITE){
            //king: E1 -> G1
            piece_boards[WHITE_KING] = (piece_boards[WHITE_KING] & ~(1ULL << E1)) | (1ULL << G1);
            //rook: H1 -> F1
            piece_boards[WHITE_ROOK] = (piece_boards[WHITE_ROOK] & ~(1ULL << H1)) | (1ULL << F1);
            //update boards
            colored_boards[WHITE] = (colored_boards[WHITE] & ~(1ULL << E1) & ~(1ULL << H1)) | (1ULL << G1) | (1ULL << F1);
            occupied_board = (occupied_board & ~(1ULL << E1) & ~(1ULL << H1)) | (1ULL << G1) | (1ULL << F1);
            //remove white castling rights
            castling_rights &= 0b11110011;
        }
        else{
            //king: E8 -> G8
            piece_boards[BLACK_KING] = (piece_boards[BLACK_KING] & ~(1ULL << E8)) | (1ULL << G8);
            //rook: H8 -> F8
            piece_boards[BLACK_ROOK] = (piece_boards[BLACK_ROOK] & ~(1ULL << H8)) | (1ULL << F8);
            //update boards
            colored_boards[BLACK] = (colored_boards[BLACK] & ~(1ULL << E8) & ~(1ULL << H8)) | (1ULL << G8) | (1ULL << F8);
            occupied_board = (occupied_board & ~(1ULL << E8) & ~(1ULL << H8)) | (1ULL << G8) | (1ULL << F8);
            //remove black castling rights
            castling_rights &= 0b11111100;
        }

        //update counter and similar
        ++half_moves;
        en_passant_square = NONE_SQUARE;
        side_to_move = static_cast<Color>(side_to_move ^ 1);
        return;
    }

    if(move.flag == QUEEN_CASTLE){
        if(side_to_move == WHITE){
           //king: E1 -> C1
            piece_boards[WHITE_KING] = (piece_boards[WHITE_KING] & ~(1ULL << E1)) | (1ULL << C1);
            //rook: A1 -> D1
            piece_boards[WHITE_ROOK] = (piece_boards[WHITE_ROOK] & ~(1ULL << A1)) | (1ULL << D1);
            //update boards
            colored_boards[WHITE] = (colored_boards[WHITE] & ~(1ULL << E1) & ~(1ULL << A1)) | (1ULL << C1) | (1ULL << D1);
            occupied_board = (occupied_board & ~(1ULL << E1) & ~(1ULL << A1)) | (1ULL << C1) | (1ULL << D1);
            //remove white castling rights
            castling_rights &= 0b11110011;
        }
        else{
            //king: E8 -> C8
            piece_boards[BLACK_KING] = (piece_boards[BLACK_KING] & ~(1ULL << E8)) | (1ULL << C8);
            //rook: A8 -> D8
            piece_boards[BLACK_ROOK] = (piece_boards[BLACK_ROOK] & ~(1ULL << A8)) | (1ULL << D8);
            //update boards
            colored_boards[BLACK] = (colored_boards[BLACK] & ~(1ULL << E8) & ~(1ULL << A8)) | (1ULL << C8) | (1ULL << D8);
            occupied_board = (occupied_board & ~(1ULL << E8) & ~(1ULL << A8)) | (1ULL << C8) | (1ULL << D8);
            //remove black castling rights
            castling_rights &= 0b11111100;
        }
    
        //update counter and similar
        ++half_moves;
        en_passant_square = NONE_SQUARE;
        side_to_move = static_cast<Color>(side_to_move ^ 1);
        return;
    }

    if(move.flag == CAPTURE){
        ColoredPieceType move_piece = get_piece_at(move.from, side_to_move);

        Color other_side = static_cast<Color>(side_to_move ^ 1); //side not moving
        ColoredPieceType captured_piece = get_piece_at(move.to, other_side);

        piece_boards[move_piece] = (piece_boards[move_piece] & ~(1ULL << move.from)) | (1ULL << move.to);
        colored_boards[side_to_move] = (colored_boards[side_to_move] & ~(1ULL << move.from)) | (1ULL << move.to);

        piece_boards[captured_piece] &= ~(1ULL << move.to);
        colored_boards[other_side] &= ~(1ULL << move.to);

        occupied_board &= ~(1ULL << move.from);

        //update counter and similar
        half_moves = 0;
        en_passant_square = NONE_SQUARE;
        side_to_move = static_cast<Color>(side_to_move ^ 1);
        state.captured_piece = captured_piece;

        //update castling right of the captured side
        if(captured_piece == WHITE_ROOK){
            if(move.to == A1){ //queen side
                castling_rights &= 0b11111011;
            }else if(move.to == H1){ //king side
                castling_rights &= 0b11110111;
            }
        }
        else if(captured_piece == BLACK_ROOK){
            if(move.to == A8){ //queen side
                castling_rights &= 0b11111110;
            }else if(move.to == H8){ //king side
                castling_rights &= 0b11111101;
            }
        }
        //update castling right of the moving side
        if(move_piece == WHITE_ROOK){
            if(move.from == A1){ //queen side
                castling_rights &= 0b11111011;
            }else if(move.from == H1){ //king side
                castling_rights &= 0b11110111;
            }
        }
        else if(move_piece == BLACK_ROOK){
            if(move.from == A8){ //queen side
                castling_rights &= 0b11111110;
            }else if(move.from == H8){ //king side
                castling_rights &= 0b11111101;
            }
        }
        else if(move_piece == WHITE_KING){
            castling_rights &= 0b11110011;
        }
        else if(move_piece == BLACK_KING){
            castling_rights &= 0b11111100;
        }
        return;
    }

    if(move.flag == PROMOTION){
        //update pawn board
        ColoredPieceType pawn_type = side_to_move == WHITE ? WHITE_PAWN : BLACK_PAWN;
        piece_boards[pawn_type] &= ~(1ULL << move.from);
        //update promotion piece board
        piece_boards[move.promotion_piece] |= (1ULL << move.to);
        //update color board
        colored_boards[side_to_move] = (colored_boards[side_to_move] & ~(1ULL << move.from)) | (1ULL << move.to);
        occupied_board = (occupied_board & ~(1ULL << move.from)) | (1ULL << move.to);
        //update side and half-moves
        side_to_move = static_cast<Color>(side_to_move ^ 1);
        en_passant_square = NONE_SQUARE;
        half_moves = 0;
        return;
    }

    if(move.flag == CAPTURE_PROMOTION){
        ColoredPieceType move_piece = side_to_move == WHITE ? WHITE_PAWN : BLACK_PAWN;

        Color other_side = static_cast<Color>(side_to_move ^ 1); //side not moving
        ColoredPieceType captured_piece = get_piece_at(move.to, other_side);
        //boards of the moving side (pawn + promotion)
        piece_boards[move_piece] &= ~(1ULL << move.from); //the pawn is removed from the board (promotion)
        piece_boards[move.promotion_piece] |= (1ULL << move.to); //add promotion piece
        colored_boards[side_to_move] = (colored_boards[side_to_move] & ~(1ULL << move.from)) | (1ULL << move.to); //pawn is removed, but promotion is added to move.to
        //boards of captured side
        piece_boards[captured_piece] &= ~(1ULL << move.to);
        colored_boards[other_side] &= ~(1ULL << move.to);

        occupied_board &= ~(1ULL << move.from);

        //update counter and similar
        half_moves = 0;
        en_passant_square = NONE_SQUARE;
        side_to_move = static_cast<Color>(side_to_move ^ 1);
        state.captured_piece = captured_piece;

        //update castling right of the captured side
        if(captured_piece == WHITE_ROOK){
            if(move.to == A1){ //queen side
                castling_rights &= 0b11111011;
            }else if(move.to == H1){ //king side
                castling_rights &= 0b11110111;
            }
        }
        else if(captured_piece == BLACK_ROOK){
            if(move.to == A8){ //queen side
                castling_rights &= 0b11111110;
            }else if(move.to == H8){ //king side
                castling_rights &= 0b11111101;
            }
        }
        return;
    }

    if(move.flag == QUIET){
        ColoredPieceType piece = get_piece_at(move.from, side_to_move);

        //update boards
        piece_boards[piece] = (piece_boards[piece] & ~(1ULL << move.from)) | (1ULL << move.to);
        colored_boards[side_to_move] = (colored_boards[side_to_move] & ~(1ULL << move.from)) | (1ULL << move.to);
        occupied_board = (occupied_board & ~(1ULL << move.from)) | (1ULL << move.to);

        //update counters and other
        en_passant_square = NONE_SQUARE;
        side_to_move = static_cast<Color>(side_to_move ^ 1);

        if(piece == WHITE_PAWN || piece == BLACK_PAWN ){
            half_moves = 0;
            return;
        }else{
            ++half_moves;
        }

        //update castling rights
        if(piece == WHITE_KING){
            castling_rights &= 0b11110011;
            return;
        }
        if(piece == BLACK_KING){
            castling_rights &= 0b11111100;
            return;
        }
        if(piece == WHITE_ROOK){
            if(move.from == A1){ //queen side
                castling_rights &= 0b11111011;
            }else if(move.from == H1){ //king side
                castling_rights &= 0b11110111;
            }
            return;
        }
        if(piece == BLACK_ROOK){
            if(move.from == A8){ //queen side
                castling_rights &= 0b11111110;
            }else if(move.from == H8){ //king side
                castling_rights &= 0b11111101;
            }
            return;
        }
        return;
    }
}

void Board::unmake_move(const Move& move, const StateInfo& state){
    if(side_to_move == WHITE)
        --full_moves;
    //restore state
    half_moves = state.half_moves;
    en_passant_square = state.en_passant_square;
    castling_rights = state.castling_rights;

    if(move.flag == DOUBLE_PAWN_PUSH){
        //update pawn board
        ColoredPieceType pawn_type = side_to_move == WHITE ? BLACK_PAWN : WHITE_PAWN;
        piece_boards[pawn_type] = (piece_boards[pawn_type] & ~(1ULL << move.to)) | (1ULL << move.from);
        //update other boards
        Color other_side = static_cast<Color>(side_to_move ^ 1);
        colored_boards[other_side] = (colored_boards[other_side] & ~(1ULL << move.to)) | (1ULL << move.from);
        occupied_board = ((occupied_board) & ~(1ULL << move.to)) | (1ULL << move.from);
        //update side
        side_to_move = static_cast<Color>(side_to_move ^ 1);
        return;
    }

    if(move.flag == EN_PASSANT){
        //calculate where is the piece we capture
        Square captured_square = side_to_move == BLACK ? static_cast<Square>(move.to - 1) : static_cast<Square>(move.to + 1);

        Color other_side = static_cast<Color>(side_to_move ^ 1); //side that moved when was performing the move
        ColoredPieceType pawn_type = other_side == WHITE ? WHITE_PAWN : BLACK_PAWN;

        //update capturing side
        piece_boards[pawn_type] = (piece_boards[pawn_type] & ~(1ULL << move.to)) | (1ULL << move.from);
        colored_boards[other_side] = (colored_boards[other_side] & ~(1ULL << move.to)) | (1ULL << move.from);

        //update captured side
        piece_boards[state.captured_piece] |= 1ULL << captured_square;
        colored_boards[side_to_move] |= 1ULL << captured_square;

        occupied_board = (occupied_board & ~(1ULL << move.to)) | (1ULL << move.from) | (1ULL << captured_square);
        
        //update side and half-moves
        side_to_move = static_cast<Color>(side_to_move ^ 1);
        return;
    }

    if(move.flag == KING_CASTLE){
        if(side_to_move == BLACK){ //i.e. white was the one doing the castling
            //king: E1 -> G1 (flip it)
            piece_boards[WHITE_KING] = (piece_boards[WHITE_KING] & ~(1ULL << G1)) | (1ULL << E1);
            //rook: H1 -> F1 (flip it)
            piece_boards[WHITE_ROOK] = (piece_boards[WHITE_ROOK] & ~(1ULL << F1)) | (1ULL << H1);
            //update boards
            colored_boards[WHITE] = (colored_boards[WHITE] & ~(1ULL << G1) & ~(1ULL << F1)) | (1ULL << E1) | (1ULL << H1);
            occupied_board = (occupied_board & ~(1ULL << G1) & ~(1ULL << F1)) | (1ULL << E1) | (1ULL << H1);
        }
        else{
            //king: E8 -> G8 (flip it)
            piece_boards[BLACK_KING] = (piece_boards[BLACK_KING] & ~(1ULL << G8)) | (1ULL << E8);
            //rook: H8 -> F8 (flip it)
            piece_boards[BLACK_ROOK] = (piece_boards[BLACK_ROOK] & ~(1ULL << F8)) | (1ULL << H8);
            //update boards
            colored_boards[BLACK] = (colored_boards[BLACK] & ~(1ULL << G8) & ~(1ULL << F8)) | (1ULL << E8) | (1ULL << H8);
            occupied_board = (occupied_board & ~(1ULL << G8) & ~(1ULL << F8)) | (1ULL << E8) | (1ULL << H8);
        }

        //update side
        side_to_move = static_cast<Color>(side_to_move ^ 1);
        return;
    }

    if(move.flag == QUEEN_CASTLE){
        if(side_to_move == BLACK){ //i.e. white was the one doing the castling
           //king: E1 -> C1 (flip it)
            piece_boards[WHITE_KING] = (piece_boards[WHITE_KING] & ~(1ULL << C1)) | (1ULL << E1);
            //rook: A1 -> D1 (flip it)
            piece_boards[WHITE_ROOK] = (piece_boards[WHITE_ROOK] & ~(1ULL << D1)) | (1ULL << A1);
            //update boards
            colored_boards[WHITE] = (colored_boards[WHITE] & ~(1ULL << C1) & ~(1ULL << D1)) | (1ULL << E1) | (1ULL << A1);
            occupied_board = (occupied_board & ~(1ULL << C1) & ~(1ULL << D1)) | (1ULL << E1) | (1ULL << A1);
        }
        else{
            //king: E8 -> C8 (flip it)
            piece_boards[BLACK_KING] = (piece_boards[BLACK_KING] & ~(1ULL << C8)) | (1ULL << E8);
            //rook: A8 -> D8 (flip it)
            piece_boards[BLACK_ROOK] = (piece_boards[BLACK_ROOK] & ~(1ULL << D8)) | (1ULL << A8);
            //update boards
            colored_boards[BLACK] = (colored_boards[BLACK] & ~(1ULL << C8) & ~(1ULL << D8)) | (1ULL << E8) | (1ULL << A8);
            occupied_board = (occupied_board & ~(1ULL << C8) & ~(1ULL << D8)) | (1ULL << E8) | (1ULL << A8);
        }
    
        //update side
        side_to_move = static_cast<Color>(side_to_move ^ 1);
        return;
    }

    if(move.flag == CAPTURE){
        Color capturing_side = static_cast<Color>(side_to_move ^ 1); //side that was doing the move
        ColoredPieceType capturing_piece = get_piece_at(move.to, capturing_side);
        //update capturing side's board
        piece_boards[capturing_piece] = (piece_boards[capturing_piece] & ~(1ULL << move.to)) | (1ULL << move.from);
        colored_boards[capturing_side] = (colored_boards[capturing_side] & ~(1ULL << move.to)) | (1ULL << move.from);
        //captured side's boards
        piece_boards[state.captured_piece] |= 1ULL << move.to;
        colored_boards[side_to_move] |= 1ULL << move.to;

        occupied_board |= 1ULL << move.from;

        //update side
        side_to_move = static_cast<Color>(side_to_move ^ 1);
        return;
    }

    if(move.flag == PROMOTION){
        //side that did the move
        Color other_side = static_cast<Color>(side_to_move ^ 1);
        //update pawn board
        ColoredPieceType pawn_type = other_side == WHITE ? WHITE_PAWN : BLACK_PAWN;
        piece_boards[pawn_type] |= 1ULL << move.from;
        //update promotion piece board
        piece_boards[move.promotion_piece] &= ~(1ULL << move.to);
        //update color board
        colored_boards[other_side] = (colored_boards[other_side] & ~(1ULL << move.to)) | (1ULL << move.from);
        occupied_board = (occupied_board & ~(1ULL << move.to)) | (1ULL << move.from);
        //update side
        side_to_move = static_cast<Color>(side_to_move ^ 1);
        return;
    }

    if(move.flag == CAPTURE_PROMOTION){
        Color capturing_side = static_cast<Color>(side_to_move ^ 1);
        ColoredPieceType capturing_piece = capturing_side == WHITE ? WHITE_PAWN : BLACK_PAWN;

        //boards of the capturing side (pawn + promotion)
        piece_boards[capturing_piece] |= 1ULL << move.from; //the pawn is added back to the board (promotion)
        piece_boards[move.promotion_piece] &= ~(1ULL << move.to); //remove promotion piece
        colored_boards[capturing_side] = (colored_boards[capturing_side] & ~(1ULL << move.to)) | (1ULL << move.from);
        //boards of captured side
        piece_boards[state.captured_piece] |= 1ULL << move.to;
        colored_boards[side_to_move] |= 1ULL << move.to;

        occupied_board |= 1ULL << move.from;

        //update side
        side_to_move = static_cast<Color>(side_to_move ^ 1);
        return;
    }

    if(move.flag == QUIET){
        Color other_side = static_cast<Color>(side_to_move ^ 1); //side that did the move
        ColoredPieceType piece = get_piece_at(move.to, other_side);

        //update boards
        piece_boards[piece] = (piece_boards[piece] & ~(1ULL << move.to)) | (1ULL << move.from);
        colored_boards[other_side] = (colored_boards[other_side] & ~(1ULL << move.to)) | (1ULL << move.from);
        occupied_board = (occupied_board & ~(1ULL << move.to)) | (1ULL << move.from);

        //update side
        side_to_move = static_cast<Color>(side_to_move ^ 1);
        return;
    }
}

int Board::search_max(int depth, int alpha, int beta){
    MoveGenerator generator = MoveGenerator(*this);

    //check if previous move (min side) left their king in check - return a value saying this is illegal
    Square min_side_king = lsb(piece_boards[side_to_move==WHITE ? BLACK_KING : WHITE_KING]);
    if(generator.is_square_attacked(min_side_king, side_to_move))
        return ILLEGAL_MOVE_SCORE;

    //leaf
    if(depth == 0){
        return score_board();
    }

    Move move;
    int best_score = -ILLEGAL_MOVE_SCORE;

    while(generator.step(move)){ //while MoveGenerator still has moves

        //try this move
        make_move(move, states[depth]);
        int score = search_min(depth - 1, alpha, beta);
        unmake_move(move, states[depth]);

        //if illegal move (left our king in check)
        if(score == -ILLEGAL_MOVE_SCORE)
            continue; //skip this illegal move

        //alpha-beta pruning. parent node (minimizer) already found a move that gives a lower score
        if(score >= beta){ 
            return score;
        }

        //best move so far
        if(best_score < score){
            best_score = score;
            //if we found a move beating our grandparent's best (our parent's alpha)
            if(score > alpha)
                alpha = score;
        }
    }

    //we could not find any legal move
    if(best_score == -ILLEGAL_MOVE_SCORE){
        Square my_king = lsb(piece_boards[side_to_move == WHITE ? WHITE_KING : BLACK_KING]);
        if (generator.is_square_attacked(my_king, static_cast<Color>(side_to_move ^ 1)))
            return -MATE_SCORE - 50 * depth; //checkmate for the other side
        return 0; //stalemate
    }

    return best_score;
}

int Board::search_min(int depth, int alpha, int beta){
    MoveGenerator generator = MoveGenerator(*this);

    //check if previous move (max side) left their king in check - return a value saying this is illegal
    Square max_side_king = lsb(piece_boards[side_to_move==WHITE ? BLACK_KING : WHITE_KING]);
    if(generator.is_square_attacked(max_side_king, side_to_move))
        return -ILLEGAL_MOVE_SCORE;

    //leaf
    if(depth == 0){
        return score_board();
    }

    Move move;
    int best_score = ILLEGAL_MOVE_SCORE;

    while(generator.step(move)){ //while MoveGenerator still has moves

        //try this move
        make_move(move, states[depth]);
        int score = search_max(depth - 1, alpha, beta);
        unmake_move(move, states[depth]);

        //if illegal move (left our king in check)
        if(score == ILLEGAL_MOVE_SCORE)
            continue; //skip this illegal move

        //alpha-beta pruning. parent node (maximizer) already found a move that gives a lower score
        if(score <= alpha){ 
            return score;
        }

        //best move so far
        if(best_score > score){
            best_score = score;
            //if we found a move beating our grandparent's best (our parent's beta)
            if(score < beta)
                beta = score;
        }
    }

    //we could not find any legal move
    if(best_score == ILLEGAL_MOVE_SCORE){
        Square my_king = lsb(piece_boards[side_to_move == WHITE ? WHITE_KING : BLACK_KING]);
        if (generator.is_square_attacked(my_king, static_cast<Color>(side_to_move ^ 1)))
            return MATE_SCORE + 50 * depth; //checkmate for the other side
        return 0; //stalemate
    }

    return best_score;
}

Move Board::search_best_move(){
    MoveGenerator generator(*this);
    Move move;
    Move best_move;
    int alpha = -ILLEGAL_MOVE_SCORE;
    int beta = ILLEGAL_MOVE_SCORE;

    //if engine is white - it should maximize the score
    if(side_to_move == WHITE){
        int best_score = -ILLEGAL_MOVE_SCORE;

        while(generator.step(move)){
            //try this move
            make_move(move, states[DEPTH]);
            int score = search_min(DEPTH - 1, alpha, beta);
            unmake_move(move, states[DEPTH]);

            //if illegal move (left our king in check)
            if(score == -ILLEGAL_MOVE_SCORE)
                continue; //skip this illegal move

            //found new best move
            if(score > best_score){ 
                best_score = score;
                best_move = move;
                alpha = score; //alpha is just always best-score in the root (uses 2 different vars for clarity)
            }
        }
    } else { //if engine is black - it should minimize the score
        int best_score = ILLEGAL_MOVE_SCORE;

        while(generator.step(move)){
            //try this move
            make_move(move, states[DEPTH]);
            int score = search_max(DEPTH - 1, alpha, beta);
            unmake_move(move, states[DEPTH]);

            //if illegal move (left our king in check)
            if(score == ILLEGAL_MOVE_SCORE)
                continue; //skip this illegal move

            //found new best move
            if(score < best_score){ 
                best_score = score;
                best_move = move;
                beta = score; //alpha is just always best-score in the root (uses 2 different vars for clarity)
            }
        }
    }

    return best_move;
}

//check is side_to_move lost/stalemate (if other_side won in their move)
GamePhase Board::get_game_phase(){
    //if side_to_move's king is not currently attacked, game still running
    ColoredPieceType king = side_to_move == WHITE ? WHITE_KING : BLACK_KING;
    MoveGenerator generator(*this);
    Move move;

    //try to find a single move that side_to_move's king won't be attacked after
    //if can find one - game still goingc
    //if cannot - mate for other side or stalemate
    while(generator.step(move)){
        make_move(move, states[0]);
        Square king_sq = lsb(piece_boards[king]);

        //check if king is attacked
        //note that side_to_move flipped after make_move()
        if(!generator.is_square_attacked(king_sq, side_to_move)){
            unmake_move(move, states[0]);
            return GOING;
        }
        
        unmake_move(move, states[0]);
    }

    //we could not find any move that will leave our king safe
    //if it currently attacked - mate for the other side
    //if currently not attacked - stalemate
    Square king_sq = lsb(piece_boards[king]);
    if(generator.is_square_attacked(king_sq, static_cast<Color>(side_to_move ^ 1)))
        return side_to_move == WHITE ? BLACK_MATE : WHITE_MATE;
    return STALEMATE;
}

Move Board::get_user_move(){
    std::cout << "Enter your move - <from> <to> (e.g. e4 e3)" << std::endl;

    std::string from_str, to_str;
    std::cin >> from_str >> to_str;

    //parse e2 -> ('e' - 'a') * 8 + ('2' - '1') = 4 * 8 + 1 = 33
    Square from = static_cast<Square>((from_str[0] - 'a') * 8 + (from_str[1] - '1'));
    Square to = static_cast<Square>((to_str[0] - 'a') * 8 + (to_str[1] - '1'));

    //determine flag - instead of logic, generate all moves, and take the matching one
    MoveGenerator generator(*this);
    Move move;
    while(generator.step(move)){
        // print_enging_move(move);
        if(move.from == from && move.to == to){
            if((move.flag != CAPTURE_PROMOTION) && (move.flag != PROMOTION)) //not a promotion
                return move;
            
            //a promotion, ask user for what type to prompte
            std::cout << "Enter to what piece to promote your pawn (q/r/b/n)" << std::endl;
            std::string promotion_str;
            ColoredPieceType promotion_piece;
            std::cin >> promotion_str;
            if(promotion_str == "q" || promotion_str == "Q"){
                promotion_piece = side_to_move == WHITE ? WHITE_QUEEN : BLACK_QUEEN;
            } else if (promotion_str == "r" || promotion_str == "R"){
                promotion_piece = side_to_move == WHITE ? WHITE_ROOK : BLACK_ROOK;
            } else if (promotion_str == "b" || promotion_str == "B"){
                promotion_piece = side_to_move == WHITE ? WHITE_BISHOP : BLACK_BISHOP;
            } else if (promotion_str == "n" || promotion_str == "N"){
                promotion_piece = side_to_move == WHITE ? WHITE_KNIGHT : BLACK_KNIGHT;
            } else {
                std::cout << "Entered an invalid value. Promotes to a queen." << std::endl;
                promotion_piece = side_to_move == WHITE ? WHITE_QUEEN : BLACK_QUEEN;
            }

            move.promotion_piece = promotion_piece;
            return move;
        }
    }


    //invalid call - recursive call
    std::cout << "You entered an invalid move." << std::endl;
    return get_user_move();
}

void Board::ask_user_color(){
    while(true){
        std::cout << "Enter the color you want to play (w/b)" << std::endl;
        std::string user_color;
        std::cin >> user_color;

        if(user_color == "W" || user_color == "w"){
            engine_color = BLACK;
            return;
        }

        if(user_color == "B" || user_color == "b"){
            engine_color = WHITE;
            return;
        }

        std::cout << "You enterd an invalid color." << std::endl;
    }
}

void Board::print_enging_move(const Move& move){
    //Square 33 = 'e2'. (33 / 8) + 'a' = 4 + 'a' = 'e' | 33 % 8 + '1' = 1 + '1' = '2'
    char from_file = move.from/8 + 'a';
    char from_rank = move.from%8 + '1';
    char to_file = move.to/8 + 'a';
    char to_rank = move.to%8 + '1';
    std::cout << "Engine moved " << from_file << from_rank << " to " << to_file << to_rank << std::endl;
}

void Board::play_game(){
    setup_start_position();
    
    ask_user_color();

    while(get_game_phase() == GOING){
        print_board();

        if(side_to_move == engine_color){
            Move move = search_best_move();
            print_enging_move(move);
            make_move(move, states[DEPTH]);
        } else {
            Move move = get_user_move();
            make_move(move, states[DEPTH]);
        }
    }

    print_board();
    switch (get_game_phase())
    {
    case WHITE_MATE:
        std::cout << "\n\nWHITE WON!" << std::endl;
        break;
    case BLACK_MATE:
        std::cout << "\n\nBLACK WON!" << std::endl;
        break;
    case STALEMATE:
        std::cout << "\n\nSTALEMATE!" << std::endl;
        break;
    default:
        std::cout << "\n\nGAME IS STILL RUNNING FOR SOME REASON?!?!" << std::endl;
    }
}
