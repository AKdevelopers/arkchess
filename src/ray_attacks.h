#include <stdio.h>
#include "types.h"
#include "ark.h"

U64 ReverseBits (U64 num);
U64 EastAttack(U64 piece, U64 occupied, U64 mask);
U64 NorthAttack(U64 piece, U64 occupied, U64 mask); 
U64 SouthAttack(U64 piece, U64 occupied, U64 mask); 
U64 WestAttack(U64 piece, U64 occupied, U64 mask); 
U64 ray_attacks;
U64 generate_ray_attacks(U64 rank_mask, U64 file_mask, struct Board *pos, U64 piece);
