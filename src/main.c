#include <stdio.h>
#include <stdint.h>
#include "ray_attacks.h"
#include "move_gen.h"
#include "FEN_Parser.h" 

int main()
{
    char FEN_str[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	struct Board pos;

	init_position(&pos, FEN_str);

	return 0;
}
