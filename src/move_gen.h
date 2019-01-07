#ifndef ARK_INCLUDED
#define ARK_INCLUDED
#include "types.h"
U64 KingMoves(U64 king_loc, U64 all_pieces);
U64 KnightMoves (U64 knight_loc, U64 all_pieces);
U64 WhitePawnMoves(U64 pawn_loc, U64 all_pieces, U64 black_pieces, U64 *clear_file, U64 *mask_rank);
U64 BlackPawnMoves(U64 pawn_loc, U64 all_pieces, U64 white_pieces, U64 *clear_file, U64 *mask_rank);
U16 process_move(int dest, int src, int promo_piece, int special);
U16 *get_knight_moves(struct Board *pos);
U16 *get_rook_moves(struct Board *pos);
U16 *get_bishop_moves(struct Board *pos);
U16 *get_queen_moves(struct Board *pos);
void fill_move_list(U16 *move_list, U64 move_bb, int src_index);
#endif 


