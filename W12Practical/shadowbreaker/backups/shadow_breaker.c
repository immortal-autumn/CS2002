#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#include <crypt.h>
#include "shadow_breaker.h"

#include "libs/utils.h"
#include "libs/output_helpers.h"
#include "libs/queue_r.h"

//since one character is one byte, length of username and length of password has 16 bytes in total.
//There may exists password and Iterative number, which has 24 bytes in total. 128 bytes is enough to store all informations
// #define MAX_SIZE (size_t)128 
#define MAX_LENGTH 24

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 

/******* OPTIONAL TEMPLATE CODE STARTS HERE -- MODIFY OR DELETE *******/

/**
 * You may wish to define a struct for your task here.
 *  Remember that enqueue/dequeue funcs take and return (void *)!
 * 
 * [Advanced] Separately, you may wish to define a de-structor function for
 *  use by queue_destroy() in "queue.h" -- see that header for details.
 */

static char* input[3]; //global variables: 0: username 1: hex 2: prefix
static int amount = 0;
static int unknown = 0;
static int GLOB_THREAD_CONT;
static int thread_found = 0;


typedef struct {
  char a[22];
  char *b;
} myX;

void myX_destroy(void *data)
{
  myX *x = (myX *) data;
  // We don't free(x->a) since a[22] was never mallec'ed.
  free(x->b);
  free(x);
}

void free_char() {
  for(int i = 0; i < 3; i++) {
    free(input[i]);
  }
}


/******* OPTIONAL POSSIBLE TEMPLATE CODE __ENDS__ HERE *******/

/**
 * REQUIRED FUNCTION - This is the start of your threads.
 */
void * thread_start_routine(void* param) 
{
  char str[MAX_LENGTH];
  strcpy(str, input[2]); //copy a back_up to prevent starving and keep thread safe
  long start_index, count; //start index is the first possi. pwd, count is the total number of pwd
  getSubrange(unknown, GLOB_THREAD_CONT, (long)param, &start_index, &count);
  // printf("start index: %li , count: %li , opy_str: %s\n", start_index, count, str);
  char attempt[unknown];
  for(int i = 0; i < unknown; i++) {
    attempt[i] = 'a';
  } //initialise all characters as a
  setStringPosition(attempt, start_index); //tested. correct
  // printf("current attempt: %s, current count: %li\n", attempt, start_index);

  sleep(1);

  //initialise encryption
  struct crypt_data cdata;
  cdata.initialized = 0;
  const char *hash;

  for(int i = 0; i < count; i++) {
    if(thread_found == 1) {
      pthread_exit((void*)-1);
    }
    for(int j = 0; j < unknown; j++) {
      str[amount + j] = attempt[j];
    }
    hash = crypt_r(str,"cs2002",&cdata);
    // printf("hash: %s, string: %s, cmp_hash: %s\n", hash, str, input[1]);
    if(hash != NULL) {
      if(strcmp(hash, input[1]) == 0) {
        fprintf(stderr, "Result found: %s\n", str);
        thread_found = 1;
        pthread_exit((void*)1);      }
    }
    if(incrementString(attempt) == 0) {
      pthread_exit((void*)-1);
    }
  }

  return (void*)-1;
}

/**
 * REQUIRED FUNCTION - Called by main to launch and manage threads.
 */
int start(size_t thread_count) {
  // Remember, this is only the thread launching point, i.e. this function:
  //   - calls pthread_create(..., ..., &thread_start_routine, ...) 
  //   - reads tasks (one per line) from stdin, etc.
  //   - example queue code is 'just in case' anyone wishes to handle
  //     multiple passwords.
  GLOB_THREAD_CONT = thread_count; //assign parameters to global variable

  for(int i = 0; i < 3; i++) {
    input[i] = (char*)malloc(MAX_LENGTH);
    if(scanf("%s", input[i]) != 1) {
      printf("Invalid syntax! Case error!\n");
      exit(0);
    }
  }
  print_parr_start_user(input[0]); //print Start <username>
  
  
  amount = getPrefixLength(input[2]); //This will return the position of the last letter not '.'
  unknown = strlen(input[2]) - amount; //This will set the global value that get the number of unknown values
  if (unknown == 0) {
    printf("This is not a valid prefix!\n");
    exit(0);
  }
  long thread_id = 1;
  pthread_t thread[thread_count];
  while(thread_id < (long)thread_count + 1) { //create 4 times (correct).
    pthread_create(&thread[thread_id - 1], NULL, &thread_start_routine, (void*)thread_id); //the first thread id is 1, so create in array thread_id - 1;
    thread_id++;
  }

  int output;
  for(int i = 0; i < thread_id - 1; i++) {  //Since i start from 0 but thread id start from 1
    pthread_join(thread[i], (void*)&output);
  }
  // printf("Result here! %i\n", output);



  // All code below is for demonstrative use of queue only.
  queue *q = queue_create(-1);

  myX *data;

  for(int i = 0; i < 1000; ++i) {
    data = malloc(sizeof(myX));
    
    // Contents may or may not need to be handled in myX_destroy()
    (void)strncpy(data->a, "Hello ", sizeof(data->a));
    data->b = strdup("World! "); // strdup mallocs
    // fprintf(stderr, "%d ", i);

    enqueue(q, (void *)data);
  }

  data = NULL;

  for(int i = 500; i > 0; --i) {
    data = (myX *) dequeue(q);
    if(!data) perror("__Impossible__ since dequeue() blocks when empty!\n");
    fprintf(stderr, "%d - %s%s\n", i, data->a, data->b);
  }

  //Here, free the malloc of String:
  free_char();
  // Here, still 500 nodes remain in the queue.
  //  myX_destroy will be called to free individual nodes in the queue.
  queue_destroy(q, &myX_destroy);
  q = NULL;

  return 0; // 0 indicates success
}

