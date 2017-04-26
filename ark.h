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

