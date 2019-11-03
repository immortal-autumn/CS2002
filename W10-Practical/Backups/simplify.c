#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "logic.h"
#include "laws.h"

/*
Define a linked list that record the expression
*/
typedef struct expressions {
	int *path;
	struct expressions *Left;
	struct expressions *Right;
} Path;


void mem_chk_node(Path *node) {
	if(node == NULL) {
		printf("No enough memory!");
        exit(0);
	}
} 

void mem_chk_ptr(int *path) {
	if(path == NULL) {
		printf("No enough memory!");
        exit(0);
	}
} 

void check() {
	printf("\n------------check point------------\n");
	exit(0);
}

//This will create a new node and malloc this node and int*
Path *create_node(int max_depth) {
	Path* list = (Path*)malloc(sizeof(Path));
	mem_chk_node(list);
	list -> path = (int*)malloc(sizeof(int) * max_depth);
	mem_chk_ptr(list -> path);
	list -> Left = NULL;
	list -> Right = NULL;
	return list;
}

// int * arrcpy(int* target, int max_depth) {
// 	int *array = (int*)malloc(sizeof(int) * max_depth);
// 	for(int i = 0; i < max_depth; i++) {
// 		array[i] = target[i];
// 	}
// 	free(target);
// 	return array;
// }

/*
This function will output paths and saves into linked list
*/
Path *set_path(Path *node, struct Expr *expr, int current_depth ,int max_depth) {
	//NULL or REACH TO THE MAXIMUM
	if(expr == NULL || current_depth > max_depth) {
		node->path[0] = -1;
		return node;
	}
	printf("\ncurrent_depth: %lu", sizeof(int));
	// check();
	//Left
	if(expr -> expr1 != NULL) {
		Path *new_node = create_node(max_depth + 1);
		node -> Left = new_node;
		// node -> Left -> path = arrcpy(node -> path, max_depth + 1);
		memcpy(node -> Left -> path, node -> path, sizeof(int) * (max_depth + 1));
		node -> Left -> path[current_depth] = 1;
		return set_path(node -> Left, expr -> expr1, current_depth++, max_depth);
	}
	//Right
	if(expr -> expr2 != NULL) {
		Path *new_node = create_node(max_depth + 1);
		node -> Right = new_node;
		// node -> Right -> path = arrcpy(node -> path, max_depth + 1);
		memcpy(node -> Right -> path, node -> path, sizeof(int) * (max_depth + 1));
		node -> Right -> path[current_depth] = 2;
		return set_path(node -> Right, expr -> expr2, current_depth++, max_depth);
	}
	//End
	node -> path[current_depth] = 0;
	return node;
}

int find_derivations(struct Expr *expr, Path *path, LawSearch searches[], LawApplication applies[], char* names[], int n_laws) {
	if(path -> Left == NULL || path -> Right == NULL) {
		for(int i = 0; i < n_laws; i++) {
			if(searches[i](expr, path -> path) == NULL)
		}
	}

	if(path -> Left != NULL) {
		return find_derivations(expr, path -> Left, searches, applies, names, n_laws);
	}
	if(path -> Right != NULL) {
		return find_derivations(expr, path -> Right, searches, applies, names, n_laws);
	}
}

/* Read lines with expressions from standard input.
 * Find derivations from each expression.
 * Use the indicated laws.
 * -> use searches
 * -> applies
 */
void find_derivations_for_strings(int max_depth,
		LawSearch searches[], LawApplication applies[], char* names[], int n_laws) {
	char* line = NULL;
	size_t len = 0;
	while (getline(&line, &len, stdin) != -1) {
		int size = strlen(line);
		if (size >= 1 && line[size-1] == '\n') {
			line[size-1] = '\0';
		}
		// Replace this line by a call of a function whose functionality is
		// specified by Part 1 of the specs.
		struct Expr *expr;
		if((expr = read_expr(line)) != NULL) {
			Path* list = create_node(max_depth + 1);
			list = set_path(list, expr, 0, max_depth); //<Fault1
			// printf("\n%i\n", expr->tag);
		} //test

	}
}

