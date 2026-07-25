#include <algorithm>
#include <iostream>
#include <sstream>
#include <bitset>
#include "board.h"

Board::Board(){
    //init all bitboards to 0
    std::fill(std::begin(piece_boards), std::end(piece_boards), 0);

    side_to_move = WHITE;

    //init all 4 castling options to 1
    castling_rights = 0b00001111;

    en_passant_square = NONE_SQUARE; //init to an invalid square
    half_moves = 0;
    full_moves = 1; //chess games start at 1 and not 0
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
}

ColoredPieceType Board::get_peice_at(Square sq){
    //go over all peice types
    //check for each if it occupies sq
    for(int type = 0; type < 12; type++){
        //peice_boards[type] is our current board & peice we are checking
        //board >> sq, moves the relevan square to the LSB
        //by doing & 1ULL we preserve only the LSB
        int is_occupied = (piece_boards[type] >> sq) & 1ULL;
        if(is_occupied) //if the square was occupied by this type, return it
            return static_cast<ColoredPieceType>(type);
    }

    return NONE_PEICE;
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
    std::cout << "en passant" << en_passant << std::endl;
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
        std::cout << "Current square is " << current_square << std::endl;
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
}

void Board::print_board(){
    //in chess: files are lettered rows (e.g., A, B, C, etc..)
    //ranks are numerical rows (e.g., 1, 2, 3, etc..)
    for(int rank = 0; rank < 8; rank++){
        for(int file = 0; file < 8; file++){
            Square sq = calc_square(file, rank);
            ColoredPieceType peice = get_peice_at(sq);

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

    std::cout << "Side to move: " << side_to_move << " | Castling rights: " << std::bitset<4>(castling_rights) <<
    " | en passant: " << en_passant_square << " | halfmoves: " << half_moves << " | fullmoves " << full_moves << std::endl;
}
