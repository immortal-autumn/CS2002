#ifndef SIMPLIFY_H
#define SIMPLIFY_H

#include "laws.h"

typedef struct expressions {
	int* path[50];
	struct expressions *Left;
	struct expressions *Right;
} Path;

void mem_chk_node(Path *node); //Check memory for linked list
void mem_chk_ptr(int *path); //Check memory for arrays
// int * arrcpy(int* target, int max_depth); //Copy arr
void check(); //Stop the program and check where goes wrong
Path *create_node(int max_depth); // Create new node and initialise
Path *set_path(Path *node, struct Expr *expr, int current_depth ,int max_depth); //Put path into linked list


void find_derivations_for_strings(int max_depth,
		LawSearch searches[], LawApplication applies[], char* names[], int n_laws);

#endif // SIMPLIFY_H
