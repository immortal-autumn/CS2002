#include <stdio.h>
#include "pell.h"
/*
This file will write the second main method and will link with personlised_pell.c
*/
int start1, start2;

int main(void) {
    int length;
    int loop = 1;

    while(loop == 1) {
        start1 = start2 = 0;
        
        printf("Starting values? ");

        if(scanf("%i, %i", &start1, &start2) != 2) {
            printf("Invalid input");
            return 0;
        } 

        if (start1 == 0 && start2 == 0) {
            return 0;
        }

        if (start1 < 0 || start2 < 0) {
            printf("Invalid input");
            return 0;
        }

        printf("Length? ");
        
        if(scanf("%i", &length) != 1) {
            printf("Invalid input");
            return 0;
        }

        if (length < 0) {
            return 0;
        }

        printf("[");
        for (int n = 0; n < length; n++) {
            if(n == length - 1) {
                printf("%i", pellcalc(n));
            }
            else
            {
            printf("%i, ", pellcalc(n));
            }
        }
        printf("]\n");
    }
    return 0;
}