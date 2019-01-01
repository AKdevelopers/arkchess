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

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("learn", NULL, NULL);
    CU_add_test(suite, "test_knight_moves", test_knight);
    CU_curses_run_tests();

    return 0;
}
