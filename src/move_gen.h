#ifndef ARK_INCLUDED
#define ARK_INCLUDED
#include "types.h"
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
U16 *get_bishop_moves(struct Board *pos);
U16 *get_queen_moves(struct Board *pos);
void fill_move_list(U16 *move_list, U64 move_bb, int src_index);
#endif 


