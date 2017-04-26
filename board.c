#include <stdio.h>
#include "ray_attacks.c"
#include "ark.c"
#include "FEN_Parser.c"
#include <stdint.h>

typedef uint16_t U16;

int main()
{
	int GetIndex(int rank, int file); 
	struct Board InitPosition(char* FEN_str);
	U64 KingMoves(U64 king_loc, U64 all_pieces, U64 *clear_file);
	U64 KnightMoves (U64 knight_loc, U64 all_pieces, U64 *clear_file);
	U64 WhitePawnMoves(U64 pawn_loc, U64 all_pieces, U64 black_pieces, U64 *clear_file, U64 *mask_rank);
	U64 BlackPawnMoves(U64 pawn_loc, U64 all_pieces, U64 white_pieces, U64 *clear_file, U64 *mask_rank);
	U64 RookMoves(U64 rook_loc, U64 own_pieces, U64 all_pieces, U64 *mask_file, U64 *mask_rank);
	U64 BishopMoves(U64 bishop_loc, U64 own_pieces, U64 all_pieces, U64 *mask_antidiagonal, U64 *mask_diagonal);
	U64 QueenMoves(U64 queen_loc, U64 own_pieces, U64 all_pieces, U64 *mask_file, U64 *mask_rank, U64 *mask_antidiagonal, U64 *mask_diagonal) ;
	U64 white_king_moves, white_knight_moves, black_pawn_moves, white_rook_moves, white_bishop_moves, white_queen_moves;
	
	char FEN_str[] = "rnbqkbnr/pppp1ppp/8/4p3/3N1N2/5N2/PPPPPPPP/RNBQKB1R w KQkq - 0 2";
	struct Board StartingBoard;

	StartingBoard = InitPosition(FEN_str);

	white_king_moves = KingMoves(StartingBoard.white_king, StartingBoard.all_white_pieces, ClearFile);
	white_knight_moves = KnightMoves(StartingBoard.white_knights, StartingBoard.all_white_pieces, ClearFile);
	black_pawn_moves = BlackPawnMoves(StartingBoard.black_pawns, StartingBoard.all_pieces, StartingBoard.all_white_pieces, ClearFile, MaskRank);
	white_rook_moves = RookMoves(StartingBoard.white_rooks, StartingBoard.all_white_pieces, StartingBoard.all_pieces, MaskFile, MaskRank);
	white_bishop_moves = BishopMoves(StartingBoard.white_bishops, StartingBoard.all_white_pieces, StartingBoard.all_pieces, AntiDiagonalMasks, DiagonalMasks);
	white_queen_moves = QueenMoves(StartingBoard.white_queen, StartingBoard.all_white_pieces, StartingBoard.all_pieces, MaskFile, MaskRank, AntiDiagonalMasks, DiagonalMasks);
	printf ("Knight moves: %llu\n", black_pawn_moves);
	return 0;
}

struct Board InitPosition(char* FEN_str) {
	struct Board state;
	state = ParseFEN(FEN_str);	

	return state;
}

/*
U16* EncodeMoveList(U64 location, U64 moves) {
	
}



*/


int GetIndex(int rank, int file) {
	// A,B,..,H = 0,1,..7 for files e.g. A1 is {0,0}
	int squareIndex = 8 * rank + file;

	return squareIndex;
}

int GetSetBit(U64 bitboard) {
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

U64 KingMoves(U64 king_loc, U64 own_pieces, U64 *clear_file) {
	U64 a_file_clip, h_file_clip, spot_1, spot_2, spot_3, spot_4, spot_5, spot_6, spot_7, spot_8;
	U64 valid_moves;
	// if king is on a or h file, it will clear the bitboard (give 0)
	a_file_clip = king_loc & clear_file[0];
	h_file_clip = king_loc & clear_file[7];

	spot_1 = a_file_clip << 7; 
	spot_8 = a_file_clip >> 1;
	spot_7 = a_file_clip >> 9;

	spot_3 = h_file_clip << 9;
	spot_4 = h_file_clip << 1;
	spot_5 = h_file_clip >> 7;

	spot_2 = king_loc << 8;
	spot_6 = king_loc >> 8; 

	valid_moves = spot_1 | spot_2 | spot_3 | spot_4 | spot_5 | spot_6 |
					spot_7 | spot_8;

	// & with not all_pieces of the side to get valid moves
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

	// a bug arose where two_step had no value assigned to it, generated a RN, therefore a move
	two_step = 0;

	/* not all pieces will yield a BB of empty squares, which when ANDed with pawn loc
	left shifted 8 (to the next rank) will determine is square in front 
	of pawn is empty or not
	*/
	a_file_clip = pawn_loc & clear_file[0];
	h_file_clip = pawn_loc & clear_file[7];
	one_step = (pawn_loc << 8) & ~all_pieces;

	/* will generate an empty bitboard if the pawn is not on the second rank */ 
	two_step = pawn_loc & mask_rank[1];
	two_step = (two_step << 8) & ~all_pieces; // checks 1 square in front
	two_step = (two_step << 8) & ~all_pieces; // checks 2 squares in front

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

	
	two_step = pawn_loc & mask_rank[6];
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



U64 RookMoves(U64 rook_loc, U64 own_pieces, U64 all_pieces, U64 *mask_file, U64 *mask_rank) {
	U64 valid_moves;
	int index, file, rank;

	index = GetSetBit(rook_loc);
	file = GetFile(index);
	rank = GetRank(index);

	valid_moves = GenerateRayAttacks(mask_rank[rank], mask_file[file], all_pieces, rook_loc) & (~own_pieces);
	//valid_moves = GenerateRayAttacks() & (~own_pieces);

	return valid_moves;
}	

U64 BishopMoves(U64 bishop_loc, U64 own_pieces, U64 all_pieces, U64 *mask_antidiagonal, U64 *mask_diagonal) {
	U64 valid_moves;

	int diagonal, antidiagonal, index, file, rank;
	int GetDiagonal(U64 bishop_loc, int file, int rank); 
	int GetAntiDiagonal(U64 bishop_loc, int file, int rank);

	index = GetSetBit(bishop_loc);
	file = GetFile(index);
	rank = GetRank(index);

	diagonal = GetDiagonal(bishop_loc, file, rank);
	antidiagonal = GetAntiDiagonal(bishop_loc, file, rank);
	valid_moves = GenerateRayAttacks(mask_antidiagonal[antidiagonal], mask_diagonal[diagonal], all_pieces, bishop_loc) &
					(~own_pieces);

	return valid_moves;
}

int GetDiagonal(U64 bishop_loc, int file, int rank) {
	int diagonal;

	diagonal = file - (rank - 7);

	return diagonal;
}

int GetAntiDiagonal(U64 bishop_loc, int file, int rank) {
	int antidiagonal;

	antidiagonal = file + rank;

	return antidiagonal;
}

U64 QueenMoves(U64 queen_loc, U64 own_pieces, U64 all_pieces, U64 *mask_file, U64 *mask_rank, U64 *mask_antidiagonal, U64 *mask_diagonal) {
	U64 valid_moves, straight_attacks, diagonal_attacks;

	straight_attacks = RookMoves(queen_loc, own_pieces, all_pieces, mask_file, mask_rank);
	diagonal_attacks = BishopMoves(queen_loc, own_pieces, all_pieces, mask_antidiagonal, mask_diagonal);
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