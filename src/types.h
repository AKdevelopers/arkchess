#ifndef TYPES_INCLUDED
#define TYPES_INCLUDED
#include <stdint.h>
typedef uint64_t U64;
typedef uint16_t U16;

struct Board {
    // White pieces
    U64 white_pawns;
    U64 white_rooks;
    U64 white_bishops;
    U64 white_knights;
    U64 white_queen;
    U64 white_king;

    // Black pieces
    U64 black_pawns;
    U64 black_rooks;
    U64 black_bishops;
    U64 black_knights;
    U64 black_queen;
    U64 black_king;

    // Colors
    U64 all_white_pieces;
    U64 all_black_pieces;

    // All pieces
    U64 all_pieces;

    int colour_to_move;
	int castling_rights; // 15 for KQkq, 0 for ---- 
	int ep_square; // 0..63
	int half_move_ctr;
	int full_move_ctr;
};

extern U64 ClearFile[8];
extern U64 MaskFile[8];
extern U64 ClearRank[8];
extern U64 MaskRank[8];
extern U64 DiagonalMask[15];
extern U64 AntiDiagonalMask[15];

extern const int KINGSIDE_WHITE;
extern const int QUEENSIDE_WHITE;
extern const int KINGSIDE_BLACK;
extern const int QUEENSIDE_BLACK;
#endif 

