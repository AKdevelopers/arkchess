#ifndef BITS_INCLUDED
#define BITS_INCLUDED
#include "types.h"
int get_file(int index);
int get_rank(int index);
int get_index(int rank, int file);
int count_set_bits(U64 bb);
void split_bits_index(U64 bb, int num_set, int *index_arr);
int get_set_bit(U64 bitboard);
#endif
