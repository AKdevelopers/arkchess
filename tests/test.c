#include <CUnit/CUnit.h>
#include <CUnit/CUError.h>
#include <CUnit/TestDB.h>
#include <CUnit/TestRun.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
//#include <CUnit/CUCurses.h>
#include "../src/move_gen.h"
#include "../src/FEN_Parser.h"
#include "../src/ray_attacks.h"
#include "../src/types.h"
#include "../src/vector.h"
#include "../src/bits.h"

// testing constants
U64 START_POS = 0xffff00000000ffff;
U64 START_BLACK = 0xffff000000000000; 
U64 START_WHITE = 0x000000000000ffff;
U64 START_WHITE_KNIGHT = 0x42; 
U64 START_BLACK_KNIGHT = 0x4200000000000000;
U64 START_WHITE_ROOK_RIGHT = 0x80;
U64 START_WHITE_ROOK_LEFT = 0x1;
U64 START_WHITE_ROOK = 0x81;
U64 START_BLACK_ROOK = 0x8100000000000000;
U64 START_WHITE_BISHOP_RIGHT = 0x20;
U64 START_WHITE_KING = 0x10;
U64 MIDDLE_PIECE = 0x8000000; // d4
U64 MIDDLE_PIECE_2 = 0x100000; // e3
U64 BOTTOM_RIGHT = 0x80;
U64 TOP_RIGHT = 0x8000000000000000;
U64 BOTTOM_LEFT = 0x1;

void test_fen_parser() {
	struct Board pos;
	// test 1 : starting board
	char fen_1[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	init_position(&pos, fen_1);
    CU_ASSERT(pos.white_rooks == START_WHITE_ROOK);
    CU_ASSERT(pos.black_rooks == START_BLACK_ROOK);
    CU_ASSERT(pos.white_knights == START_WHITE_KNIGHT);
    CU_ASSERT(pos.black_knights == START_BLACK_KNIGHT);

    CU_ASSERT(pos.colour_to_move == 0);
    CU_ASSERT(pos.castling_rights == 15);
    CU_ASSERT(pos.ep_square == -1);
    CU_ASSERT(pos.half_move_ctr == 0);
    CU_ASSERT(pos.full_move_ctr == 1);

	// test 2 : empty board	
	char fen_2[] = "8/8/8/8/8/8/8/8 w KQkq - 0 1";
	init_position(&pos, fen_2);

	CU_ASSERT(pos.all_pieces == 0);

    CU_ASSERT(pos.colour_to_move == 0);
    CU_ASSERT(pos.castling_rights == 15);
    CU_ASSERT(pos.ep_square == -1);
    CU_ASSERT(pos.half_move_ctr == 0);
    CU_ASSERT(pos.full_move_ctr == 1);

    // test 3 : different configurations
	char fen_3[] = "8/8/8/8/8/8/8/8 b K--q d4 5 22";
	init_position(&pos, fen_3);

	CU_ASSERT(pos.all_pieces == 0);

    CU_ASSERT(pos.colour_to_move == 1);
    CU_ASSERT(pos.castling_rights == 9);
    CU_ASSERT(pos.ep_square == 27);
    CU_ASSERT(pos.half_move_ctr == 5);
    CU_ASSERT(pos.full_move_ctr == 22);

	// test 3 : complex position

}

void test_fill_move_list() {
	U16 *move_list = NULL;
    U16 expected[8] = {0x6ca, 0x6cc, 0x6d1, 0x6d5, 0x6e1, 0x6e5, 0x6ea, 0x6ec};
	U64 move_bb = MIDDLE_PIECE;
	int index = 27;
	fill_move_list(move_list, move_bb, index);
	for (int i = 0; i < vector_size(move_list); i++) {
		CU_ASSERT(move_list[i] == expected[i]);
	}
}

void test_generate_ray_attacks() {
	struct Board pos;
	// ROOK TESTS
	// test 1: rook in starting position (rook with own blocker)
    char fen_1[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	init_position(&pos, fen_1);
	CU_ASSERT(generate_ray_attacks(MaskRank[0], MaskFile[7], &pos, START_WHITE_ROOK_RIGHT) == 0);

	// test 2: rook on empty file and rank
    char fen_2[] = "8/8/8/8/3R4/8/8/8 w KQkq - 0 1";
	init_position(&pos, fen_2);
	CU_ASSERT(generate_ray_attacks(MaskRank[3], MaskFile[3], &pos, MIDDLE_PIECE) == 0x8080808f7080808);

	// test 3: rook with opponent blocker on rank only, empty file
    char fen_3[] = "8/8/1n1R3r/8/8/8/8/8 w KQkq - 0 1";
	init_position(&pos, fen_3);
	CU_ASSERT(generate_ray_attacks(MaskRank[5], MaskFile[3], &pos, 0x80000000000) == 0x808f60808080808);

	// rook with opponent blocker on file only, empty rank
    char fen_4[] = "3k4/8/3R4/8/8/3b4/8/8 w KQkq - 0 1";
	init_position(&pos, fen_4);
	CU_ASSERT(generate_ray_attacks(MaskRank[5], MaskFile[3], &pos, 0x80000000000) == 0x808f70808080000);
	
	// centrally placed rook with varying blockers
    char fen_5[] = "3R4/8/1p1R2N1/8/8/3b4/8/8 w KQkq - 0 1";
	init_position(&pos, fen_5);
	CU_ASSERT(generate_ray_attacks(MaskRank[5], MaskFile[3], &pos, 0x80000000000) == 0x8360808080000);
	
	// corner placed rook with varying blockers
    char fen_6[] = "R1r1r2R/8/8/7R/8/7r/8/7r w KQkq - 0 1";
	init_position(&pos, fen_6);
	CU_ASSERT(generate_ray_attacks(MaskRank[7], MaskFile[7], &pos, TOP_RIGHT) == 0x7080800000000000);

	// BISHOP TESTS
    // test 7 : bishop on d4, empty board
    char fen_7[] = "8/8/8/8/3B4/8/8/8 w KQkq - 0 1";
	init_position(&pos, fen_7);
	CU_ASSERT(generate_ray_attacks(AntiDiagonalMask[6], DiagonalMask[7], &pos, MIDDLE_PIECE) == 0x8041221400142241);

    // test 8 : bishop in starting position, on starting board
    char fen_8[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	init_position(&pos, fen_8);
	CU_ASSERT(generate_ray_attacks(AntiDiagonalMask[5], DiagonalMask[12], &pos, START_WHITE_BISHOP_RIGHT) == 0);
    
    // test 9 : bishop on e3 
    char fen_9[] = "8/8/1b6/6p1/8/4B3/8/2P5 w KQkq - 0 1";
	init_position(&pos, fen_9);
	CU_ASSERT(generate_ray_attacks(AntiDiagonalMask[6], DiagonalMask[9], &pos, MIDDLE_PIECE_2) == 0x24428002840);
    
    // test 10 : bishop on e5 
    char fen_10[] = "8/2r3r1/8/4B3/8/2r3r1/8/8 w KQkq - 0 1";
	init_position(&pos, fen_10);
	CU_ASSERT(generate_ray_attacks(AntiDiagonalMask[8], DiagonalMask[7], &pos, 0x1000000000) == 0x44280028440000);

    // test 11 : bishop on e3, piece on next diagonal square
    char fen_11[] = "8/8/1b6/6p1/8/4B3/8/8 w KQkq - 0 1";
	init_position(&pos, fen_11);
	CU_ASSERT(generate_ray_attacks(AntiDiagonalMask[6], DiagonalMask[9], &pos, MIDDLE_PIECE_2) == 0x24428002844);
	
    // test 12 : bishop on d4, opponent blockers on c5 and e5
    char fen_12[] = "8/8/8/2b1p3/3B4/8/8/8 w KQkq - 0 1";
	init_position(&pos, fen_12);
	CU_ASSERT(generate_ray_attacks(AntiDiagonalMask[6], DiagonalMask[7], &pos, MIDDLE_PIECE) == 0x1400142241);
	
    // test 13 : bishop on d4, opponent blockers on b6 and f6
    char fen_13[] = "8/8/1b3p2/8/3B4/8/8/8 w KQkq - 0 1";
	init_position(&pos, fen_13);
	CU_ASSERT(generate_ray_attacks(AntiDiagonalMask[6], DiagonalMask[7], &pos, MIDDLE_PIECE) == 0x221400142241);
	
    // test 14 : bishop on f2, opponent blockers on b6 and h4
    char fen_14[] = "8/8/1b6/8/7p/8/5B2/8  w KQkq - 0 1";
	init_position(&pos, fen_14);
	CU_ASSERT(generate_ray_attacks(AntiDiagonalMask[6], DiagonalMask[11], &pos, 0x2000) == 0x20488500050);
	
    // test 15 : bishop on g1, opponent blockers on a7 and h2
    char fen_15[] = "8/b7/8/8/8/8/7p/6B1 w KQkq - 0 1";
	init_position(&pos, fen_15);
	CU_ASSERT(generate_ray_attacks(AntiDiagonalMask[6], DiagonalMask[13], &pos, 0x40) == 0x102040810a000);
	
    // test 16 : bishop on b1, opponent blockers on a2 and h7
    char fen_16[] = "8/7b/8/8/8/8/b7/1B6 w KQkq - 0 1";
	init_position(&pos, fen_16);
	CU_ASSERT(generate_ray_attacks(AntiDiagonalMask[1], DiagonalMask[8], &pos, 0x2) == 0x80402010080500);
}

void test_get_rook_moves() {
}

// test if KingMoves func is generating the correct
// bitboards
void test_king_bb() {
	CU_ASSERT(KingMoves(START_WHITE_KING, START_WHITE) == 0);
	// king on a-file
	CU_ASSERT(KingMoves(BOTTOM_LEFT, BOTTOM_LEFT) == 0x302);
	// king on h-file
	CU_ASSERT(KingMoves(TOP_RIGHT, TOP_RIGHT) == 0x40c0000000000000);
}

void test_knight()
{
    CU_ASSERT(KnightMoves(START_WHITE_KNIGHT, START_WHITE_KNIGHT) == 0xa51800);
    CU_ASSERT(KnightMoves(START_WHITE_KNIGHT, START_WHITE) == 0xa50000);
    CU_ASSERT(KnightMoves(MIDDLE_PIECE, MIDDLE_PIECE) == 0x142200221400);
    CU_ASSERT(KnightMoves(MIDDLE_PIECE, MIDDLE_PIECE | 0x142200221400) == 0);
    CU_ASSERT(KnightMoves(BOTTOM_RIGHT, BOTTOM_RIGHT) == 0x402000);
}

void test_process_move() {
    CU_ASSERT(process_move(1, 21, 0, 0) == 0x541);
}

void test_get_knight_moves() {
	struct Board pos;
	U16 *move_list = NULL;

    char fen_str_1[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    init_position(&pos, fen_str_1);
    move_list = get_knight_moves(&pos);
    U16 expected_1[4] = {0x50, 0x52, 0x195, 0x197};
    for (int i = 0; i < vector_size(move_list); i++) {
        CU_ASSERT(move_list[i] == expected_1[i]);
    }
	vector_free(move_list);

	// empty board, single knight in the middle of the board
    char fen_str_2[] = "8/8/8/8/3N4/8/8/8 w KQkq - 0 1";
	init_position(&pos, fen_str_2);
	move_list = get_knight_moves(&pos);
    U16 expected_2[8] = {0x6ca, 0x6cc, 0x6d1, 0x6d5, 0x6e1, 0x6e5, 0x6ea, 0x6ec};
    for (int i = 0; i < vector_size(move_list); i++) {
        CU_ASSERT(move_list[i] == expected_2[i]);
    }
	vector_free(move_list);

	
}

void test_bit_func() {
    CU_ASSERT(count_set_bits(START_WHITE_KNIGHT) == 2);
    CU_ASSERT(count_set_bits(0x8000000000000001) == 2);
    CU_ASSERT(count_set_bits(0x8100040022001081) == 8);

    int arr[2];
    // basic knight position 
    split_bits_index(START_WHITE_KNIGHT, 2, arr);
    CU_ASSERT(arr[0] == 1);
    CU_ASSERT(arr[1] == 6);
    // testing first and last bit
    split_bits_index(0x8000000000000001, 2, arr);
    CU_ASSERT(arr[0] == 0);
    CU_ASSERT(arr[1] == 63);
    // complex 8 bit set randomly
    int arr2[8];
    int expected[8] = {0, 7, 12, 25, 29, 42, 56, 63};
    split_bits_index(0x8100040022001081, 8, arr2);
    for (int i = 0; i < 8; i++) {
        CU_ASSERT(arr2[i] == expected[i]);
    }
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("main_test_suite", NULL, NULL);
    CU_add_test(suite, "test_knight_moves", test_knight);
    CU_add_test(suite, "test_bit_func", test_bit_func);
    CU_add_test(suite, "test_process_move", test_process_move);
    CU_add_test(suite, "test_get_knight_moves", test_get_knight_moves);
    CU_add_test(suite, "test_king_bitboard", test_king_bb);
    CU_add_test(suite, "test_fill_move_list", test_fill_move_list);
    CU_add_test(suite, "test_generate_ray_attacks", test_generate_ray_attacks);
    CU_add_test(suite, "test_fen_parser", test_fen_parser);
	CU_basic_run_tests();
    //CU_curses_run_tests();

    return 0;
}
