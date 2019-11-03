/**
 * Header to link and launch YOUR start function from _main.c
 */

#include <stddef.h>
#define MAX_LENGTH 24


// Function called from shadow_breaker main()
void free_char();
void * thread_start_routine(void* param);

int start(size_t thread_count);
