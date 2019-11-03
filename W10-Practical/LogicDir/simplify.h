#ifndef SIMPLIFY_H
#define SIMPLIFY_H

#include "laws.h"

typedef struct Dep {
	int* path;
	int depth;
	struct Expr *current;
	struct Dep *next;	
} Depth;

Depth *create_dep(int* path, int depth, struct Expr *current);
void free_dep(Depth* head);
void cmp_min(int temp);
void find_final_result(struct Expr *expr, int current_depth, int max_depth, LawSearch searches[], LawApplication applies[], int n_laws);
void find_derivations_for_strings(int max_depth,
		LawSearch searches[], LawApplication applies[], char* names[], int n_laws);
Depth *find_stage(struct Expr *expr, LawSearch searches[], LawApplication applies[], int n_laws);
void check_stp(unsigned int a);
void check_ctn(unsigned int a);
	
#endif // SIMPLIFY_H
