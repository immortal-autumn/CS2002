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

int min_depth = -1;
// int temp_result = -1;

typedef struct Dep {
	struct Expr *expr;
	struct Dep *next;	
} Depth;

/*This will free all nodes in linked list.
*/
void free_dep(Depth* head) {
    Depth* prev;
    while(head != NULL) {
        prev = head;
        head = head -> next;
		free_expr(prev -> expr);
        free(prev);
    }
}

void print_dep(Depth* head) {
	int i = 0;
	while (head != NULL) {
		printf("\n%i : ", i++);
		print_expr(head -> expr);
		head = head -> next;
	}
}

/*This will create a new node with elements
*/
Depth *create_dep(struct Expr *expr) {
	Depth *dep_list = (Depth*)malloc(sizeof(Depth));
	if(dep_list == NULL) {
		printf("Lack of memory!");
		exit(0);
	}
	dep_list -> expr = expr;
	dep_list -> next = NULL;
	return dep_list;
}

void cmp_min(int temp) {
	if(min_depth == -1) {
		min_depth = temp;
	}
	else {
		if(temp < min_depth && temp != -1) {
			min_depth = temp;
		}
	}
	
}

/*Fourth Try:
Logic: pass -1 to searches and use this path to apply rules
This should be a recursive function.
The function will stop when it reaches to the maximum depth or there is no more expr1 and expr0
*/

/*This function will add all expressions to list and then free the expression
*/
Depth *find_stage(struct Expr *expr, LawSearch searches[], LawApplication applies[], int n_laws) {
	if(expr == NULL) {
		return NULL;
	}

	Depth *stage_list = NULL;
	Depth *list_ptr;

	for(int i = 0; i < n_laws; i++) {
		int* path = non_path();
		while(path != NULL) {
			int* next_path = searches[i](expr, path);
			free(path);
			path = next_path;
			if(path != NULL) {
				struct Expr *new_expr = applies[i](expr, path); 
				Depth *new_node = create_dep(new_expr);
				if(stage_list == NULL) {
					stage_list = new_node;
					list_ptr = stage_list;
				}
				else
				{
					list_ptr -> next = new_node;
					list_ptr = list_ptr -> next;
				}
				// printf("\n%i:\n", i);
				// print_expr(list_ptr -> expr);
			}
		}
	}
	// free_expr(expr);
	return stage_list;
}

void find_final_result(struct Expr *expr, int current_depth, int max_depth, LawSearch searches[], LawApplication applies[], char* names[], int n_laws) {
	if(current_depth > max_depth) {
		// check_ctn(0);
		return;
	}

	if(expr -> tag == isTrue) {
		// check_stp(current_depth);
		cmp_min(current_depth);
		return;
	}
	
	Depth *list = find_stage(expr, searches, applies, n_laws);
	Depth *ptr = list;
	current_depth++;
	// print_dep(list);
	while(ptr != NULL){
		// check_ctn(current_depth);
		// print_dep(ptr);
		find_final_result(ptr -> expr, current_depth ,max_depth, searches, applies, names, n_laws);
		// print_expr(ptr->expr);
		ptr = ptr -> next;
	}

	free_dep(list);
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
	
		// Replace this line by a call of a function whose functionality is
		// specified by Part 1 of the specs.	

		if(expr != NULL) {
			// Depth *result = find_final_result(expr, 0, max_depth, searches, applies, n_laws);
			find_final_result(expr, 0 ,max_depth, searches, applies, names, n_laws);
			printf("%i\n", min_depth);
			// printf
			min_depth = -1;
			free_expr(expr);
		}
	}
}
