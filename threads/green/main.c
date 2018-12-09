#include <stdio.h>
#include "green.h"

void *test(void *arg) {
    int i = *(int*)arg;
    int loop = 10;

    while (loop > -1) {
        printf("Thread: %d - %*s %d\n", i, loop, " ", loop);
        //printf("thread %d: %d%*s\n", i, loop, loop);
        loop--;
        green_yield();
    }
}

int main() {
    printf(" - Testing -\n");
    green_t g0, g1, g2;
    int a0 = 0;
    int a1 = 1;
    int a2 = 2;

    green_create(&g0, test, &a0);
    green_create(&g1, test, &a1);
    green_create(&g2, test, &a2);

  //  printf(" - JOINING 1 - \n");
    green_join(&g0);
   // printf(" - JOINING 2 - \n");
    green_join(&g1);
    green_join(&g2);

    printf("done\n");

    return 0;
}