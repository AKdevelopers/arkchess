int GetLEIndex(int rank, int file);
struct Board ParseFEN(char *fen_str);
struct Board GetBoardArray(char *placement);
struct Board ParseBoard(char* board_tokens[]);
struct Board PopulateBitBoard(char board[8][8], struct Board state);

