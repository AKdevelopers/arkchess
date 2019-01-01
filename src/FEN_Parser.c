#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ark.h"
#include "FEN_Parser.h"
#include "types.h"

int GetLEIndex(int rank, int file) {
    // A,B,..,H = 0,1,..7 for files e.g. A1 is {0,0}
    int squareIndex = (56 - (8 * rank)) + file;

    return squareIndex;
}

/* Splits FEN string into respective arrays */
struct Board ParseFEN(char *fen_str) {
    char* piece_placement = strtok(fen_str, " "); // the array showing piece position i.e. rnbqkbnr/pppppppp/8/8/...
    char* active_color = strtok(NULL, " "); // array showing whose move it is (w or b)
    char* castling_rights = strtok(NULL, " "); // castling rights KQ/kq
    char* ep_square = strtok(NULL, " "); // en passant square
    char* half_move_counter = strtok(NULL, " "); 
    char* full_move_counter = strtok(NULL, " ");

    return GetBoardArray(piece_placement);
}

struct Board GetBoardArray(char *placement) 
{
    int i = 0;
    char *p = strtok (placement, "/");
    char *array[8];

    // splits position placement string into 8 arrays (according the 8 ranks, indexes 0-7 correspond to ranks 8 to 1 respectively
    while (p != NULL)
    {
        array[i++] = p;
        p = strtok (NULL, "/");
    }

    return ParseBoard(array);

}

struct Board ParseBoard(char* board_tokens[]) {
    char board[8][8];
    struct Board PopulateBitBoard(char board[8][8], struct Board state);

    int counter;

    /* Main code for creating board representation
    The idea is to create a board representation such that blank spaces are filled in with a dash while pieces are left untouched
    Before this stage we had something like this: rnbqkbnr/8/8... The number representation for blank spaces make it difficult to work with
    Converting to a 8x8 multidimensional arrays allows for easy bitboard creaiton later, we want to end up with something like this:
    rnbqkbnr/--------/--------/... 
    */
    for (int i = 0; i < 8; i++) { // i means rank, j means file
        counter = 0; // counts the file that is currently being filled in the new board representations
        // the reason we need a separate j counter is that j only reads from the old representation, while counter fills the new one
        for (int j = 0; board_tokens[i][j] != '\0';) { // continues until end of index (i.e. end of rank, going horizontally h8, h7, h6,...)
            if (isdigit(board_tokens[i][j])) { // if its a digit (i.e. blank squares referring to the FEN representation)
                int number_of_zeroes = board_tokens[i][j] - '0'; // if board_tokens[i][j] is 4, then 4 zeroes/blank spaces exist
                for (int x = 0; x < number_of_zeroes; x++) {
                    board[i][x + counter] = '-'; // print the requisite number of blank spaces
                }
                counter += number_of_zeroes; // at this point the blank spaces have been inserted, so counter needs to be incremented by the number of zeroes
            }
            else { // if its not a blank space, i.e. its a piece, simply add it to the new representation, increment counter and continue
                board[i][counter] = board_tokens[i][j];
                counter++;
            }
            j++; // move onto next file
        }
    }

    struct Board StartingBoard, PopulatedBoard; // create instance of bitboard representation
    memset(&StartingBoard, 0, sizeof(StartingBoard)); // set all values to 0
    PopulatedBoard = PopulateBitBoard(board, StartingBoard); // the argument is now a 8x8 multidim array, along with a struct Board member
    // this function returns fully populated bitboards


    // print the representation
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
    /* The code that populates every bitboard in the Board structure */
    int index;
    char piece;
    U64 shifter = 1;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == '-') {
                // do nothing, the square is empty
            }
            else { // there is a piece on the square
                index = GetLEIndex(i, j); // get index according to Little Endian mapping
                piece = board[i][j]; // store board[i][j] (which is a char e.g. P for white pawn) into piece

                /* Populates the appropriate bitboard. Since every bitboard is blank,
                shifter "spawns" a piece placeholder, and index then shifts it the appropriate indexes. */
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

    // Create the aggregate bitboards
    state.all_white_pieces = state.white_pawns | state.white_rooks | state.white_knights |
                            state.white_bishops | state.white_queen | state.white_king;

    state.all_black_pieces = state.black_pawns | state.black_rooks | state.black_knights |
                            state.black_bishops | state.black_queen | state.black_king;

    state.all_pieces = state.all_white_pieces | state.all_black_pieces;

    return state;
}

