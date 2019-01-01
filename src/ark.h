typedef unsigned long long U64;
struct Board {
    // White pieces
    U64 white_pawns;
    U64 white_rooks;
    U64 white_bishops;
    U64 white_knights;
    U64 white_queen;
    U64 white_king;

    // Black pieces
    U64 black_pawns;
    U64 black_rooks;
    U64 black_bishops;
    U64 black_knights;
    U64 black_queen;
    U64 black_king;

    // Colors
    U64 all_white_pieces;
    U64 all_black_pieces;

    // All pieces
    U64 all_pieces;
};

extern U64 ClearFile[8];
U64 MaskFile[8];
U64 ClearRank[8];
U64 MaskRank[8];
U64 DiagonalMasks[15];
U64 AntiDiagonalMasks[15];

int GetIndex(int rank, int file); 
struct Board InitPosition(char* FEN_str);
U64 KingMoves(U64 king_loc, U64 all_pieces, U64 *clear_file);
U64 KnightMoves (U64 knight_loc, U64 all_pieces);
U64 WhitePawnMoves(U64 pawn_loc, U64 all_pieces, U64 black_pieces, U64 *clear_file, U64 *mask_rank);
U64 BlackPawnMoves(U64 pawn_loc, U64 all_pieces, U64 white_pieces, U64 *clear_file, U64 *mask_rank);
U64 StraightAttacks(U64 piece_BB, U64 own_pieces, U64 all_pieces, U64 *mask_file, U64 *mask_rank);
U64 DiagonalAttacks(U64 piece_BB, U64 own_pieces, U64 all_pieces, U64 *mask_antidiagonal, U64 *mask_diagonal);
U64 QueenMoves(U64 queen_loc, U64 own_pieces, U64 all_pieces, U64 *mask_file, U64 *mask_rank, U64 *mask_antidiagonal, U64 *mask_diagonal) ;


