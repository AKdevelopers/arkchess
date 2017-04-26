#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main ()
{
    void parser(char* board_tokens[]);
    char buf[] ="r1bq1rk1/pppp1ppp/2n2n2/2b1p3/2B1P3/3P1N2/PPP2PPP/RNBQ1RK1";
    int i = 0;
    char *p = strtok (buf, "/");
    char *array[8];

    while (p != NULL)
    {
        array[i++] = p;
        p = strtok (NULL, "/");
    }

    for (i = 0; i < 8; ++i) 
        printf("%s\n", array[i]);

    parser(array);
    return 0;
}

void parser(char* board_tokens[]) {
    //printf ("%s\n", board_tokens[3]);
    char board[8][8];

    int counter;

    for (int i = 0; i < 8; i++) {
        counter = 0;
        for (int j = 0; board_tokens[i][j] != '\0';) {
            if (isdigit(board_tokens[i][j])) {
                int number_of_zeroes = board_tokens[i][j] - '0';
                //printf ("yes: %c, %i\n", board_tokens[i][j], number_of_zeroes);
                for (int x = 0; x < number_of_zeroes; x++) {
                    printf ("i: %i, x: %i, counter: %i, j: %i\n",
                             i, x, counter, j);
                    board[i][x + counter] = '-';
                    printf ("Square: %c\n", board[i][x + counter]);
                    
                }
                printf ("Counter before addition: %i, zeroes: %i\n", 
                    counter, number_of_zeroes);
                counter += number_of_zeroes;
                printf ("Counter after addition: %i, zeroes: %i\n", 
                    counter, number_of_zeroes);
                // the number tells us how many 0s to store 
                //printf ("yes %c, %i %i\n", board[i][j], i, j);
            }
            else {
                board[i][counter] = board_tokens[i][j];
                printf ("board[i][counter]: %c\n", board[i][counter]);
                printf ("counter char loop: %i, rank/i: %i, j: %i\n", 
                    counter, i, j);
                counter++;
            }
            j++;
        }
    }

    printf ("\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf ("%c", board[i][j]);
        }
        printf ("\n");
    }    
}
/*
if it is a digit



*/