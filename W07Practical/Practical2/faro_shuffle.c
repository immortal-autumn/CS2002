#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include "faro.h"

int check_overflow(long num) {
    if(INT_MAX - num < 0) {
        return 0;
    }
    return -1;
}

int check_ranksuit(int num) {
    if(num > 52 || num < 0) {
        return 0;
    }
    return -1;
}

int check_numeric(unsigned int num) {
    if(num > UINT_MAX || num < 0) {
        return 0;
    }
    return -1;
}

void flush() {
    int c;
    while((c = getchar()) == EOF || c != '\n')
    ;
}

int main(int argc, char *argv[]) {
    int size = 0; //size is size of the deck, k is desired position
    // char** decks_ptr = &decks;
    int k;

    while(2) {
        char* decks = malloc(256);
        if(decks == NULL) {
            printf("Memory is not enough!\n");
            return 0;
        } //Memory check

        if(argc != 2) {
            printf("Please input correct arguments e.g: ./main RANKSUIT or ./main NUMERCIAL\n");
            return 0;
        } // This checks the command line argument

        if(scanf("%i", &size) == 0) {
            printf("This is not a valid integer size!\n");
            return 0; 
        } //This checks the size input

        if(size == -1) {
            break;
        }

        if(check_overflow(size) == 0 && size % 2 != 0) {
            printf("This is not a valid integer size!\n");
            return 0;
        }
        flush();

        if(scanf("%i", &k) == 0) {
            printf("This is not a valid desired position\n");
            return 0;
        }

        flush();
        if(fgets(decks,size * 3,stdin) == NULL) {
            printf("The deck is invalid!\n");
            return 0;
        }

        // printf("DEBUGGING: size = %i, k = %c, decks = %s\n", size, (char)k, decks);
        if (strcmp(argv[1], "RANKSUIT") == 0)
        {
            if(check_ranksuit(size) == 0) {
                printf("Invalid size for RANKSUIT type!\n");
                return 0;
            }
            // printf("DEBUGGING: FLAG1 REACHED");
            ranksuit(decks, size, k);
        }
        else if (strcmp(argv[1], "NUMERICAL") == 0)
        {
            if(check_numeric(size) == 0) {
                printf("Invalid size for NUMERCIAL type!\n");
                return 0;
            }
            int len = strlen(decks);
            decks[len-1] = ' ';
            numerical(decks, size, k);
        }
        else {
            printf("Please input correct arguments e.g: ./main RANKSUIT or ./main NUMERCIAL");
            return 0;
        }
        free(decks);
    }
    return EXIT_SUCCESS;
}
