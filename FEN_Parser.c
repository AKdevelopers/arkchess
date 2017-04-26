#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ark.h"

// uint16_t
/*
int main ()
{
    void ParseFEN(char *fen_str);
	char FEN_str[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    ParseFEN(FEN_str);

	return 0;
}
*/

int GetLEIndex(int rank, int file) {
    // A,B,..,H = 0,1,..7 for files e.g. A1 is {0,0}
    int squareIndex = (56 - (8 * rank)) + file;

    return squareIndex;
}

struct Board ParseFEN(char *fen_str) {
    struct Board GetBoardArray(char *placement);
    char* piece_placement = strtok(fen_str, " ");
    char* active_color = strtok(NULL, " ");
    char* castling_rights = strtok(NULL, " ");
    char* ep_square = strtok(NULL, " ");
    char* half_move_counter = strtok(NULL, " ");
    char* full_move_counter = strtok(NULL, " ");

    return GetBoardArray(piece_placement);
}

struct Board GetBoardArray(char *placement) 
{
    struct Board ParseBoard(char* board_tokens[]);
    int i = 0;
    char *p = strtok (placement, "/");
    char *array[8];

    while (p != NULL)
    {
        array[i++] = p;
        p = strtok (NULL, "/");
    }

    for (i = 0; i < 8; ++i) 
//        printf("%s\n", array[i]);

    return ParseBoard(array);

}

struct Board ParseBoard(char* board_tokens[]) {
    char board[8][8];
    struct Board PopulateBitBoard(char board[8][8], struct Board state);



    int counter;

    // Main code for creating board representation
    for (int i = 0; i < 8; i++) {
        counter = 0;
        for (int j = 0; board_tokens[i][j] != '\0';) {
            if (isdigit(board_tokens[i][j])) {
                int number_of_zeroes = board_tokens[i][j] - '0';
                for (int x = 0; x < number_of_zeroes; x++) {
                    board[i][x + counter] = '-';
                }
                counter += number_of_zeroes;
            }
            else {
                board[i][counter] = board_tokens[i][j];
                counter++;
            }
            j++;
        }
    }

    struct Board StartingBoard, PopulatedBoard; // create instance of bitboard representation
    memset(&StartingBoard, 0, sizeof(StartingBoard)); // set all values to 0
    PopulatedBoard = PopulateBitBoard(board, StartingBoard);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf ("%c ", board[i][j]);
        }
        printf ("\n");
    }  
    printf ("\n");
    return PopulatedBoard;  
}

struct Board PopulateBitBoard(char board[8][8], struct Board state) {
    int index;
    char piece;
    U64 shifter = 1;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == '-') {
                // do nothing, the square is empty
            }
            else {
                index = GetLEIndex(i, j);
                piece = board[i][j];

                if (piece == 'P' || piece == 'p') {
                    if (piece == 'P') { 
                        state.white_pawns |= shifter << (index); 
                    }
                    else { 
                        state.black_pawns |= shifter << (index);
                    }
                }  

                else if (piece == 'R' || piece == 'r') {
                    if (piece == 'R') { 
                        state.white_rooks |= shifter << (index); 
                    }
                    else { 
                        state.black_rooks |= shifter << (index);
                    }
                }
                else if (piece == 'N' || piece == 'n') {
                    if (piece == 'N') { 
                        state.white_knights |= shifter << (index); 
                    }
                    else { 
                        state.black_knights |= shifter << (index);
                    }
                }

                else if (piece == 'B' || piece == 'b') {
                    if (piece == 'B') { 
                        state.white_bishops |= shifter << (index); 
                    }
                    else { 
                        state.black_bishops |= shifter << (index);
                    }
                }      

                else if (piece == 'Q' || piece == 'q') {
                    if (piece == 'Q') { 
                        state.white_queen |= shifter << (index); 
                    }
                    else { 
                        state.black_queen |= shifter << (index);
                    }
                }  

                else if (piece == 'K' || piece == 'k') {
                    if (piece == 'K') { 
                        state.white_king |= shifter << (index); 
                    }
                    else { 
                        state.black_king |= shifter << (index);
                    }
                }  



            }
        }
    }

    state.all_white_pieces = state.white_pawns | state.white_rooks | state.white_knights |
                            state.white_bishops | state.white_queen | state.white_king;

    state.all_black_pieces = state.black_pawns | state.black_rooks | state.black_knights |
                            state.black_bishops | state.black_queen | state.black_king;

    state.all_pieces = state.all_white_pieces | state.all_black_pieces;

    return state;
}

