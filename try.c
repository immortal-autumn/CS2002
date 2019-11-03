#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <signal.h>

int main(void) {
    int i = 0;
    LABEL1:
    i++;
    printf("我是真的很不错\n");
    signal(SIGINT, SIG_IGN);
    goto LABEL1;
}