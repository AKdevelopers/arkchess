#include <stdio.h>
#include "ray_attacks.h"
#include "ark.h"
#include "FEN_Parser.h"
#include <stdint.h>

typedef uint16_t U16;
// main move generator 2

U64 ClearFile[8] = 
{
	0xfefefefefefefefe, 0xfdfdfdfdfdfdfdfd, 0xfbfbfbfbfbfbfbfb,
	0xf7f7f7f7f7f7f7f7, 0xefefefefefefefef, 0xdfdfdfdfdfdfdfdf,
	0xbfbfbfbfbfbfbfbf, 0x7f7f7f7f7f7f7f7f
}; // when ANDed with another bitboard, clears the file

// left to right
U64 MaskFile[8] = 
{
	0x101010101010101, 0x202020202020202, 0x404040404040404, 
	0x808080808080808, 0x1010101010101010, 0x2020202020202020, 
	0x4040404040404040, 0x8080808080808080
};

U64 ClearRank[8] =
{
	0xffffffffffffff00, 0xffffffffffff00ff, 0xffffffffff00ffff,
	0xffffffff00ffffff, 0xffffff00ffffffff, 0xffff00ffffffffff,
	0xff00ffffffffffff, 0xffffffffffffff
};

// bottom to top
U64 MaskRank[8] =
{
	0xff, 0xff00, 0xff0000, 0xff000000, 0xff00000000,
	0xff0000000000, 0xff000000000000, 0xff00000000000000
};

	/* from top left to bottom right, like this slash: / */
U64 DiagonalMasks[15] = 
{
	0x100000000000000, 0x201000000000000, 0x402010000000000, 0x804020100000000, 
	0x1008040201000000, 0x2010080402010000, 0x4020100804020100, 0x8040201008040201,
	0x80402010080402, 0x804020100804, 0x8040201008, 0x80402010, 0x804020, 0x8040, 0x80
};

/* from bottom left to top right, like this backslash: \ */
U64 AntiDiagonalMasks[15] =
{
	0x1, 0x102, 0x10204, 0x1020408, 0x102040810, 0x10204081020, 0x1020408102040, 
	0x102040810204080, 0x204081020408000, 0x408102040800000, 0x810204080000000, 
	0x1020408000000000, 0x2040800000000000, 0x4080000000000000, 0x8000000000000000
};

int main()
{
	int GetIndex(int rank, int file); 
	struct Board InitPosition(char* FEN_str);
	U64 KingMoves(U64 king_loc, U64 all_pieces, U64 *clear_file);
	U64 KnightMoves (U64 knight_loc, U64 all_pieces, U64 *clear_file);
	U64 WhitePawnMoves(U64 pawn_loc, U64 all_pieces, U64 black_pieces, U64 *clear_file, U64 *mask_rank);
	U64 BlackPawnMoves(U64 pawn_loc, U64 all_pieces, U64 white_pieces, U64 *clear_file, U64 *mask_rank);
	U64 StraightAttacks(U64 piece_BB, U64 own_pieces, U64 all_pieces, U64 *mask_file, U64 *mask_rank);
	U64 DiagonalAttacks(U64 piece_BB, U64 own_pieces, U64 all_pieces, U64 *mask_antidiagonal, U64 *mask_diagonal);
	U64 QueenMoves(U64 queen_loc, U64 own_pieces, U64 all_pieces, U64 *mask_file, U64 *mask_rank, U64 *mask_antidiagonal, U64 *mask_diagonal) ;
	U64 white_king_moves, white_knight_moves, black_pawn_moves, white_rook_moves, white_bishop_moves, white_queen_moves;
	
	char FEN_str[] = "rnbqkbnr/pppppppp/8/8/8/R6R/PPPPPPPP/1NBQKBN1 w KQkq - 0 1";
	struct Board StartingBoard;

	StartingBoard = InitPosition(FEN_str);

	white_king_moves = KingMoves(StartingBoard.white_king, StartingBoard.all_white_pieces, ClearFile);
	white_knight_moves = KnightMoves(StartingBoard.white_knights, StartingBoard.all_white_pieces, ClearFile);
	black_pawn_moves = BlackPawnMoves(StartingBoard.black_pawns, StartingBoard.all_pieces, StartingBoard.all_white_pieces, ClearFile, MaskRank);
	white_rook_moves = StraightAttacks(StartingBoard.white_rooks, StartingBoard.all_white_pieces, StartingBoard.all_pieces, MaskFile, MaskRank);
	white_bishop_moves = DiagonalAttacks(StartingBoard.white_bishops, StartingBoard.all_white_pieces, StartingBoard.all_pieces, AntiDiagonalMasks, DiagonalMasks);
	white_queen_moves = QueenMoves(StartingBoard.white_queen, StartingBoard.all_white_pieces, StartingBoard.all_pieces, MaskFile, MaskRank, AntiDiagonalMasks, DiagonalMasks);
	printf ("Knight moves: %llu\n", white_rook_moves);
	return 0;
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
	// e.g. index = 64
	int rank = index / 8;

	return rank;
}

int GetFile(int index) {
	int file = index % 8;

	return file;
}

/* Takes current king location bitboard, own pieces bitboard and returns moves */
U64 KingMoves(U64 king_loc, U64 own_pieces, U64 *clear_file) {
	U64 a_file_clip, h_file_clip, spot_1, spot_2, spot_3, spot_4, spot_5, spot_6, spot_7, spot_8; // 8 possible moves for the King
	U64 valid_moves;
	// if king is on a or h file, it will clear the bitboard (give 0)
	a_file_clip = king_loc & clear_file[0];
	h_file_clip = king_loc & clear_file[7];

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

	// FormatMoves(U64 valid_moves, king_loc)

	return valid_moves;
}

U64 KnightMoves(U64 knight_loc, U64 own_pieces, U64 *clear_file) {
	U64 a_file_clip, h_file_clip, ab_file_clip, gh_file_clip;
	U64 spot_1, spot_2, spot_3, spot_4, spot_5, spot_6, spot_7, spot_8;
	U64 valid_moves;

	/* if N is on a or h file, the first two will yield empty bitboards
	if N is on a,b file it will yield the 3rd empty BB, if on g,h, it will yield empty BB */

	a_file_clip = knight_loc & clear_file[0];
	h_file_clip = knight_loc & clear_file[7];
	ab_file_clip = knight_loc & clear_file[0] & clear_file[1];
	gh_file_clip = knight_loc & clear_file[6] & clear_file[7];

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
	// take the passed array, fill it in, it will be an object on a higher scope 
	int counter = 0;

	while (piece_BB != 0) {
		index_array[counter] = GetSetBit(piece_BB);
		piece_BB &= ~(1 << index_array[counter]);
		counter++;
	}
}

U64 StraightAttacks(U64 piece_BB, U64 own_pieces, U64 all_pieces, U64 *mask_file, U64 *mask_rank) {
	U64 valid_moves = 0;
	int index, file, rank;
	int piece_locations[10] = {0};
	int counter = 0;

	FilterPieces(piece_BB, piece_locations);

	while (piece_locations[counter] != 0) { 
		U64 current_piece_BB = 1;
		// once the condition is met, there are no more pieces to generate moves for
		file = GetFile(piece_locations[counter]);
		rank = GetRank(piece_locations[counter]);
		current_piece_BB <<= piece_locations[counter];

		valid_moves |= GenerateRayAttacks(mask_rank[rank], mask_file[file], all_pieces, current_piece_BB) & (~own_pieces);
		counter++;
	}

	return valid_moves;
}	

U64 DiagonalAttacks(U64 piece_BB, U64 own_pieces, U64 all_pieces, U64 *mask_antidiagonal, U64 *mask_diagonal) {
	U64 valid_moves;

	int diagonal, antidiagonal, index, file, rank;
	int GetDiagonal(U64 piece_BB, int file, int rank); 
	int GetAntiDiagonal(U64 piece_BB, int file, int rank);

	index = GetSetBit(piece_BB);
	file = GetFile(index);
	rank = GetRank(index);

	diagonal = GetDiagonal(piece_BB, file, rank);
	antidiagonal = GetAntiDiagonal(piece_BB, file, rank);
	valid_moves = GenerateRayAttacks(mask_antidiagonal[antidiagonal], mask_diagonal[diagonal], all_pieces, piece_BB) &
					(~own_pieces);

	return valid_moves;
}

int GetDiagonal(U64 piece_BB, int file, int rank) {
	int diagonal;

	diagonal = file - (rank - 7);

	return diagonal;
}

int GetAntiDiagonal(U64 piece_BB, int file, int rank) {
	int antidiagonal;

	antidiagonal = file + rank;

	return antidiagonal;
}

U64 QueenMoves(U64 queen_loc, U64 own_pieces, U64 all_pieces, U64 *mask_file, U64 *mask_rank, U64 *mask_antidiagonal, U64 *mask_diagonal) {
	U64 valid_moves, straight_attacks, diagonal_attacks;

	straight_attacks = StraightAttacks(queen_loc, own_pieces, all_pieces, mask_file, mask_rank);
	diagonal_attacks = DiagonalAttacks(queen_loc, own_pieces, all_pieces, mask_antidiagonal, mask_diagonal);
	valid_moves = straight_attacks | diagonal_attacks;

	return valid_moves;
}	



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
