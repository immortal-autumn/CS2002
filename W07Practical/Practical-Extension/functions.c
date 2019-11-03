#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "print_faro_val.h"

#define H L'♥';
#define S L'♣';
#define D L'♠';
#define C L'♦';

typedef struct deck
{
    int index;
    wchar_t* card;
    struct deck *next;
} decks;

/*
 * This method checks the memory is enough to store the Linked list.
 */
void memory_check(decks* deck) {
    if(deck == NULL) {
        printf("No enough memory!");
        exit(0);
    }
}
/*
 * This function will stop the program and put a mark to check the error and prevent the execution error.
 */
void test() {
    printf("\n--------------Test case-----------------\n");
    exit(0);
}

wchar_t *encoding(wchar_t* card) {
    if(card[2] == 'S') {
        card[2] = S;
    }
    if(card[2] == 'H') {
        card[2] = H;
    }
    if(card[1] == 'D') {
        card[1] = D;
    }
    if(card[2] == 'C') {
        card[2] = C;
    }
    printf("%ls", card);
    return card;
}


/**
 * Create a new node with parameters index and card.
 * */
decks* create_node(int index, wchar_t* card) {
    decks* node = NULL;
    node = malloc(sizeof(decks)); 
    memory_check(node);
    node -> index = index;
    node -> card = card;
    node -> next = NULL;
    // printf("%i : %ls\n", node -> index, node -> card);
    return node;
}
/*
    This method will split string by white space and return a linked list
    In each linked list there are three elements: index, card and next node
    Mention that index should always add one in each loop and have to be REORDERED after each shuffle!
*/
decks* order_deck(char* unsorted_deck) {
    int current_index = 0;
    char *ptr;
    ptr = strtok(unsorted_deck, " ");  

    decks* head = NULL;
    head = create_node(current_index, (wchar_t*)ptr);
    decks* current = head;
    ptr = strtok(NULL, " ");
    // decks** current;

    while(ptr != NULL) {
        // current = &head;
        current_index++;
        current -> next = create_node(current_index, (wchar_t*)ptr);
        // current_index++;
        current = current -> next;
        // printf("\n%s : %i", current -> card, current->index);
        ptr = strtok(NULL, " ");
    }
    // test();
    return head;
}

/*
    This re-order the index from 0 to its size and get the total size of the deck.
*/
int index_reorder(decks* deck) {
    decks *current = deck;
    int index = 0;
    while(current != NULL) {
        current -> index = index;
        current = current -> next;
        index++;
    }
    // printf("-%i\n", deck -> index);
    return index;
}

/*
    Free all nodes in head. <- not sure
*/
void free_all(decks* head) {
    decks* prev;
    while(head != NULL) {
        prev = head;
        head = head -> next;
        free(prev);
    }
}

/*
    For decks of size 8:
    index: 0 1 2 3 | 4 5 6 7
    deck : 1 2 3 4 | 5 6 7 8

    WTC:
    i: 0 1 2 3 4 5 6 7
    d: 1 5 2 6 3 7 4 8
    o: 0 size/2+0 1 size/2+1 2 size/2+2 3 size/2+3

    Process: (Like merge sort)
    define intemediateWhat will be the result of this code and will this code crash and why? value: x
    swap: 1,4 2,4 3,5 5,6 
*/
decks* out_shuffle(decks* list, int size) {
    //Move pointers to their correct space
    decks* ptr1 = list;
    decks* ptr2 = list;
    int half = size/2;
    while(ptr2 -> index != half) {  
        ptr2 = ptr2 -> next;
    } //put pointer to the half

    //define a new linked list: shuffled
    decks* shuffled = NULL;
    shuffled = create_node(ptr1->index, ptr1->card);
    decks* current = shuffled;
    current->next = create_node(ptr2->index, ptr2->card);
    current = current -> next;

    for(int i = 0; i < half - 1; i++) {
        ptr1 = ptr1 -> next;
        ptr2 = ptr2 -> next;
        current -> next = create_node(ptr1->index, ptr1->card);
        current -> next -> next = create_node(ptr2->index, ptr2->card);
        current = current ->next -> next;
    }

    free_all(list);

    return shuffled;
}

/*
    For decks of size 8:
    i: 0 1 2 3 | 4 5 6 7
    d: 1 2 3 4 | 5 6 7 8

    WTC:
    i: 0 1 2 3 4 5 6 7
    d: 5 1 6 2 7 3 8 4
*/
decks* in_shuffle(decks* list, int size) {
    //Move pointers to their correct space
    decks* ptr1 = list;
    decks* ptr2 = list;
    int half = size/2;
    while(ptr2 -> index != half) {  
        ptr2 = ptr2 -> next;
    } //put pointer to the half

    //define a new linked list: shuffled
    decks* shuffled = NULL;
    shuffled = create_node(ptr2->index, ptr2->card);
    decks* current = shuffled;
    current->next = create_node(ptr1->index, ptr1->card);
    current = current -> next;

    for(int i = 0; i < half - 1; i++) {
        ptr1 = ptr1 -> next;
        ptr2 = ptr2 -> next;
        current -> next = create_node(ptr2->index, ptr2->card);
        current -> next -> next = create_node(ptr1->index, ptr1->card);
        current = current ->next -> next;
    }

    free_all(list);
    return shuffled;
}

/*
    Create two list and dynamically allocate the memory, the first list stores the binary of k from right to left,
    Second list stores the binary from left to right by putting the nth value of first list to max_index - nth position. 
*/
int* k_to_binary(int k){
    int *list = malloc(sizeof(256));
    int *result = malloc(sizeof(256));

    unsigned long current_index = 0;
    unsigned long max_index = 0;

    if(list == NULL) {
        printf("Binary list established failed: Memory not enough!");
        exit(0);
    }

    while(k > 0) {
        *(list + current_index) = k % 2;
        k/=2;
        current_index+=1;
    }

    max_index += current_index - 1;
    for(unsigned long i = 0; i <= max_index; i++) {
        *(result + i) = *(list + (max_index - i));
    }
    *(result + max_index + 1) = -1;

    free(list);

    return result;
}

/*
    This function will shuffle the deck and return this is an IN or OUT flag;
*/
decks* shuffle(decks* deck,int size, int i) {
    if(i == 0) {
        deck = out_shuffle(deck, size);
    }
    else if(i == 1) {
        deck = in_shuffle(deck, size);
    }
    else{
        printf("DEBUGGING: INVALID FLAGS");
        exit(0);
    }
    return deck;
}
/*
    This function will print the content of the deck by using print_faro_val() which was provided
*/
void print_content(int i, decks* deck) {
    decks* ptr = deck;
    if(i == 0) {
        print_faro_val("", PREFIX_OUT);
    }
    else if (i == 1)
    {
        print_faro_val("", PREFIX_IN);
    }
    while(ptr != NULL) {
        print_faro_val((char*)ptr->card, CARD_VAL);
        ptr = ptr -> next;
    }
    print_faro_val("", SUFFIX);
}

decks* transfer(decks* deck) {
    decks* ptr = deck;
    while(ptr != NULL) {
        ptr -> card = encoding(ptr -> card);
        wprintf(L"\n---->%s", ptr->card);
        ptr = ptr->next;
    }
    // test();
    return deck;
}

/*
    This is the main function of the ranksuit which call functions to get the final result.
*/
void ranksuit(char* deck, int size, int k) {
    decks* head = transfer(order_deck(deck));
    int *k_list = k_to_binary(k);
    int i;

    while((i = *(k_list)) != -1) {
        // printf("%i - %i - : ", i, k);
        head = shuffle(head, size, i);
        index_reorder(head);
        print_content(i, head);
        k_list++;
    }
    // test();
    // free(k_list);
}

/*
    This is the function of numerical which call functions to get the final result.
*/
void numerical(char* deck, int size, int k) {
    decks* head = transfer(order_deck(deck));
    int *k_list = k_to_binary(k);
    int i;

    while((i = *(k_list)) != -1) {
        // decks* ptr = head;
        // while(ptr != NULL) {
        //     // printf("%i : %s\n", ptr -> index, ptr -> card);
        //     ptr = ptr -> next;
        // }
        // printf("%i -- : ", i);
        head = shuffle(head, size, i);
        index_reorder(head);
        print_content(i, head);
        k_list++;
    }
}