#include <stdio.h>
#include <stdlib.h>
#include "ray_attacks.h"
#include "types.h"
#include "ark.h"

U64 generate_ray_attacks(U64 rank_mask, U64 file_mask, struct Board *pos, U64 piece) 
{	
    U64 own_pieces = (pos->colour_to_move) ? pos->all_black_pieces : pos->all_white_pieces;
	// find all pieces occupying the rank and file the piece is currently on
	U64 occupied = (pos->all_pieces & rank_mask) | (pos->all_pieces & file_mask); 

	ray_attacks = positive_ray_attack(piece, occupied, rank_mask) 
				| positive_ray_attack(piece, occupied, file_mask) 
				| negative_ray_attack(piece, occupied, file_mask) 
				| negative_ray_attack(piece, occupied, rank_mask);

	return ray_attacks & (~own_pieces);
}

/* north and east attacks */
U64 positive_ray_attack(U64 piece, U64 occupied, U64 mask) {
	occupied &= mask;
	return (occupied ^ (occupied - 2 * piece)) & mask;
}

/* south and west attacks */
U64 negative_ray_attack(U64 piece, U64 occupied, U64 mask) {
	U64 r_occupied = ReverseBits(occupied & mask);
	U64 r_piece = ReverseBits(piece);
	return (occupied ^ ReverseBits((r_occupied - 2 * r_piece))) & mask;
}

U64 ReverseBits (U64 num)
{
	num = (((num >> 1) & 0x5555555555555555) | ((num & 0x5555555555555555) << 1));
	num = (((num >> 2) & 0x3333333333333333) | ((num & 0x3333333333333333) << 2));
	num = (((num >> 4) & 0xf0f0f0f0f0f0f0f) | ((num & 0xf0f0f0f0f0f0f0f) << 4));
	num = (((num >> 8) & 0xff00ff00ff00ff) | ((num & 0xff00ff00ff00ff) << 8));
	num = (((num >> 16) & 0xffff0000ffff) | ((num & 0xffff0000ffff) << 16));
	num = (((num >> 32) & 0xffffffff) | ((num & 0xffffffff) << 32));

	return num;
} 	




