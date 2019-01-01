#include <stdio.h>
#include <stdint.h>
#include "ray_attacks.h"
#include "ark.h"
#include "FEN_Parser.h" 

int main()
{
	U64 white_king_moves, white_knight_moves, black_pawn_moves, white_rook_moves, white_bishop_moves, white_queen_moves;
	
	//char FEN_str[] = "rnbqkbnr/pppppppp/8/8/8/R6R/PPPPPPPP/1NBQKBN1 w KQkq - 0 1";
	char FEN_str[] = "8/8/8/8/8/8/8/1N4N1 w KQkq - 0 1";
	struct Board StartingBoard;

	StartingBoard = InitPosition(FEN_str);

	white_king_moves = KingMoves(StartingBoard.white_king, StartingBoard.all_white_pieces, ClearFile);
	white_knight_moves = KnightMoves(StartingBoard.white_knights, StartingBoard.all_white_pieces);
	black_pawn_moves = BlackPawnMoves(StartingBoard.black_pawns, StartingBoard.all_pieces, StartingBoard.all_white_pieces, ClearFile, MaskRank);
	white_rook_moves = StraightAttacks(StartingBoard.white_rooks, StartingBoard.all_white_pieces, StartingBoard.all_pieces, MaskFile, MaskRank);
	white_bishop_moves = DiagonalAttacks(StartingBoard.white_bishops, StartingBoard.all_white_pieces, StartingBoard.all_pieces, AntiDiagonalMasks, DiagonalMasks);
	white_queen_moves = QueenMoves(StartingBoard.white_queen, StartingBoard.all_white_pieces, StartingBoard.all_pieces, MaskFile, MaskRank, AntiDiagonalMasks, DiagonalMasks);
	printf ("Knight moves: %llu\n", white_knight_moves);
	return 0;
}
