#include <stdint.h>
#include <stdint.h>
#include "bits.h"
#include "types.h"

int get_index(int rank, int file) {
	// A,B,..,H = 0,1,..7 for files e.g. A1 is {0,0}
	return 8 * rank + file;
}

int get_set_bit(U64 bitboard) {
	// function that returns index of least significant set bit
	// the builtin function returns set bit + 1, so subtracting 1 reverses that
	return __builtin_ffsll(bitboard) - 1;
}

int get_rank(int index) {
	return index / 8;
}

int get_file(int index) {
	return index % 8;
}

void split_bits_index(U64 bb, int num_set, int *index_arr) {
	int index = 0;
	int bit = 0;
	
	while (bit < 64 && index < num_set) {
		if (bb & 1) {
			index_arr[index] = bit;
			index++;
		}
		bb >>= 1;
		bit++;
	}
}

int count_set_bits(U64 bb) {
	int num_set = 0;
	for (int i = 0; i < 64; i++) {
		if (bb & 1) {
			num_set++;
		}
		bb >>= 1;
	}
	return num_set;
}
