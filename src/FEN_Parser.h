#include "types.h"
int GetLEIndex(int rank, int file);
void init_position(struct Board *pos, char *fen_str);
char parse_castling_rights(char *castling_rights);
void split_placement(char *rank_arr[8], char *placement);
void process_placement(char board_arr[][8], char *rank_arr[8]);
void fill_board(struct Board *pos, char board[][8]);
