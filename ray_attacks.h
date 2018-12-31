#include <stdio.h>

typedef unsigned long long U64;

U64 ReverseBits (U64 num);
U64 EastAttack(U64 piece, U64 occupied, U64 mask);
U64 NorthAttack(U64 piece, U64 occupied, U64 mask); 
U64 SouthAttack(U64 piece, U64 occupied, U64 mask); 
U64 WestAttack(U64 piece, U64 occupied, U64 mask); 
U64 ray_attacks;
