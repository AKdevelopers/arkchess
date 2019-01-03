#include <stdio.h>
#include <stdlib.h>
#include "ray_attacks.h"
#include "types.h"
#include "ark.h"

U64 GenerateRayAttacks(U64 rank, U64 file, U64 all_pieces, U64 piece) 
{	
	U64 occupied = (all_pieces & MaskRank[rank]) | (all_pieces & MaskFile[file]); // pieces occupying the boards

	ray_attacks = EastAttack(piece, occupied, MaskRank[rank]) 
				| NorthAttack(piece, occupied, MaskFile[file]) 
				| SouthAttack(piece, occupied, MaskFile[file]) 
				| WestAttack(piece, occupied, MaskRank[rank]);

	return ray_attacks;
}


U64 EastAttack(U64 piece, U64 occupied, U64 mask) {
	U64 east_attack;
	east_attack = (occupied ^ (occupied - 2 * piece)) & mask;
	return east_attack;
}

U64 NorthAttack(U64 piece, U64 occupied, U64 mask) {
	U64 north_attack;
	U64 blockers;
	blockers = occupied & mask;
	north_attack = (occupied ^ (blockers - 2 * piece)) & mask;
	return north_attack;
}

/* A file mask required for vertical attack */
U64 SouthAttack(U64 piece, U64 occupied, U64 mask) {
	U64 south_attack;
	U64 blockers;
	blockers = ReverseBits(occupied & mask);
	south_attack = (occupied ^ ReverseBits((blockers - 2 * ReverseBits(piece)))) & mask;
	return south_attack;
}

/* rank mask required for horizontal attack */
U64 WestAttack(U64 piece, U64 occupied, U64 mask) {
	U64 west_attack;
	west_attack = (occupied ^ ReverseBits((ReverseBits(occupied) - 2 * ReverseBits(piece)))) & mask;
	return west_attack;
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




