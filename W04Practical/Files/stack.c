#include <stdio.h>
#include <inttypes.h>

/* Print stack frame and that of recursive callers. */
static void print_recur(int64_t ret, int64_t rbp, int64_t next_rbp) {
	int64_t this_ret;
	asm("movq 8(%1), %0;" : "=r"(this_ret) : "r"(rbp));
	if (this_ret == ret) {
		long prev_rbp, val, quot, mod;
		int depth, len;
		char** end;
		char* start;
		// int64_t* addr_rbp;

		asm("movq (%1), %0;" : "=r"(prev_rbp) : "r"(rbp));
		asm("movq (%1), %0;" : "=r"(val) : "r"(rbp - 8));
		asm("movl (%1), %0;" : "=r"(depth) : "r"(rbp - 12));
		asm("movq (%1), %0;" : "=r"(end) : "r"(rbp - 24));
		asm("movq (%1), %0;" : "=r"(quot) : "r"(rbp - 32));
		asm("movq (%1), %0;" : "=r"(mod) : "r"(rbp - 40));
		asm("movq (%1), %0;" : "=r"(start) : "r"(rbp - 48));
		asm("movl (%1), %0;" : "=r"(len) : "r"(rbp - 52));
		// asm("movq (%1), %0;" : "=r"(addr_rbp) : "r"(rbp));
		
		print_recur(ret, prev_rbp, rbp);

		printf(" Stack reserved for local variables are from %li to %li \n", 
					(long) next_rbp, (long) rbp);
		// printf(" Absolute address in current stack is %p\n", addr_rbp);
		//find absolute address of variables.
		printf(" Value of arguments:\n \"val\" : %li address : %li \n \"depth\" : %i address : %li \n \"*end\" : %p address : %li", 
					val, (rbp - 8), depth, (rbp - 12), end, (rbp - 24));
		printf(" \n----------------------\n");
		printf(" Value for local variables are: \n \"quot\" : %li address : %li \n \"mod\" : %li address : %li\n \"*start\" : %p address : %li\n \"len\" : %i address : %li", 
					quot, (rbp - 32), mod, (rbp - 40), start, (rbp - 48), len, (rbp - 52));
		printf(" \n==================\n\n");

	}
}

/* Print stack frames of sequence of callers that have same return address. */
void print_stack() {
	int64_t rbp;
	int64_t prev_rbp;
	int64_t prev_ret;
	asm("movq %%rbp, %0;" : "=r"(rbp));
	asm("movq (%1), %0;" : "=r"(prev_rbp) : "r"(rbp));
	asm("movq 8(%1), %0;" : "=r"(prev_ret) : "r"(prev_rbp));
	// printf("____%li", prev_ret);
	print_recur(prev_ret, prev_rbp, rbp);
}

