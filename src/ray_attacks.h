#include <stdio.h>
#include "types.h"

U64 ReverseBits (U64 num);
U64 EastAttack(U64 piece, U64 occupied, U64 mask);
U64 NorthAttack(U64 piece, U64 occupied, U64 mask); 
U64 SouthAttack(U64 piece, U64 occupied, U64 mask); 
U64 WestAttack(U64 piece, U64 occupied, U64 mask); 
U64 ray_attacks;
U64 GenerateRayAttacks(U64 rank_mask, U64 file_mask, U64 all_pieces, U64 piece);
