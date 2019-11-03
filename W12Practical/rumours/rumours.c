/**
 * @file rumours.c
 * @author 170025298
 * @brief 
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int CURRENT_DEPTH = 0;

void test_stp(unsigned input) {
   printf("------------%u----------------\n", input);
   exit(0);
}

char* link_args(int argc, char** arg) {
   int size = 0;
   for(int i = 0; i < argc - 1; i++) {
      size+= strlen(arg[i]) + 1;
   }
   size--;
   char* line = malloc(size);
   for(int i = 0; i < argc - 1; i++) {
      strcat(strcat(line, " "), arg[i]);
      // printf("%i : %s\n", i, line);
   }
   // test_stp(sizeof(line));
   return line;
}

char* switch_string(char* arg) {
   int size = strlen(arg);
   int i1 = 0, i2 = 0;
   srand(getpid());
   while(i1 == i2 || arg[i1] == ' ' || arg[i2] == ' ') {
      i1 = (rand() + 1) % size;
      i2 = (rand() - 1) % size;
   }
   char tmp = arg[i1];
   arg[i1] = arg[i2];
   arg[i2] = tmp;
   
   printf("pid: %u Swapped indices %i, %i\n", (unsigned)getpid(), i1, i2);
   return arg;
}
/*
   This function will do the following things:
   1. Create a new Pipe
   1. Create a new Child process
   2. Read String from parent process -> use pipe to read the line and passing to Child process
   3. Switch letters -> use switch function
   4. Writing String to pipe
*/
void create_process(char* arg) {
   //create a pipe
   int fd[2]; //This will pass the string to child process
   int fd2[2]; //This will pass the number of process
   if(pipe(fd) != 0 || pipe(fd2) != 0) {
      perror("Pipe creates failed!\n");
      exit(0);
   }

   //switch strings
   char* str;
   int cpid = -1;

   //create a new process
   if(CURRENT_DEPTH < 10) {
      str = switch_string(arg);
      CURRENT_DEPTH++; //current += 1
      cpid = fork();
      // printf("-----------------fork: %i, depth: %i----------------\n", cpid, CURRENT_DEPTH);
   }
   else {
      return;
   }

   //pipe
   if(cpid < 0) { //handle error
      perror("Process create failed!\n");
      exit(0);
   }
   if(cpid != 0) { //parent
      close(fd[0]); //close reading of first pipe
      close(fd2[0]); //close reading of second pipe
      write(fd2[1], &CURRENT_DEPTH, sizeof(int)); //insert currentdepth to pipe
      write(fd[1], &str, sizeof(str) + 1); //insert string to pipe
      close(fd[1]); //close writing
      close(fd2[1]); //close writing
      free(str); //free the string
      wait(NULL); //wait until the child finished in case of keep the parent pid not equal to 1
   } 
   else { //child
      printf("New process: %u, parent: %u\n", (unsigned)getpid(), (unsigned)getppid());
      close(fd[1]); //close writing of first pipe
      close(fd2[1]); //close writing of second pipe
      char* buffer = malloc(sizeof(str)); //create a string with specific size
      read(fd[0], &buffer, sizeof(buffer)); //read string using first pipe
      read(fd2[1], &CURRENT_DEPTH, sizeof(int)); //read current depth using second pipe
      close(fd[0]); //close reading of first pipe
      close(fd2[0]); //close reading of second pipe
      printf("pid: %u received string: %s\n", (unsigned)getpid(), arg);
      create_process(buffer); //create next process
   }
}

int main(int argc, char **argv)
{
   if(argc <= 1) {
      printf("Invalid argument: Usage: %s LINES\n", argv[0]);
      exit(EXIT_FAILURE);
   }
   char* lines[argc - 1];
   for(int i = 1; i < argc; i++) {
      lines[i-1] = argv[i];
   }
   char* input = link_args(argc, lines);
   // test_stp(input);
   create_process(input);
}