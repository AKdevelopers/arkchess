#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "FEN_Parser.h"
#include "types.h"
#include "bits.h"

void *Malloc(int size) {
    void *result = malloc(size);
    if (result == NULL) {
        perror("malloc");
        exit(1);
    }

    return result;
}

void free_board(char **board) {
	for (int i = 0; i < 8; i++) {
		free(board[i]);
	}
	free(board);
}

void init_position(struct Board *pos, char *fen_str) {
    memset(pos, 0, sizeof(struct Board));
    char* piece_placement = strtok(fen_str, " "); 
	char* active_color = strtok(NULL, " "); 
    char* castling_rights = strtok(NULL, " "); 
    char* ep_square = strtok(NULL, " "); 
    char* half_move_counter = strtok(NULL, " "); 
    char* full_move_counter = strtok(NULL, " ");

	pos->colour_to_move = (strcmp(active_color, "w")) ? 1 : 0;
	pos->castling_rights = parse_castling_rights(castling_rights);
	pos->ep_square = (strcmp(ep_square, "-")) ? square_to_index(ep_square) : -1;
	pos->half_move_ctr = strtol(half_move_counter, NULL, 10);
	pos->full_move_ctr = strtol(full_move_counter, NULL, 10); 

    char *rank_arr[8];

	char **board_arr = Malloc(sizeof(char *) * 8);
	for (int i = 0; i < 8; i++) {
		board_arr[i] = Malloc(sizeof(char) * 8);
	}

    split_placement(rank_arr, piece_placement);
    process_placement(board_arr, rank_arr);
    fill_board(pos, board_arr);
	free_board(board_arr);
}

int square_to_index(char *square) {
    int file, rank;
    char files[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    for (int i = 0; i < 8; i++) {
        if (square[0] == files[i]) {
            file = i;
        }
    }
    
    rank = square[1] - '0' - 1;
    return get_index(rank, file);
}

int parse_castling_rights(char *castling_rights) {
    int cmap = 0;
    if (castling_rights[0] == 'K') {
        cmap |= KINGSIDE_WHITE;
    }

    if (castling_rights[1] == 'Q') {
        cmap |= QUEENSIDE_WHITE;
    }

    if (castling_rights[2] == 'k') {
        cmap |= KINGSIDE_BLACK;
    }


    if (castling_rights[3] == 'q') {
        cmap |= QUEENSIDE_BLACK;
    }

	return cmap;
}

void split_placement(char *rank_arr[8], char *placement) {
    int i = 0;
    char *p = strtok(placement, "/");
     
    // splits position placement string into 8 arrays (according the 8 ranks, indexes 0-7 correspond to ranks 8 to 1 respectively
    while (p != NULL && i < 8) {
        rank_arr[i++] = p;
        p = strtok (NULL, "/");
    }
}

void process_placement(char **board_arr, char *rank_arr[8]) {
    int counter;

    for (int i = 0; i < 8; i++) { // i means rank, j means file
        counter = 0; // counts the file that is currently being filled in the new board representations
        // the reason we need a separate j counter is that j only reads from the old representation, while counter fills the new one
        for (int j = 0; rank_arr[i][j] != '\0';) { // continues until end of index (i.e. end of rank, going horizontally h8, h7, h6,...)
            if (isdigit(rank_arr[i][j])) { // if its a digit (i.e. blank squares referring to the FEN representation)
                int number_of_zeroes = rank_arr[i][j] - '0'; // if board_tokens[i][j] is 4, then 4 zeroes/blank spaces exist
                for (int x = 0; x < number_of_zeroes; x++) {
                    board_arr[i][x + counter] = '-'; // print the requisite number of blank spaces
                }
                counter += number_of_zeroes; // at this point the blank spaces have been inserted, so counter needs to be incremented by the number of zeroes
            }
            else { // if its not a blank space, i.e. its a piece, simply add it to the new representation, increment counter and continue
                board_arr[i][counter] = rank_arr[i][j];
                counter++;
            }
            j++; // move onto next file
        }
    }
	
    // print the representation
	/*
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf ("%c ", board_arr[i][j]);
        }
        printf ("\n");
    }  
    printf ("\n");
	*/
	
}

void fill_board(struct Board *pos, char **board) {
    /* The code that populates every bitboard in the Board structure */
    int index;
    char piece;
	U64 shifter = 1;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            piece = board[i][j];
            index = GetLEIndex(i, j);

            switch(piece) {
                case '-':
					break;
                case 'P':
                    pos->white_pawns |= shifter << index;
                    break;
                case 'p':
                    pos->black_pawns |= shifter << index;
                    break;
                case 'K':
                    pos->white_king |= shifter << index;
                    break;
                case 'k':
                    pos->black_king |= shifter << index;
                    break;
                case 'Q':
                    pos->white_queen |= shifter << index;
                    break;
                case 'q':
                    pos->black_queen |= shifter << index;
                    break;
                case 'R':
                    pos->white_rooks |= shifter << index;
                    break;
                case 'r':
                    pos->black_rooks |= shifter << index;
                    break;
                case 'N':
                    pos->white_knights |= shifter << index;
                    break;
                case 'n':
                    pos->black_knights |= shifter << index;
                    break;
                case 'B':
                    pos->white_bishops |= shifter << index;
                    break;
                case 'b':
                    pos->black_bishops |= shifter << index;
                    break;
            }
        }
    }

    pos->all_white_pieces = pos->white_pawns | pos->white_bishops | pos->white_knights |
                                pos->white_rooks | pos->white_queen | pos->white_king;

    

    pos->all_black_pieces = pos->black_pawns | pos->black_bishops | pos->black_knights |
                                pos->black_rooks | pos->black_queen | pos->black_king;

    pos->all_pieces = pos->all_white_pieces | pos->all_black_pieces;
}

int GetLEIndex(int rank, int file) {
    // A,B,..,H = 0,1,..7 for files e.g. A1 is {0,0}
    int squareIndex = (56 - (8 * rank)) + file;

    return squareIndex;
}
