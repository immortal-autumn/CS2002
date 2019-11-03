//functions.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void test() {
    printf("===============================FG===========================\n");
    exit(0);
}

char **order_deck(char *unsorted_deck, int size) {
    char *ptr;

    char *decks; //the list of String
    char **ptr_d = &decks;

    ptr = strtok(unsorted_deck, " ");

    // while(ptr != NULL) {
    //     *ptr_d = ptr;
    //     // printf("%s, %s, %li, %li\n", *ptr_d, ptr, (long)ptr_d, (long)ptr);
    //     ptr = strtok(NULL, " ");
    //     ptr_d++;
    // }
    printf("%i\n", size);

    for(int i = 0; i < size; i++) {
        *(ptr_d + i) = ptr;
        ptr = strtok(NULL, " ");
        // if(ptr == NULL) {
        //    break;
        // }
        printf("%i:==========%s========%li====\n",i, ptr, (long)(ptr_d+i));
    }

    return ptr_d;
}
/*
    For decks of size 8:
    i: 0 1 2 3 | 4 5 6 7
    d: 1 2 3 4 | 5 6 7 8

    WTC:
    i: 0 1 2 3 4 5 6 7
    d: 1 5 2 6 3 7 4 8

    Process: (Like merge sort)
    define intemediateWhat will be the result of this code and will this code crash and why? value: x
    swap: 1,4 2,4 3,5 5,6 
*/
char* out_shuffle(char** decks, int size) {
    char* in_decks;
    char** ptr = &in_decks;
    int current = 0;
    int half = size / 2;
    while(current != size / 2){
        *(ptr+=1) = *decks;
        *(ptr+=1) = *(decks +=half);
        // in_decks[2 * current - 1] = decks[current];
        // in_decks[2 * current] = decks[half + current];
        current+=1;
    }
    return in_decks;
}
/*
    For decks of size 8:
    i: 0 1 2 3 | 4 5 6 7
    d: 1 2 3 4 | 5 6 7 8

    WTC:
    i: 0 1 2 3 4 5 6 7
    d: 5 1 6 2 7 3 8 4
*/
char* in_shuffle(char** decks, int size) {
    char* out_decks;
    char** ptr = &out_decks;

    int current = 0;
    int half = size / 2;
    while(current != size / 2){
        *(ptr+=1) = *(decks +=half);
        *(ptr+=1) = *decks;
        // out_decks[2 * current - 1] = decks[half + current];
        // out_decks[2 * current] = decks[current];
        current+=1;
    }
    
    return out_decks;
}
/*
    This function convert an integer to binary arrays
*/
int k_to_binary(int k) {
    int binary;
    int *ptr = &binary;

    int i = 0;

    while(k > 0) {
        *(ptr++) = k % 2;
        k /= 2;
        i++;
    }

    return binary;
}

void show_content(char** decks, int binary) {
    char* content;
    if(binary == 0) {
        content = "OUT";
    }
    else{
        content = "IN";
    }
    printf("%s: ", content);
    
    // for(int i = 0; i < sizeof(decks); i++) {
    //     printf("%c ", decks[i]);
    // }
    while(*decks != 0) {
        printf("%s ", *decks);
        decks += 1;
    }

    printf("EoD\n");
}

void shuffle(char** decks, int size, int i) {
    if (i == 0) {
        *decks = in_shuffle(decks, size);
    }
    else if (i == 1) {
        *decks = out_shuffle(decks, size);
    }
    else
    {
        printf("DEBUGGING: ERROR FOR FUNCTION SHUFFLE!");
        exit(0);
    }
}

void ranksuit(char* decks, int size, int k) {
    // int divide = size / 2;
    char **suits = order_deck(decks, size);
    test();
    // char **list = &suits;
    // char** ptr = &suits;

    // malloc(suits = order_deck(decks, size));
    int binary = k_to_binary(k);
    int *ptr = &binary;
    // int** ptrb_adde = & ptr_b;


    while(binary != 0) {
        shuffle(suits, size, binary);
        ptr += 1;
    }
} 