#ifndef ARK_INCLUDED
#define ARK_INCLUDED
#include "types.h"
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
};

extern U64 ClearFile[8];
extern U64 MaskFile[8];
extern U64 ClearRank[8];
extern U64 MaskRank[8];
extern U64 DiagonalMask[15];
extern U64 AntiDiagonalMask[15];

int GetIndex(int rank, int file); 
struct Board InitPosition(char* FEN_str);
U64 KingMoves(U64 king_loc, U64 all_pieces);
U64 KnightMoves (U64 knight_loc, U64 all_pieces);
U64 WhitePawnMoves(U64 pawn_loc, U64 all_pieces, U64 black_pieces, U64 *clear_file, U64 *mask_rank);
U64 BlackPawnMoves(U64 pawn_loc, U64 all_pieces, U64 white_pieces, U64 *clear_file, U64 *mask_rank);
U64 DiagonalAttacks(U64 piece_BB, U64 own_pieces, U64 all_pieces, U64 *mask_antidiagonal, U64 *mask_diagonal);
U64 QueenMoves(U64 queen_loc, U64 own_pieces, U64 all_pieces, U64 *mask_file, U64 *mask_rank, U64 *mask_antidiagonal, U64 *mask_diagonal) ;
int count_set_bits(U64 bb);
void split_bits_index(U64 bb, int num_set, int *index_arr);
U16 process_move(int dest, int src, int promo_piece, int special);
U16 *get_knight_moves(struct Board *pos);
U16 *get_rook_moves(struct Board *pos);
void fill_move_list(U16 *move_list, U64 move_bb, int src_index);
#endif 


