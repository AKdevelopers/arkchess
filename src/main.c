#include <stdio.h>
#include <stdint.h>
#include "ray_attacks.h"
#include "ark.h"
#include "FEN_Parser.h" 

int main()
{
	U64 white_king_moves, white_knight_moves, black_pawn_moves, white_rook_moves, white_bishop_moves, white_queen_moves;
	
	//char FEN_str[] = "rnbqkbnr/pppppppp/8/8/8/R6R/PPPPPPPP/1NBQKBN1 w KQkq - 0 1";
	//char FEN_str[] = "8/8/8/8/8/8/8/1N4N1 w KQkq - 0 1";
    char FEN_str[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	struct Board StartingPos;

	StartingPos = InitPosition(FEN_str);

	/*
	white_king_moves = KingMoves(StartingPos.white_king, StartingPos.all_white_pieces, ClearFile);
	white_knight_moves = KnightMoves(StartingPos.white_knights, StartingPos.all_white_pieces);
	black_pawn_moves = BlackPawnMoves(StartingPos.black_pawns, StartingPos.all_pieces, StartingPos.all_white_pieces, ClearFile, MaskRank);
	white_rook_moves = StraightAttacks(StartingPos.white_rooks, StartingPos.all_white_pieces, StartingPos.all_pieces, MaskFile, MaskRank);
	white_bishop_moves = DiagonalAttacks(StartingPos.white_bishops, StartingPos.all_white_pieces, StartingPos.all_pieces, AntiDiagonalMasks, DiagonalMasks);
	white_queen_moves = QueenMoves(StartingPos.white_queen, StartingPos.all_white_pieces, StartingPos.all_pieces, MaskFile, MaskRank, AntiDiagonalMasks, DiagonalMasks);
	*/
	printf ("Knight moves: %llu\n", white_knight_moves);
	return 0;
}
