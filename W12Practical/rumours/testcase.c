#include <stdlib.h>
#include <stdio.h>
// #include <string.h>
#include <pthread.h>
// #include <math.h>

#include <unistd.h>
#include <sys/types.h>
// #include <sys/wait.h>

int cont = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 

void * worker(void *data){
    sleep(1);
    pthread_mutex_lock(&lock);
    printf("data: %lu-tid: %lu-cont : %i\n", (pthread_t)data, pthread_self(),cont);
    cont++;
    pthread_mutex_unlock(&lock);
    if(data) {
        pthread_join((pthread_t)data, NULL);
    }
    pthread_exit((void*)cont); 
}

int main(){
    pthread_t thread = (pthread_t)NULL;
    void *output;
    for(int i = 0; i < 5; ++i){
        pthread_create(&thread, NULL, worker, (void *)thread);
        printf("i : %i, addr : %lu-----------\n", i, thread);
    }
    printf("---------------\nfirst addr : %lu\n-----------\n", thread);
    pthread_join(thread, &output);
    printf("\noutput : %i\n", (int)output);
}