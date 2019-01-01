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
U64 START_WHITE = 0xffff000000000000; 
U64 START_BLACK = 0x000000000000ffff;
U64 START_WHITE_KNIGHT = 0x42; 

void test_knight()
{
    CU_ASSERT(KnightMoves(START_WHITE_KNIGHT, START_WHITE_KNIGHT) == 0xa51800);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("learn", NULL, NULL);
    CU_add_test(suite, "first", test_knight);
    CU_curses_run_tests();

    return 0;
}
