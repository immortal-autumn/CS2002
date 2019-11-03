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

/*Third Try:
Logic: pass -1 to searches and use this path to apply rules
This should be a recursive function.
The function will stop when it reaches to the maximum depth or there is no more expr1 and expr0
*/
int find_derivations(struct Expr *expr, int min_depth, int current_depth, int max_depth, 
		LawSearch searches[], LawApplication applies[], char* names[], int n_laws) {
	if(current_depth > max_depth) {
		return -1;
	}
	if(expr -> tag == isTrue) {
		if(min_depth == -1) {
			return current_depth;
		}
		else if(current_depth > min_depth) {
			return min_depth;
		}
		else
		{
			return current_depth;
		}
		
	}

	for(int i = 0; i < n_laws; i++) {
		int* path = non_path();
		while(path != NULL) {
			int* next_path = law_searches[i](expr, path);
			free(path);
			path = next_path;
			struct Expr *new_expr = law_applies[i](expr, next_path); 
			current_depth++;
			min_depth = find_derivations(expr, min_depth, current_depth, max_depth, searches, applies, names, n_laws);
		}
	}
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
		// int* path = non_path();
		int min_depth = find_derivations(expr, NULL, -1, 0 , max_depth, searches, applies, names, n_laws);
		// Replace this line by a call of a function whose functionality is
		// specified by Part 1 of the specs.		
		free_expr(expr);
	}
}
