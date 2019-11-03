#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

#include "shadow_breaker.h"


#define DEFAULT_THREADS 4

void usage() 
{
  fprintf(stderr, "\n  ./shadow_pool [thread_count] < <password_file>\n\n");
  exit(1);
}

int main(int argc, char **argv) 
{
  size_t thread_count = DEFAULT_THREADS;

  if (argc > 2)
    usage();

  if (argc > 1) {
    if (1 != sscanf(argv[1], "%lu", &thread_count) || thread_count < 1)
      usage();
  }

  double start_time = getTime();
  double start_cpu_time = getCPUTime();

  // Your code called here
  int ret = start(thread_count);

  // Keep this timing code in the provided main.
  double elapsed = getTime() - start_time;
  double total_cpu_time = getCPUTime() - start_cpu_time;

  printf("Total time: %.2f seconds.\n", elapsed);
  printf("Total CPU time: %.2f seconds.\n", total_cpu_time);
  printf("CPU usage: %.2fx\n", total_cpu_time / elapsed);

  return ret;
}
