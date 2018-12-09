#define _GNU_SOURCE

#include <stdio.h>
#include "green.h"
#include <pthread.h>

void *test(void *arg) {
    int i = *(int*)arg;
    int loop = 4;

    while (loop > -1) {
        printf("Thread: %d - %*s %d\n", i, loop, " ", loop);
        //printf("thread %d: %d%*s\n", i, loop, loop);
        loop--;
        green_yield();
    }
}

void *test2(void *arg) {
    int i = *(int*)arg;
    int loop = 4;

    while (loop > -1) {
        printf("Thread: %d - %*s %d\n", i, loop, " ", loop);
        //printf("thread %d: %d%*s\n", i, loop, loop);
        loop--;
        pthread_yield();
    }
}

int main() {
    printf(" - Testing -\n");
    green_t g0, g1, g2, g3;
    pthread_t p0, p1, p2, p3;
    int a0 = 0;
    int a1 = 1;
    int a2 = 2;
    int a3 = 3;

    green_create(&g0, test, &a0);
    green_create(&g1, test, &a1);
//    green_create(&g2, test, &a2);
//    green_create(&g3, test, &a3);

    //  printf(" - JOINING 1 - \n");
    green_join(&g0);
   // printf(" - JOINING 2 - \n");
    green_join(&g1);
   // green_join(&g2);

    printf("\nREAL SHIT\n");

    pthread_create(&p0, NULL, test2, &a0);
    pthread_create(&p1, NULL, test2, &a1);
   // pthread_create(&p2, NULL, test, &a2);
    //pthread_create(&p3, NULL, test, &a3);

    pthread_join(p0, NULL);

    printf("done\n");

    return 0;
}