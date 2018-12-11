#define _GNU_SOURCE

#include <stdio.h>
#include "green.h"
#include <pthread.h>

static int a0 = 0;
static int a1 = 1;
static int a2 = 2;
static int a3 = 3;
static int TOTAL = 4;
int flag = 0;
green_cond_t con;

void *test(void *arg) {
    int i = *(int*)arg;
    int loop = TOTAL;

    while (loop > -1) {
        printf("Thread: %d- %*s %d\n", i, loop, " ", loop);
        //delay(2000);
        loop--;
       // green_yield();
    }
}

void *test2(void *arg) {
    int i = *(int*)arg;
    int loop = TOTAL;

    while (loop > -1) {
        if (flag == i) {
            printf("Thread: %d Flag: %d- %*s %d\n", i, flag, loop, " ", loop);
            loop--;
            flag = (i + 1)%3;
           // green_cond_signal(&con);
        } else {
           // printf("wait\n");
           // green_cond_wait(&con);
        }
    }
}

void *testPthread(void *arg) {
    int i = *(int*)arg;
    int loop = TOTAL;

    while (loop > -1) {
        printf("Thread: %d - %*s %d\n", i, loop, " ", loop);
        loop--;
       // pthread_yield();
    }
}

void greenTest() {
    printf("-- Running Green Threading --\n");
    green_cond_init(&con);
    green_t g0, g1, g2;

    green_create(&g0, test2, &a0);
    green_create(&g1, test2, &a1);
    green_create(&g2, test2, &a2);

    green_join(&g0);
    green_join(&g1);
    green_join(&g2);

    printf("-- Ending Green Threading --\n");
    return;
}

void pthreadTest() {
//    printf("-- Running Pthread --\n");
//    pthread_t p0, p1, p2, p3;
//
//    pthread_create(&p0, NULL, testPthread, &a0);
//    pthread_create(&p1, NULL, testPthread, &a1);
//    pthread_create(&p2, NULL, testPthread, &a2);
//
//    pthread_join(p0, NULL);
//    pthread_join(p1, NULL);
//    pthread_join(p2, NULL);
//    printf("-- Ending Pthread --\n");
//    return;
}

int main() {
    printf(" - START -\n");

    greenTest();
//    pthreadTest();

    printf("- END -\n");

    return 0;
}