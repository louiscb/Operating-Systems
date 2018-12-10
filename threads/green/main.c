#define _GNU_SOURCE

#include <stdio.h>
#include "green.h"
#include <pthread.h>

int a0 = 0;
int a1 = 1;
int a2 = 2;
int a3 = 3;
int TOTAL;

void *test(void *arg) {
    int i = *(int*)arg;
    int loop = TOTAL;

    while (loop > -1) {
        printf("Thread: %d - %*s %d\n", i, loop, " ", loop);
        loop--;
        green_yield();
    }
}

void *test2(void *arg) {
    int i = *(int*)arg;
    int loop = TOTAL;

    while (loop > -1) {
        printf("Thread: %d - %*s %d\n", i, loop, " ", loop);
        loop--;
        pthread_yield();
    }
}

int main() {
    printf(" - START -\n");

    greenTest();
    pthreadTest();

    printf("- END -\n");

    return 0;
}

void greenTest() {
    printf("-- Running Green Threading --\n");
    green_t g0, g1, g2;

    green_create(&g0, test, &a0);
    green_create(&g1, test, &a1);
    green_create(&g2, test, &a2);

    green_join(&g0);
    green_join(&g1);
    green_join(&g2);
    printf("-- Ending Green Threading --\n");
    return;
}

void pthreadTest() {
    printf("-- Running Pthread --\n")
    pthread_t p0, p1, p2, p3;

    pthread_create(&p0, NULL, test2, &a0);
    pthread_create(&p1, NULL, test2, &a1);
    pthread_create(&p2, NULL, test2, &a0);

    pthread_join(p0, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("-- Ending Pthread --\n");
    return;
}