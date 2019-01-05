#include <stdio.h>
#include "ray_attacks.h"
#include "move_gen.h"
#include "types.h"
#include "FEN_Parser.h" 
#include "vector.h"

U16 DEST_MASK = 0x3f;
U16 SRC_MASK = 0xfc0;
U16 PROMO_PIECE_MASK = 0x3000;
U16 SPECIAL_MOVE_MASK = 0xc000;

int get_src(U16 move) {
    return (SRC_MASK & move) >> 6;
}

int get_dest(U16 move) {
    return DEST_MASK & move;
}

int get_promo_piece(U16 move) {
    return (PROMO_PIECE_MASK & move) >> 12;
}

int get_special_move(U16 move) {
    return (SPECIAL_MOVE_MASK & move) >> 14;
}


// initializes the position by parsing the given FEN string and returning bitboards
struct Board InitPosition(char* FEN_str) {
	struct Board state;
	state = ParseFEN(FEN_str);	

	return state;
}

int GetIndex(int rank, int file) {
	// A,B,..,H = 0,1,..7 for files e.g. A1 is {0,0}
	int squareIndex = 8 * rank + file;

	return squareIndex;
}

int GetSetBit(U64 bitboard) {
	// function that returns index of least significant set bit
	// the builtin function returns set bit + 1, so subtracting 1 reverses that
	int index = __builtin_ffsll(bitboard) - 1;

	return index;
}

int GetRank(int index) {
	return index / 8;
}

int GetFile(int index) {
	return index % 8;
}

/* Takes current king location bitboard, own pieces bitboard and returns moves */
U64 KingMoves(U64 king_loc, U64 own_pieces) {
	U64 a_file_clip, h_file_clip, spot_1, spot_2, spot_3, spot_4, spot_5, spot_6, spot_7, spot_8; // 8 possible moves for the King
	U64 valid_moves;
	// if king is on a or h file, it will clear the bitboard (give 0)
	a_file_clip = king_loc & ClearFile[0];
	h_file_clip = king_loc & ClearFile[7];

	spot_1 = a_file_clip << 7; // if King was on the a-file, it is now empty and no move will be generated here (see King move mapping)
	spot_8 = a_file_clip >> 1; 
	spot_7 = a_file_clip >> 9;

	spot_3 = h_file_clip << 9; // does the same thing as above, but on the right side
	spot_4 = h_file_clip << 1;
	spot_5 = h_file_clip >> 7;

	spot_2 = king_loc << 8; // generates north move
	spot_6 = king_loc >> 8; // generates south move

	valid_moves = spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 |
					spot_7 | spot_8;

	// AND with ~own_pieces will zero out any squares where own pieces are
	valid_moves &= ~own_pieces;

	return valid_moves;
}

U16 *get_knight_moves(struct Board *pos) {
    U64 knight_loc = (pos->colour_to_move) ? pos->black_knights : pos->white_knights;
    U64 own_pieces = (pos->colour_to_move) ? pos->all_black_pieces : pos->all_white_pieces;

	int num_set = count_set_bits(knight_loc);
	int knights_index[num_set];
	split_bits_index(knight_loc, num_set, knights_index);

    U16 *move_list = NULL;
    int num_moves;

	for (int i = 0; i < num_set; i++) {
        int curr_src = knights_index[i];
		U64 curr_bb = 1 << curr_src;
		U64 move_bb = KnightMoves(curr_bb, own_pieces);
        num_moves = count_set_bits(move_bb);
        int dest[num_moves];
        split_bits_index(move_bb, num_moves, dest);
        for (int i = 0; i < num_moves; i++) {
            vector_push_back(move_list, process_move(dest[i], curr_src, 0, 0));
        }
	}

    return move_list;
}

U16 process_move(int dest, int src, int promo_piece, int special) {
    U16 move = 0;
    move |= dest | (src << 6) | (promo_piece << 12) | (special << 14);

    return move;
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

U64 KnightMoves(U64 knight_loc, U64 own_pieces) {
	U64 a_file_clip, h_file_clip, ab_file_clip, gh_file_clip;
	U64 spot_1, spot_2, spot_3, spot_4, spot_5, spot_6, spot_7, spot_8;
	U64 valid_moves;

	/* if N is on a or h file, the first two will yield empty bitboards
	if N is on a,b file it will yield the 3rd empty BB, if on g,h, it will yield empty BB */

	a_file_clip = knight_loc & ClearFile[0];
	h_file_clip = knight_loc & ClearFile[7];
	ab_file_clip = knight_loc & ClearFile[0] & ClearFile[1];
	gh_file_clip = knight_loc & ClearFile[6] & ClearFile[7];

	// Left exterior (refer to documentation)
	spot_1 = ab_file_clip << 6;
	spot_8 = ab_file_clip >> 10;

	// Left interior
	spot_2 = a_file_clip << 15;
	spot_7 = a_file_clip >> 17;

	// Right exterior 
	spot_4 = gh_file_clip << 10;
	spot_5 = gh_file_clip >> 6;

	// Left interior
	spot_3 = h_file_clip << 17;
	spot_6 = h_file_clip >> 15;

	valid_moves = spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 |
					spot_7 | spot_8;

	valid_moves &= ~own_pieces;
			
	return valid_moves;
}

U64 WhitePawnMoves(U64 pawn_loc, U64 all_pieces, U64 black_pieces, U64 *clear_file, U64 *mask_rank) {
	U64 valid_moves, one_step, two_step, left_capture, right_capture, a_file_clip, h_file_clip;
	two_step = 0;

	/* ~all_pieces will yield a BB of 0s where a piece exists, which when ANDed with pawn loc
	left shifted 8 (to the next rank) will determine if square in front 
	of pawn is empty or not. E.g. ~all_pieces is 1 (meaning square is free) and pawn_loc << 8 is 
	also 1, then it will generate a move for that square
	*/
	a_file_clip = pawn_loc & clear_file[0];
	h_file_clip = pawn_loc & clear_file[7];
	one_step = (pawn_loc << 8) & ~all_pieces;

	two_step = pawn_loc & mask_rank[1]; // BB will be 1 only if pawn is on second rank
	two_step = (two_step << 8) & ~all_pieces; // checks 1 square in front
	two_step = (two_step << 8) & ~all_pieces; // checks 2 squares in front, note:
	// BB will already be 0'ed if a piece is 1 square in front

	// only captures generate diagonal moves, thus clips need to be used
	left_capture = (a_file_clip << 7) & black_pieces;
	right_capture = (h_file_clip << 9) & black_pieces;
//	printf ("%llu \t %llu\n", right_capture, two_step);

	valid_moves = one_step | two_step | left_capture | right_capture;

	return valid_moves;
}

U64 BlackPawnMoves(U64 pawn_loc, U64 all_pieces, U64 white_pieces, U64 *clear_file, U64 *mask_rank) {
	U64 valid_moves, one_step, two_step, left_capture, right_capture, a_file_clip, h_file_clip;

	// a bug arose where two_step had no value assigned to it, generated a RN, therefore a move
	two_step = 0;

	/* not all pieces will yield a BB of empty squares, which when ANDed with pawn loc
	left shifted 8 (to the next rank) will determine is square in front 
	of pawn is empty or not
	*/
	a_file_clip = pawn_loc & clear_file[0]; // clears any pawns on A file
	h_file_clip = pawn_loc & clear_file[7];	// clears any pawns on H file
	one_step = (pawn_loc >> 8) & ~all_pieces; // moves pawns up one rank 
	// also makes sure that the square is empty
	// so any active bit on one_step 
	/* will generate an empty bitboard if the pawn is not on the second rank */ 

	
	two_step = pawn_loc & mask_rank[6]; // will generate empty BB if pawn is not on 7th rank
	two_step = (two_step >> 8) & ~all_pieces; // checks 1 square in front
	two_step = (two_step >> 8) & ~all_pieces; // checks 2 squares in front
	

	/*
	if (pawn_loc & mask_rank[6])
		printf ("%llu\n",pawn_loc & mask_rank[6]);
		two_step = (one_step >> 8) & ~all_pieces;
	*/
	left_capture = (a_file_clip >> 7) & white_pieces;
	right_capture = (h_file_clip >> 9) & white_pieces;
//	printf ("%llu \t %llu\n", right_capture, two_step);

	valid_moves = one_step | two_step | left_capture | right_capture;

	return valid_moves;
}

void FilterPieces(U64 piece_BB, int index_array[]) {
	int counter = 0;

	while (piece_BB != 0) {
		index_array[counter] = GetSetBit(piece_BB);
		piece_BB &= ~(1 << index_array[counter]);
		counter++;
	}
}

U16 *get_rook_moves(struct Board *pos) {
    int index, file, rank;
    U64 move_bb, piece_bb;
    U64 rook_loc = (pos->colour_to_move) ? pos->black_rooks : pos->white_rooks;

    int num_rooks = count_set_bits(rook_loc);
    int rook_index_list[num_rooks];
    split_bits_index(rook_loc, num_rooks, rook_index_list);

    U16 *move_list = NULL;
    for (int i = 0; i < num_rooks; i++) {
        index = rook_index_list[i];
        file = GetFile(index);
        rank = GetRank(index);
        piece_bb = 1 << index;
        move_bb = generate_ray_attacks(MaskRank[rank], MaskFile[file], pos, piece_bb);
        fill_move_list(move_list, move_bb, index);
    }
    return move_list;
}

void fill_move_list(U16 *move_list, U64 move_bb, int src_index) {
    int num_moves = count_set_bits(move_bb);
    int dest[num_moves];
    split_bits_index(move_bb, num_moves, dest);

    for (int i = 0; i < num_moves; i++) {
        vector_push_back(move_list, process_move(dest[i], src_index, 0, 0));
    }
}

int get_diagonal(int file, int rank) {
	int diagonal;

	diagonal = file - (rank - 7);

	return diagonal;
}

int get_anti_diagonal(int file, int rank) {
	int antidiagonal;

	antidiagonal = file + rank;

	return antidiagonal;
}

U16 *get_bishop_moves(struct Board *pos) {
    int index, file, rank, diagonal, antidiagonal;
    U64 move_bb, piece_bb;
    U64 pieces_loc = (pos->colour_to_move) ? pos->black_bishops : pos->white_bishops;

    int num_pieces = count_set_bits(pieces_loc);
    int piece_index_list[num_pieces];
    split_bits_index(pieces_loc, num_pieces, piece_index_list);

    U16 *move_list = NULL;
    for (int i = 0; i < num_pieces; i++) {
        index = piece_index_list[i];
        file = GetFile(index);
        rank = GetRank(index);
        diagonal = get_diagonal( file, rank);
        antidiagonal = get_anti_diagonal(file, rank);
        piece_bb = 1 << index;
        move_bb = generate_ray_attacks(AntiDiagonalMask[antidiagonal], DiagonalMask[diagonal], pos, piece_bb);
        fill_move_list(move_list, move_bb, index);
    }
    return move_list;
}

U16 *get_queen_moves(struct Board *pos) {
    int index, file, rank, diagonal, antidiagonal;
    U64 move_bb, piece_bb;
    U64 pieces_loc = (pos->colour_to_move) ? pos->black_queen : pos->white_queen;

    int num_pieces = count_set_bits(pieces_loc);
    int piece_index_list[num_pieces];
    split_bits_index(pieces_loc, num_pieces, piece_index_list);

    U16 *move_list = NULL;
    for (int i = 0; i < num_pieces; i++) {
        index = piece_index_list[i];
        file = GetFile(index);
        rank = GetRank(index);
        diagonal = get_diagonal( file, rank);
        antidiagonal = get_anti_diagonal(file, rank);
        piece_bb = 1 << index;
        move_bb = generate_ray_attacks(MaskRank[rank], MaskFile[file], pos, piece_bb) | 
			generate_ray_attacks(AntiDiagonalMask[antidiagonal], DiagonalMask[diagonal], pos, piece_bb);
        fill_move_list(move_list, move_bb, index);
    }
    return move_list;
}

/*
U64 DiagonalAttacks(U64 piece_BB, U64 own_pieces, U64 all_pieces, U64 *mask_antidiagonal, U64 *mask_diagonal) {
	U64 valid_moves;

	int diagonal, antidiagonal, index, file, rank;

	index = GetSetBit(piece_BB);
	file = GetFile(index);
	rank = GetRank(index);

	diagonal = get_diagonal(piece_BB, file, rank);
	antidiagonal = get_anti_diagonal(piece_BB, file, rank);
	valid_moves = generate_ray_attacks(mask_antidiagonal[antidiagonal], mask_diagonal[diagonal], all_pieces, piece_BB) &
					(~own_pieces);

	return valid_moves;
}


U64 QueenMoves(U64 queen_loc, U64 own_pieces, U64 all_pieces, U64 *mask_file, U64 *mask_rank, U64 *mask_antidiagonal, U64 *mask_diagonal) {
	U64 valid_moves, straight_attacks = 0, diagonal_attacks;

	//straight_attacks = StraightAttacks(queen_loc, own_pieces, all_pieces, mask_file, mask_rank);
	diagonal_attacks = DiagonalAttacks(queen_loc, own_pieces, all_pieces, mask_antidiagonal, mask_diagonal);
	valid_moves = straight_attacks | diagonal_attacks;

	return valid_moves;
}	
*/



/* 
King move generation:

1 2 3 	corresponding bit shifts:		<<7 <<8 <<9
8 X 4									>>1	 X	 <<1
7 6 5									>>9  >>8 >>7

if king is on a-file, you need to clip for position 1, 8, 7. e,g, if king is on a-file,
spot8 = king_pos & clear_file[0]. This will result in an empty bitboard if king is on a-file,
and there will be no change if it is not
*/

/* 

Knight move generation:
	2	3

1			4	  		
     N
8			5

	7	6

Visualize the knight move like this, spot 1 and 8 are on the left exterior
Spot 4 and 5 are on the right exterior. For spots on the right exterior, the moves will be cut
off if the N is on G or H file, therefore the gh_clip will be applied.
Spot 2 and 7 are on the left interior, and only get cut off if the N is on the a file.
Spot 3 and 6 are on the right interior. 
This diagram is how I laid out the position mapping in the move generation.


*/
