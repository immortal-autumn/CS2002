#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "print_faro_val.h"

typedef struct deck
{
    int index;
    char* card;
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

void test() {
    printf("\n--------------Test case-----------------\n");
    exit(0);
}
/**
 * Create a new node with parameters index and card.
 * */
decks * create_node(int index, char* card) {
    decks* node = malloc(sizeof(decks)); 
    memory_check(node);
    node -> index = index;
    node -> card = card;
    node -> next = NULL;
    // printf("%i : %s\n", node -> index, node -> card);
    return node;
}

/*
    Append the value at the final of the node
*/
void append(decks* head, decks* node) {
    if(head == NULL) {
        head = node;
    }
    else
    {
        decks* ptr = head;
        while(ptr -> next != NULL)
            ptr = ptr -> next;
        ptr -> next = node;  
    } 
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

    decks* head;
    decks** ptr_addr = &head;

    while(ptr != NULL) {
        if(head == NULL) {
            head = create_node(current_index, ptr);
        }
        else{
            append(head, create_node(current_index, ptr));
        }
        current_index++;
        // printf("\n%s : %i - %s", (char*)ptr, current_index, (char*)current -> card);
        ptr = strtok(NULL, " ");
    }

    // printf("%s", head -> next -> next -> card);
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
    decks* shuffled;
    // decks* temp;

    decks* ptr = list;
    decks* ptr2 = list;

    int half = size/2;
    while(ptr2 -> index != half) {  
        // printf("\n%i : %s ", ptr2 -> index, ptr2->card);
        ptr2 = ptr2 -> next;
    } //put pointer to the half
    // decks* temp = shuffled;

    for(int i = 0; i < half; i++) {
        // printf("%i : %s - %i : %s\n", ptr->index, ptr->card, ptr2->index, ptr2->card);
        if(shuffled == NULL) { //<-Mistakes
            shuffled = create_node(ptr->index,ptr->card);
        }
        else
        {
            append(shuffled, create_node(ptr->index,ptr->card));
        }
        append(shuffled, create_node(ptr2->index,ptr2->card));
        ptr = ptr -> next;
        ptr2 = ptr2 -> next;
    }

    free_all(list);

    decks* ptr3 = shuffled;

    while(ptr3 != NULL) {
        printf("%s : %i\n", ptr3 ->card, ptr3->index);
        ptr3 = ptr3 ->next;
    }
    // test();
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
    decks* shuffled;
    decks* temp;

    decks* ptr = list;
    decks* ptr2 = list;

    int half = size/2;
    while(ptr2 -> index != half) {
        ptr2 = ptr2 -> next;
    } //put pointer to the half

    for(int i = 0; i < half; i++) {
        if(shuffled == NULL) shuffled = create_node(ptr2->index,ptr2->card);
        else
        {
            temp = create_node(ptr2->index,ptr2->card);
            append(shuffled, temp);
        }
        temp = create_node(ptr->index,ptr->card);
        append(shuffled, temp);
    }

    free_all(list);

    return shuffled;
}

int k_to_binary(int k) {
    int binary = k % 2;
    k /= 2;
    return binary;
}

/*
    This function will shuffle the deck and return this is an IN or OUT flag;
*/
void shuffle(decks* deck,int size, int i) {
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
}

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
        print_faro_val(ptr->card, CARD_VAL);
        ptr = ptr -> next;
    }
    print_faro_val("", SUFFIX);
}

void ranksuit(char* deck, int size, int k) {
    decks* linked_list = order_deck(deck);

    k-=48; //convert char to int since positive number start with 48

    while(k > 0) {
        int i = k_to_binary(k);
        shuffle(linked_list, size, i);
        index_reorder(linked_list);
        print_content(i, linked_list);
    }
}