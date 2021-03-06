#include "types.h"
int GetLEIndex(int rank, int file);
void init_position(struct Board *pos, char *fen_str);
int parse_castling_rights(char *castling_rights);
void split_placement(char *rank_arr[8], char *placement);
void process_placement(char **board_arr, char *rank_arr[8]);
void fill_board(struct Board *pos, char **board);
int square_to_index(char *square);
