#ifndef BITBOARD
#define BITBOARD

#include "types.h"

using Bitboard = std::uint64_t;

constexpr Bitboard FILE_A_MASK = 0x00000000000000FFULL;
constexpr Bitboard FILE_H_MASK = 0xFF00000000000000ULL;
constexpr Bitboard RANK_1_MASK = 0x0101010101010101ULL;
constexpr Bitboard RANK_2_MASK = RANK_1_MASK << 1;
constexpr Bitboard RANK_3_MASK = RANK_1_MASK << 2;
constexpr Bitboard RANK_6_MASK = RANK_1_MASK << 5;
constexpr Bitboard RANK_7_MASK = RANK_1_MASK << 6;
constexpr Bitboard RANK_8_MASK = RANK_1_MASK << 7;
constexpr Bitboard NOT_RANK_1_MASK = ~RANK_1_MASK;
constexpr Bitboard NOT_RANK_8_MASK = ~RANK_8_MASK;
constexpr Bitboard NOT_RANK_12_MASK = ~(RANK_1_MASK | RANK_2_MASK);
constexpr Bitboard NOT_RANK_78_MASK = ~(RANK_7_MASK | RANK_8_MASK);

inline Square get_lsb(Bitboard& bb){
    Square sq = static_cast<Square>(__builtin_ctzll(bb)); //index of LSB
    bb &= bb - 1; //remove LSB
    return sq;
}

inline Square lsb(Bitboard bb){
    return static_cast<Square>(__builtin_ctzll(bb)); //index of LSB
}

inline Square msb(Bitboard bb){
    return static_cast<Square>(__builtin_clzll(bb)); //index of MSB
}

inline int popcount(Bitboard bb){
    return __builtin_popcountll(bb);
}

#endif