#include <stdio.h>
#include "pell.h"

int main(void) {
    int i;

    printf("Length? ");
    scanf("%i", &i);
    printf("[");
    
    for (int n = 0; n < i; n ++) {
        if(n == i - 1) {
            printf("%i", pellcalc(n));
        }
        else
        {
            printf("%i, ", pellcalc(n));
        }
    }
    printf("]");
}