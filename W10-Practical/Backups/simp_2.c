#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "logic.h"
#include "laws.h"

void check_stp(unsigned int a) {
	printf("\n-------------Check Point : %i-------------\n", a);
	exit(0);
}
void check_ctn(unsigned int a) {
	printf("\n-------------Check Point : %i-------------\n", a);
}

typedef struct Dep {
	int* path;
	int depth;
	struct Expr *current;
	struct Dep *next;	
} Depth;

/*This will create a new node with elements
*/
Depth *create_dep(int* path, int depth, struct Expr *current) {
	Depth *dep_list = malloc(sizeof(Depth));
	if(dep_list == NULL) {
		printf("Lack of memory!");
		exit(0);
	}
	dep_list -> depth = depth;
	dep_list -> path = path;
	dep_list -> next = NULL;
	dep_list -> current = current;

	return dep_list;
}

/*This will free all nodes in linked list.
*/
void free_dep(Depth* head) {
    Depth* prev;
    while(head != NULL) {
        prev = head;
        head = head -> next;
		free_expr(prev -> current);
        free(prev);
    }
}

/*This will return the minimum nodes in list
*/
int min_depth(Depth *list) {
	int min = -1;
	while(list != NULL) {
		if(list -> depth < min && list -> depth != -1) {
			min = list -> depth;
		}
	}
	return min;
}

/*Second Try:
Logic: pass -1 to searches and use this path to apply rules
This should be a recursive function.
The function will stop when it reaches to the maximum depth or there is no more expr1 and expr0
*/
Depth *find_derivations(Depth *dep_list, int current_depth, int max_depth, 
		LawSearch searches[], LawApplication applies[], char* names[], int n_laws) {
	if(dep_list ->depth > max_depth) {
		dep_list -> depth = -1;
		return dep_list;
	}
	if(dep_list -> current -> expr1 == NULL && dep_list -> current -> expr2 == NULL) {
		return dep_list;
	}

	Depth *new_node = NULL;
	Depth *ptr = new_node;
	struct Expr *exp_ptr = dep_list -> current;
	int* path = dep_list -> path;

	while (ptr != NULL) {
		for(int i = 0; i < n_laws; i++) {
			while(path != NULL) {
				int *next_path = law_searches[i](exp_ptr, path);
				if(new_node == NULL) {
					new_node = create_dep(next_path, current_depth, applies[i](exp_ptr, next_path));
				}
				else{
					ptr -> next = create_dep(next_path, current_depth, applies[i](exp_ptr, next_path));
				} 
			}
		}
		ptr = ptr -> next;
	}
	
	free_dep(dep_list);
	dep_list = new_node;
	return find_derivations(dep_list, current_depth++, max_depth, searches[], applies[], names[], n_laws);
}


/* Read lines with expressions from standard input.
 * Find derivations from each expression.
 * Use the indicated laws.
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
		struct Expr *expr = read_expr(line);
		int* path = non_path();
		Depth *list = create_dep(path, 0, expr);
		// Replace this line by a call of a function whose functionality is
		// specified by Part 1 of the specs.
		list = find_derivations(list, 0 , max_depth, searches, applies, names, n_laws);
		
		// printf("%i\n", output);

		// free(path);
		free_expr(expr);
	}
}
