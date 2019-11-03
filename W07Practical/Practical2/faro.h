typedef struct deck
{
    char* card;
    struct deck* next;
} decks;

void test();
decks* create_node(int index, char* card); //create a new node
void memory_check(decks *deck); //check whether the memory is enough
decks *order_deck(char *unsorted_deck); //order the unsorted decks and return the sorted deck
void index_reorder(decks* deck); // Order the index of each linked list into right order
void free_all(decks* deck); //free all linked list
decks* out_shuffle(char** decks, int size); //function of out_shuffle
decks* in_shuffle(char** decks, int size); //function of in_shuffle
char *k_to_binary(int k); //function to convert k to binary list
void show_content(char **decks, int binary); // show the content of current decks
void shuffle(char **decks, int size, int i); //shuffle the deck
void ranksuit(char *decks, int size, int k); //main method to do the ranksuit
void check_numerical(char* decks); //check elements in linked lists are digits.
void numerical(char *decks, int size, int k); //main mthod to do the numerical