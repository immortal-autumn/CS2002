#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "laws.h"
#include "logic.h"

/* A path is an array of numbers referring to a subexpression
 * of a given expression. Cf. the concept of Gorn address.
 * A '1' means follow the first child in the parse tree
 * and '2' means follow the second child in the parse tree
 * and '0' means 'take this node' (at the end of the path).
 * We use a path consisting of just '-1' for 'there is no path'.
 */
int *non_path() {
	int *path = malloc(sizeof(int));
	path[0] = -1;
	return path;
}

/* Non-path that doesn't need to be (should not be) freed.
 */
static int NON_PATH[] = {-1};

void print_path(int *path) {
	int i = 0;
	while (true) {
		printf("%d ", path[i]);
		if (path[i] < 1)
			break;
		else
			i++;
	}
	printf("\n");
}

/* Does this match left-hand, right-hand side of:
 * A|B = B|A
 * For uniformity, both are included, but they are of course the same.
 */
static bool is_comm_disj_lhs(struct Expr *expr) {
	return expr->tag == isDisj;
}
static bool is_comm_disj_rhs(struct Expr *expr) {
	return expr->tag == isDisj;
}

/* A&B = B&A
 */
static bool is_comm_conj_lhs(struct Expr *expr) {
	return expr->tag == isConj;
}
static bool is_comm_conj_rhs(struct Expr *expr) {
	return expr->tag == isConj;
}

/* (A|B)|C = A|(B|C)
 */
static bool is_assoc_disj_lhs(struct Expr *expr) {
	return expr->tag == isDisj && expr->expr1->tag == isDisj;
}
static bool is_assoc_disj_rhs(struct Expr *expr) {
	return expr->tag == isDisj && expr->expr2->tag == isDisj;
}

/* (A&B)&C = A&(B&C)
 */
static bool is_assoc_conj_lhs(struct Expr *expr) {
	return expr->tag == isConj && expr->expr1->tag == isConj;
}
static bool is_assoc_conj_rhs(struct Expr *expr) {
	return expr->tag == isConj && expr->expr2->tag == isConj;
}

/* A|(B&C) = (A|B)&(A|C)
 */
static bool is_distr_disj_lhs(struct Expr *expr) {
	return expr->tag == isDisj && expr->expr2->tag == isConj;
}
static bool is_distr_disj_rhs(struct Expr *expr) {
	return expr->tag == isConj &&
		expr->expr1->tag == isDisj &&
		expr->expr2->tag == isDisj &&
		equal_expr(expr->expr1->expr1, expr->expr2->expr1);
}

/* A&(B|C) = (A&B)|(A&C)
 */
static bool is_distr_conj_lhs(struct Expr *expr) {
	return expr->tag == isConj && expr->expr2->tag == isDisj;
}
static bool is_distr_conj_rhs(struct Expr *expr) {
	return expr->tag == isDisj &&
		expr->expr1->tag == isConj &&
		expr->expr2->tag == isConj &&
		equal_expr(expr->expr1->expr1, expr->expr2->expr1);
}

/* A|(A&B) = A
 */
static bool is_abs_disj_lhs(struct Expr *expr) {
	return expr->tag == isDisj && expr->expr2->tag == isConj &&
		equal_expr(expr->expr1, expr->expr2->expr1);
}
static bool is_abs_disj_rhs(struct Expr *expr) {
	(void) expr; // keep compiler happy
	return true;
}

/* A&(A|B) = A
 */
static bool is_abs_conj_lhs(struct Expr *expr) {
	return expr->tag == isConj && expr->expr2->tag == isDisj &&
		equal_expr(expr->expr1, expr->expr2->expr1);
}
static bool is_abs_conj_rhs(struct Expr *expr) {
	(void) expr; // keep compiler happy
	return true;
}

/* A|-A = T
 */
static bool is_compl_disj_lhs(struct Expr *expr) {
	return expr->tag == isDisj &&
		expr->expr2->tag == isNeg &&
		equal_expr(expr->expr1, expr->expr2->expr1);
}
static bool is_compl_disj_rhs(struct Expr *expr) {
	return expr->tag == isTrue;
}

/* A&-A = F
 */
static bool is_compl_conj_lhs(struct Expr *expr) {
	return expr->tag == isConj &&
		expr->expr2->tag == isNeg &&
		equal_expr(expr->expr1, expr->expr2->expr1);
}
static bool is_compl_conj_rhs(struct Expr *expr) {
	return expr->tag == isFalse;
}

/*******************************************/
/* Additional laws may be added here.      */
/*******************************************/
static bool is_negF_lhs(struct Expr *expr) {
	return expr -> tag == isNeg && expr -> expr1 -> tag == isFalse;
}

static bool is_negF_rhs(struct Expr *expr) {
	return expr -> tag == isTrue;
}

static bool is_negT_lhs(struct Expr *expr) {
	return expr -> tag == isNeg && expr -> expr1 -> tag == isTrue;
}

static bool is_negT_rhs(struct Expr *expr) {
	return expr -> tag == isFalse;
}

static bool is_iden_conj_lhs(struct Expr *expr) {
	return expr -> tag == isConj && expr -> expr1 -> tag == isTrue;
}

static bool is_iden_conj_rhs(struct Expr *expr) {
	return expr -> tag == isVar;
}

static bool is_iden_disj_lhs(struct Expr *expr) {
	return expr -> tag == isDisj && expr -> expr1 -> tag == isFalse;
}

static bool is_iden_disj_rhs(struct Expr *expr) {
	return expr -> tag == isVar;
}

static bool is_domin_conj_lhs(struct Expr *expr) {
	return expr -> tag == isConj && expr -> expr2 -> tag == isFalse;
}

static bool is_domin_conj_rhs(struct Expr *expr) {
	return expr -> tag == isFalse;
}

static bool is_domin_disj_lhs(struct Expr *expr) {
	return expr -> tag == isDisj && expr -> expr2 -> tag == isTrue;
}

static bool is_domin_disj_rhs(struct Expr *expr) {
	return expr -> tag == isTrue;
}

//De Morgan
static bool is_DeMorg_conj_lhs(struct Expr *expr) {
	return expr -> tag == isNeg && expr -> expr1 -> tag == isConj;
}

static bool is_DeMorg_conj_rhs(struct Expr *expr) {
	return expr -> tag == isDisj && expr -> expr1 -> tag == isNeg && expr -> expr2 -> tag == isNeg;
}

static bool is_DeMorg_disj_lhs(struct Expr *expr) {
	return expr -> tag == isNeg && expr -> expr1 -> tag == isDisj;
}

static bool is_DeMorg_disj_rhs(struct Expr *expr) {
	return expr -> tag == isConj && expr -> expr1 -> tag == isNeg && expr -> expr2 -> tag == isNeg;
}

//double negation
static bool is_DouNeg_lhs(struct Expr *expr) {
	return expr -> tag == isNeg && expr -> expr1 -> tag == isNeg;
}
//A is alwasys A, so we do not need rhs

//idempotence
static bool is_idem_conj_lhs(struct Expr *expr) {
	return expr -> tag == isConj && equal_expr(expr -> expr1, expr -> expr2);
}
static bool is_idem_conj_rhs(struct Expr *expr) {
	(void) expr; // keep compiler happy
	return true;
}
static bool is_idem_disj_lhs(struct Expr *expr) {
	return expr -> tag == isDisj && equal_expr(expr -> expr1, expr -> expr2);
}
static bool is_idem_disj_rhs(struct Expr *expr) {
	(void) expr; // keep compiler happy
	return true;
}

/*******************************************/
/* END ADDED                               */
/*******************************************/

/* Find subexpression satisfying predicate, at first position
 * lexicographically following the given path address.
 * Return NULL if none is found.
 */
static int *search_subexpression(struct Expr *expr, int *path, int depth,
		bool (*pred)(struct Expr *)) {
	// printf("depth %d\n", depth);
	// print_path(path);
	if (path[0] == -1) {
		if (pred(expr)) {
			int *found_path = malloc((depth+1) * sizeof(int));
			found_path[depth] = 0;
			return found_path;
		}
	}
	if (path[0] != 2) {
		switch (expr->tag) {
			case isDisj:
			case isConj:
			case isNeg:
			{
				int *recur_path = path[0] == 1 ? path+1 : NON_PATH;
				int *path1 = search_subexpression(expr->expr1, recur_path, depth+1, pred);
				if (path1 != NULL) {
					path1[depth] = 1;
					return path1;
				}
				break;
			}
			default:
				return NULL;
		}
	}
	switch (expr->tag) {
		case isDisj:
		case isConj:
		{
			int *recur_path = path[0] == 2 ? path+1 : NON_PATH;
			int *path2 = search_subexpression(expr->expr2, recur_path, depth+1, pred);
			if (path2 != NULL) {
				path2[depth] = 2;
				return path2;
			}
			break;
		}
		default:
			return NULL;
	}
	return NULL;
}

int *search_comm_disj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_comm_disj_lhs);
}
int *search_comm_disj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_comm_disj_rhs);
}
int *search_comm_conj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_comm_conj_lhs);
}
int *search_comm_conj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_comm_conj_rhs);
}

int *search_assoc_disj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_assoc_disj_lhs);
}
int *search_assoc_disj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_assoc_disj_rhs);
}
int *search_assoc_conj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_assoc_conj_lhs);
}
int *search_assoc_conj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_assoc_conj_rhs);
}

int *search_distr_disj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_distr_disj_lhs);
}
int *search_distr_disj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_distr_disj_rhs);
}
int *search_distr_conj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_distr_conj_lhs);
}
int *search_distr_conj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_distr_conj_rhs);
}

int *search_abs_disj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_abs_disj_lhs);
}
int *search_abs_disj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_abs_disj_rhs);
}
int *search_abs_conj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_abs_conj_lhs);
}
int *search_abs_conj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_abs_conj_rhs);
}

int *search_compl_disj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_compl_disj_lhs);
}
int *search_compl_disj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_compl_disj_rhs);
}
int *search_compl_conj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_compl_conj_lhs);
}
int *search_compl_conj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_compl_conj_rhs);
}

/*******************************************/
/* Additional laws may be added here.      */
/*******************************************/
int *search_negF_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_negF_lhs);
}
int *search_negF_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_negF_rhs);
}
int *search_negT_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_negT_lhs);
}
int *search_negT_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_negT_rhs);
}

int *search_iden_conj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_iden_conj_lhs);
}
int *search_iden_conj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_iden_conj_rhs);
}
int *search_iden_disj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_iden_disj_lhs);
}
int *search_iden_disj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_iden_disj_rhs);
}

int *search_domain_conj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_domin_conj_lhs);
}
int *search_domain_conj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_domin_conj_rhs);
}
int *search_domain_disj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_domin_disj_lhs);
}
int *search_domain_disj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_domin_disj_rhs);
}

int *search_DeMorg_conj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_DeMorg_conj_lhs);
}
int *search_DeMorg_conj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_DeMorg_conj_rhs);
}
int *search_DeMorg_disj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_DeMorg_disj_lhs);
}
int *search_DeMorg_disj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_DeMorg_disj_rhs);
}

int *search_DouNeg_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_DouNeg_lhs);
}

int *search_idem_conj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_idem_conj_lhs);
}
int *search_idem_conj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_idem_conj_rhs);
}
int *search_idem_disj_lhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_idem_disj_lhs);
}
int *search_idem_disj_rhs(struct Expr *expr, int *path) {
	return search_subexpression(expr, path, 0, is_idem_disj_rhs);
}
/*******************************************/
/* END ADDED                               */
/*******************************************/

/* A|B => B|A or A|B <= B|A, which is the same.
 */
static struct Expr *apply_comm_disj_forward(struct Expr *expr) {
	return make_disj(copy_expr(expr->expr2), copy_expr(expr->expr1));
}
static struct Expr *apply_comm_disj_backward(struct Expr *expr) {
	return apply_comm_disj_forward(expr);
}

/* A&B => B&A or A&B <= B&A, which is the same.
 */
static struct Expr *apply_comm_conj_forward(struct Expr *expr) {
	return make_conj(copy_expr(expr->expr2), copy_expr(expr->expr1));
}
static struct Expr *apply_comm_conj_backward(struct Expr *expr) {
	return apply_comm_conj_forward(expr);
}

/* (A|B)|C => A|(B|C)
 */
static struct Expr *apply_assoc_disj_forward(struct Expr *expr) {
	return make_disj(copy_expr(expr->expr1->expr1),
			make_disj(copy_expr(expr->expr1->expr2), copy_expr(expr->expr2)));
}
/* (A|B)|C <= A|(B|C)
 */
static struct Expr *apply_assoc_disj_backward(struct Expr *expr) {
	return make_disj(
			make_disj(copy_expr(expr->expr1), copy_expr(expr->expr2->expr1)),
				copy_expr(expr->expr2->expr2));
}

/* (A&B)&C => A&(B&C)
 */
static struct Expr *apply_assoc_conj_forward(struct Expr *expr) {
	return make_conj(copy_expr(expr->expr1->expr1),
			make_conj(copy_expr(expr->expr1->expr2), copy_expr(expr->expr2)));
}
/* (A&B)&C <= A&(B&C)
 */
static struct Expr *apply_assoc_conj_backward(struct Expr *expr) {
	return make_conj(
			make_conj(copy_expr(expr->expr1), copy_expr(expr->expr2->expr1)),
				copy_expr(expr->expr2->expr2));
}

/* A|(B&C) => (A|B)&(A|C)
 */
static struct Expr *apply_distr_disj_forward(struct Expr *expr) {
	return make_conj(
			make_disj(copy_expr(expr->expr1), copy_expr(expr->expr2->expr1)),
			make_disj(copy_expr(expr->expr1), copy_expr(expr->expr2->expr2)));
}
/* A|(B&C) <= (A|B)&(A|C)
 */
static struct Expr *apply_distr_disj_backward(struct Expr *expr) {
	return make_disj(copy_expr(expr->expr1->expr1),
			make_conj(copy_expr(expr->expr1->expr2), copy_expr(expr->expr2->expr2)));
}

/* A&(B|C) => (A&B)|(A&C)
 */
static struct Expr *apply_distr_conj_forward(struct Expr *expr) {
	return make_disj(
			make_conj(copy_expr(expr->expr1), copy_expr(expr->expr2->expr1)),
			make_conj(copy_expr(expr->expr1), copy_expr(expr->expr2->expr2)));
}
/* A&(B|C) <= (A&B)|(A&C)
 */
static struct Expr *apply_distr_conj_backward(struct Expr *expr) {
	return make_conj(copy_expr(expr->expr1->expr1),
			make_disj(copy_expr(expr->expr1->expr2), copy_expr(expr->expr2->expr2)));
}

/* A|(A&B) => A, cannot easily be used backwards.
 */
static struct Expr *apply_abs_disj_forward(struct Expr *expr) {
	return copy_expr(expr->expr1);
}

/* A&(A|B) => A, cannot easily be used backwards.
 */
static struct Expr *apply_abs_conj_forward(struct Expr *expr) {
	return copy_expr(expr->expr1);
}

/* A|-A => T, cannot easily be used backwards.
 */
static struct Expr *apply_compl_disj_forward(struct Expr *expr) {
	(void) expr; // keep compiler happy
	return make_true();
}

/* A&-A => F, cannot easily be used backwards.
 */
static struct Expr *apply_compl_conj_forward(struct Expr *expr) {
	(void) expr; // keep compiler happy
	return make_false();
}

/*******************************************/
/* Additional laws may be added here.      */
/*******************************************/
static struct Expr *apply_negF_forward(struct Expr *expr) {
	(void) expr; // keep compiler happy
	return make_true();
}
static struct Expr *apply_negF_backward(struct Expr *expr) {
	(void) expr; // keep compiler happy
	return make_neg(make_false());
}
static struct Expr *apply_negT_forward(struct Expr *expr) {
	(void) expr; // keep compiler happy
	return make_false();
}
static struct Expr *apply_negT_backward(struct Expr *expr) {
	(void) expr; // keep compiler happy
	return make_neg(make_true());
}

static struct Expr *apply_iden_conj_forward(struct Expr *expr) {
	return copy_expr(expr -> expr1);
}
static struct Expr *apply_iden_conj_backward(struct Expr *expr) {
	return make_conj(copy_expr(expr), make_true());
}
static struct Expr *apply_iden_disj_forward(struct Expr *expr) {
	return copy_expr(expr -> expr1);
}
static struct Expr *apply_iden_disj_backward(struct Expr *expr) {
	return make_disj(copy_expr(expr), make_false());
}
/*backwards can use complement, cannot find A from expr*
*/
static struct Expr *apply_domain_conj_forward(struct Expr *expr) {
	(void) expr; // keep compiler happy
	return make_false();
}
static struct Expr *apply_domain_disj_forward(struct Expr *expr) {
	(void) expr; // keep compiler happy
	return make_true();
}

//De Morgan
static struct Expr *apply_DeMorg_conj_forward(struct Expr *expr) {
	return make_disj(make_neg(copy_expr(expr -> expr1 -> expr1)), make_neg(copy_expr(expr -> expr1 -> expr2)));
}

static struct Expr *apply_DeMorg_conj_backward(struct Expr *expr) {
	return make_neg(make_conj(copy_expr(expr -> expr1 -> expr1), copy_expr(expr -> expr2 -> expr1)));
}

static struct Expr *apply_DeMorg_disj_forward(struct Expr *expr) {
	return make_conj(make_neg(copy_expr(expr -> expr1 -> expr1)), make_neg(copy_expr(expr -> expr1 -> expr2)));
}

static struct Expr *apply_DeMorg_disj_backward(struct Expr *expr) {
	return make_neg(make_disj(copy_expr(expr -> expr1 -> expr1), copy_expr(expr -> expr2 -> expr1)));
}

// Double Negation
static struct Expr *apply_DouNeg_forward(struct Expr *expr) {
	return copy_expr(expr -> expr1 -> expr1);
}

//idempotence
static struct Expr *apply_idem_conj_forward(struct Expr *expr) {
	return copy_expr(expr -> expr1);
}
static struct Expr *apply_idem_conj_backward(struct Expr *expr) {
	return make_conj(copy_expr(expr), copy_expr(expr));
}
static struct Expr *apply_idem_disj_forward(struct Expr *expr) {
	return copy_expr(expr -> expr1);
}
static struct Expr *apply_idem_disj_backward(struct Expr *expr) {
	return make_disj(copy_expr(expr),copy_expr(expr));
}
/*******************************************/
/* END ADDED                               */
/*******************************************/

/* Transform subexpression at path.
 */
static struct Expr *apply_law(struct Expr *expr, int *path,
		struct Expr *(*transform)(struct Expr *)) {
	if (path[0] == 0) {
		return transform(expr);
	} else if (path[0] == 1) {
		switch (expr->tag) {
			case isDisj:
				return make_disj(apply_law(expr->expr1, path+1, transform),
									copy_expr(expr->expr2));
			case isConj:
				return make_conj(apply_law(expr->expr1, path+1, transform),
									copy_expr(expr->expr2));
			case isNeg:
				return make_neg(apply_law(expr->expr1, path+1, transform));
			default:
				return NULL;
		}
	} else {
		switch (expr->tag) {
			case isDisj:
				return make_disj(copy_expr(expr->expr1),
									apply_law(expr->expr2, path+1, transform));
			case isConj:
				return make_conj(copy_expr(expr->expr1),
									apply_law(expr->expr2, path+1, transform));
			default:
				return NULL;
		}
	}
}

struct Expr *apply_comm_disj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_comm_disj_forward);
}
struct Expr *apply_comm_disj_backward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_comm_disj_backward);
}
struct Expr *apply_comm_conj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_comm_conj_forward);
}
struct Expr *apply_comm_conj_backward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_comm_conj_backward);
}

struct Expr *apply_assoc_disj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_assoc_disj_forward);
}
struct Expr *apply_assoc_disj_backward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_assoc_disj_backward);
}
struct Expr *apply_assoc_conj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_assoc_conj_forward);
}
struct Expr *apply_assoc_conj_backward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_assoc_conj_backward);
}

struct Expr *apply_distr_disj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_distr_disj_forward);
}
struct Expr *apply_distr_disj_backward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_distr_disj_backward);
}
struct Expr *apply_distr_conj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_distr_conj_forward);
}
struct Expr *apply_distr_conj_backward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_distr_conj_backward);
}

struct Expr *apply_abs_disj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_abs_disj_forward);
}
struct Expr *apply_abs_conj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_abs_conj_forward);
}

struct Expr *apply_compl_disj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_compl_disj_forward);
}
struct Expr *apply_compl_conj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_compl_conj_forward);
}

/*******************************************/
/* Additional laws may be added here.      */
/*******************************************/
struct Expr *apply_negF_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_negF_forward);
}
struct Expr *apply_negF_backward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_negF_backward);
}
struct Expr *apply_negT_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_negT_forward);
}
struct Expr *apply_negT_backward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_negT_backward);
}

struct Expr *apply_iden_conj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_iden_conj_forward);
}
struct Expr *apply_iden_conj_backward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_iden_conj_backward);
}
struct Expr *apply_iden_disj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_iden_disj_forward);
}
struct Expr *apply_iden_disj_backward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_iden_disj_backward);
}

struct Expr *apply_domain_conj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_domain_conj_forward);
}
struct Expr *apply_domain_disj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_domain_disj_forward);
}

struct Expr *apply_DeMorg_conj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_DeMorg_conj_forward);
}
struct Expr *apply_DeMorg_conj_backward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_DeMorg_conj_backward);
}
struct Expr *apply_DeMorg_disj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_DeMorg_disj_forward);
}
struct Expr *apply_DeMorg_disj_backward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_DeMorg_disj_backward);
}

struct Expr *apply_DouNeg_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_DouNeg_forward);
}

struct Expr *apply_idem_conj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_idem_conj_forward);
}
struct Expr *apply_idem_conj_backward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_idem_conj_backward);
}
struct Expr *apply_idem_disj_forward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_idem_disj_forward);
}
struct Expr *apply_idem_disj_backward_at(struct Expr *expr, int *path) {
	return apply_law(expr, path, apply_idem_disj_backward);
}
/*******************************************/
/* END ADDED                               */
/*******************************************/

/*******************************************/
/* For Part 1.                             */
/*******************************************/

LawSearch law_searches[] = {
	search_comm_disj_lhs,
	// omitted as redundant: search_comm_disj_rhs,
	search_comm_conj_lhs,
	// omitted as redundant: search_comm_conj_rhs,
	search_assoc_disj_lhs,
	search_assoc_disj_rhs,
	search_assoc_conj_lhs,
	search_assoc_conj_rhs,
	search_distr_disj_lhs,
	search_distr_disj_rhs,
	search_distr_conj_lhs,
	search_distr_conj_rhs,
	search_abs_disj_lhs,
	search_abs_conj_lhs,
	search_compl_disj_lhs,
	search_compl_conj_lhs
};

LawApplication law_applies[] = {
	apply_comm_disj_forward_at,
	// omitted as redundant: apply_comm_disj_backward_at,
	apply_comm_conj_forward_at,
	// omitted as redundant: apply_comm_conj_backward_at,
	apply_assoc_disj_forward_at,
	apply_assoc_disj_backward_at,
	apply_assoc_conj_forward_at,
	apply_assoc_conj_backward_at,
	apply_distr_disj_forward_at,
	apply_distr_disj_backward_at,
	apply_distr_conj_forward_at,
	apply_distr_conj_backward_at,
	apply_abs_disj_forward_at,
	apply_abs_conj_forward_at,
	apply_compl_disj_forward_at,
	apply_compl_conj_forward_at
};

char *law_names[] = {
	"commutative disj (forward)",
	"commutative conj (forward)",
	"associativity disj (forward)",
	"associativity disj (backward)",
	"associativity conj (forward)",
	"associativity conj (backward)",
	"distributivity disj (forward)",
	"distributivity disj (backward)",
	"distributivity conj (forward)",
	"distributivity conj (backward)",
	"absorption disj (forward)",
	"absorption conj (forward)",
	"complementation disj (forward)",
	"complementation conj (forward)"
};

int n_laws() {
	return sizeof(law_searches) / sizeof(LawSearch);
}

/*******************************************/
/* For Part 2.                             */
/*******************************************/

LawSearch extra_law_searches[] = {
	search_comm_disj_lhs,
	search_comm_conj_lhs,
	search_assoc_disj_lhs,
	search_assoc_disj_rhs,
	search_assoc_conj_lhs,
	search_assoc_conj_rhs,
	search_distr_disj_lhs,
	search_distr_disj_rhs,
	search_distr_conj_lhs,
	search_distr_conj_rhs,
	search_abs_disj_lhs,
	search_abs_conj_lhs,
	search_compl_disj_lhs,
	search_compl_conj_lhs,
	/*******************************************/
	/* Additional laws for Part 2 go here.     */
	/*******************************************/
	search_negF_lhs,
	search_negF_rhs,
	search_negT_lhs,
	search_negT_rhs,
	search_iden_conj_lhs,
	search_iden_conj_rhs,
	search_iden_disj_lhs,
	search_iden_disj_rhs,
	search_domain_conj_lhs,
	search_domain_disj_lhs,
	search_DeMorg_conj_lhs,
	search_DeMorg_conj_rhs,
	search_DeMorg_disj_lhs,
	search_DeMorg_disj_rhs,
	search_DouNeg_lhs,
	search_idem_conj_lhs,
	// search_idem_conj_rhs,
	search_idem_disj_lhs,
	// search_idem_disj_rhs
	/*******************************************/
	/* END ADDED                               */
	/*******************************************/
};

LawApplication extra_law_applies[] = {
	apply_comm_disj_forward_at,
	apply_comm_conj_forward_at,
	apply_assoc_disj_forward_at,
	apply_assoc_disj_backward_at,
	apply_assoc_conj_forward_at,
	apply_assoc_conj_backward_at,
	apply_distr_disj_forward_at,
	apply_distr_disj_backward_at,
	apply_distr_conj_forward_at,
	apply_distr_conj_backward_at,
	apply_abs_disj_forward_at,
	apply_abs_conj_forward_at,
	apply_compl_disj_forward_at,
	apply_compl_conj_forward_at,
	/*******************************************/
	/* Additional laws for Part 2 go here.     */
	/*******************************************/
	apply_negF_forward_at,
	apply_negF_backward_at,
	apply_negT_forward_at,
	apply_negT_backward_at,
	apply_iden_conj_forward_at,
	apply_iden_conj_backward_at,
	apply_iden_disj_forward_at,
	apply_iden_disj_backward_at,
	apply_domain_conj_forward_at,
	apply_domain_disj_forward_at,
	apply_DeMorg_conj_forward_at,
	apply_DeMorg_conj_backward_at,
	apply_DeMorg_disj_forward_at,
	apply_DeMorg_disj_backward_at,
	apply_DouNeg_forward_at,
	apply_idem_conj_forward_at,
	// apply_idem_conj_backward_at,
	apply_idem_disj_forward_at,
	// apply_idem_disj_backward_at
	/*******************************************/
	/* END ADDED                               */
	/*******************************************/
};

char *extra_law_names[] = {
	"commutative disj (forward)",
	"commutative conj (forward)",
	"associativity disj (forward)",
	"associativity disj (backward)",
	"associativity conj (forward)",
	"associativity conj (backward)",
	"distributivity disj (forward)",
	"distributivity disj (backward)",
	"distributivity conj (forward)",
	"distributivity conj (backward)",
	"absorption disj (forward)",
	"absorption conj (forward)",
	"complementation disj (forward)",
	"complementation conj (forward)"
	/*******************************************/
	/* Additional laws for Part 2 go here.     */
	/*******************************************/
	"negation false (forward)",
	"negation false (backward)",
	"negation true (forward)",
	"negation true (backward)",
	"identitiy conj (forward)",
	"identity conj (backward)",
	"identity disj (forward)",
	"identity disj (backward)",
	"domination conj (forward)",
	// "domination conj (backward)",
	"domination disj (forward)",
	// "domination disj (backward)"
	"De Morgan conj (forward)",
	"De Morgan conj (backward)",
	"De Morgan disj (forward)",
	"De Morgan disj (backward)",
	"Double Negation (forward)",
	"idempotence conj (forward)",
	// "idempotence conj (backward)",
	"idempotence disj (forward)",
	// "idempotence disj (backward)"
	/*******************************************/
	/* END ADDED                               */
	/*******************************************/
};

int n_extra_laws() {
	return sizeof(extra_law_searches) / sizeof(LawSearch);
}

/*******************************************/
/* For Part 3.                             */
/*******************************************/

LawSearch cnf_law_searches[] = {
	/*******************************************/
	/* Laws for Part 3 go here.                */
	/*******************************************/
	search_comm_disj_lhs,
	search_comm_conj_lhs,
	search_DeMorg_conj_lhs,
	search_DeMorg_conj_rhs,
	search_DeMorg_disj_lhs,
	search_DeMorg_disj_rhs,	
	// search_DouNeg_lhs,
	search_distr_disj_lhs,
	search_distr_disj_rhs,
	search_distr_conj_lhs,
	search_distr_conj_rhs,
	search_assoc_disj_lhs,
	search_assoc_disj_rhs,
	search_assoc_conj_lhs,
	search_assoc_conj_rhs,
	search_compl_disj_lhs,
	search_compl_conj_lhs,
	search_domain_conj_lhs,
	search_domain_disj_lhs,
	search_idem_conj_lhs,
	search_idem_disj_lhs,
	search_negF_lhs,
	search_negF_rhs,
	search_negT_lhs,
	search_negT_rhs,
	/*******************************************/
	/* END ADDED     -(-a|(b|c))&(-b|(a&c))                          */
	/*******************************************/
};

LawApplication cnf_law_applies[] = {
	/*******************************************/
	/* Laws for Part 3 go here.                */
	/*******************************************/
	// NULL // Keeping compiler happy. Remove this line after adding laws.
	apply_comm_disj_forward_at,
	apply_comm_conj_forward_at,
	apply_DeMorg_conj_forward_at,
	apply_DeMorg_conj_backward_at,
	apply_DeMorg_disj_forward_at,
	apply_DeMorg_disj_backward_at,
	// apply_DouNeg_forward_at,
	apply_distr_disj_forward_at,
	apply_distr_disj_backward_at,
	apply_distr_conj_forward_at,
	apply_distr_conj_backward_at,
	apply_assoc_disj_forward_at,
	apply_assoc_disj_backward_at,
	apply_assoc_conj_forward_at,
	apply_assoc_conj_backward_at,
	apply_compl_disj_forward_at,
	apply_compl_conj_forward_at,
	apply_domain_conj_forward_at,
	apply_domain_disj_forward_at,
	apply_idem_conj_forward_at,
	apply_idem_disj_forward_at,
	apply_negF_forward_at,
	apply_negF_backward_at,
	apply_negT_forward_at,
	apply_negT_backward_at,
	/*******************************************/
	/* END ADDED                               */
	/*******************************************/
};

char *cnf_law_names[] = {
	/*******************************************/
	/* Laws for Part 3 go here.                */
	/*******************************************/
	// NULL // Keeping compiler happy. Remove this line after adding laws.
	"commutative disj (forward)",
	"commutative conj (forward)",
	"De Morgan conj (forward)",
	"De Morgan conj (backward)",
	"De Morgan disj (forward)",
	"De Morgan disj (backward)",
	// "double negation (forward)",
	"distributivity disj (forward)",
	"distributivity disj (backward)",
	"distributivity conj (forward)",
	"distributivity conj (backward)",
	"associativity disj (forward)",
	"associativity disj (backward)",
	"associativity conj (forward)",
	"associativity conj (backward)",
	"complementation disj (forward)",
	"complementation conj (forward)",
	"apply domaination conj (forward)",
	"apply domination disj (forward)",
	"idempotence conj (forward)",
	"idempotence disj (forward)",
	"negation false (forward)",
	"negation false (backward)",
	"negation true (forward)",
	"negation true (backward)"
	/*******************************************/
	/* END ADDED                               */
	/*******************************************/
};

int n_cnf_laws() {
	return sizeof(cnf_law_searches) / sizeof(LawSearch);
}
