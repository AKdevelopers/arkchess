#include <CUnit/CUnit.h>
#include <CUnit/CUError.h>
#include <CUnit/TestDB.h>
#include <CUnit/TestRun.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/CUCurses.h>
#include "../src/ark.h"
#include "../src/FEN_Parser.h"
#include "../src/ray_attacks.h"
#include "../src/types.h"
#include "../src/vector.h"

U64 START_POS = 0xffff00000000ffff;
U64 START_BLACK = 0xffff000000000000; 
U64 START_WHITE = 0x000000000000ffff;
U64 START_WHITE_KNIGHT = 0x42; 
U64 MIDDLE_PIECE = 0x8000000;

void test_knight()
{
    CU_ASSERT(KnightMoves(START_WHITE_KNIGHT, START_WHITE_KNIGHT) == 0xa51800);
    CU_ASSERT(KnightMoves(START_WHITE_KNIGHT, START_WHITE) == 0xa50000);
    CU_ASSERT(KnightMoves(MIDDLE_PIECE, MIDDLE_PIECE) == 0x142200221400);
}

void test_process_move() {
    CU_ASSERT(process_move(1, 21, 0, 0) == 0x541);
}

void test_gen_knight_moves() {
    char *fen_str = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    struct Board pos = InitPosition(fen_str);
    U16 *move_list = generate_knight_moves(&pos);
    U16 expected[4] = {0x401, 0x481, 0x546, 0x5c6};
    for (int i = 0; i < vector_size(move_list); i++) {
        CU_ASSERT(move_list[i] == expected[i]);
    }
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
    CU_pSuite suite = CU_add_suite("learn", NULL, NULL);
    CU_add_test(suite, "test_knight_moves", test_knight);
    CU_add_test(suite, "test_bit_func", test_bit_func);
    CU_add_test(suite, "test_process_move", test_process_move);
    CU_add_test(suite, "test_generate_knight_moves", test_gen_knight_moves);
    CU_curses_run_tests();

    return 0;
}
