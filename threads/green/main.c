#define _GNU_SOURCE

#include <stdio.h>
#include "green.h"
#include <pthread.h>

static int a0 = 0;
static int a1 = 1;
static int a2 = 2;
static int a3 = 3;
static int TOTAL = 4;
static int NUM_OF_THREADS = 3;

int flag = 0;
green_cond_t con;

//test concurrency
int counter = 0;

void *testYield(void *arg) {
    int i = *(int*)arg;
    int loop = TOTAL;

    while (loop > -1) {
        printf("Thread: %d- %*s %d\n", i, loop, " ", loop);
        loop--;
        green_yield();
    }
}

void *testCondition(void *arg) {
    int i = *(int*)arg;
    int loop = TOTAL;

    while (loop > -1) {
        if (flag == i) {
            printf("Thread: %d Flag: %d- %*s %d\n", i, flag, loop, " ", loop);
            loop--;
            flag = (i + 1)%NUM_OF_THREADS;
            green_cond_signal(&con);
        } else {
            green_cond_wait(&con);
        }
    }
}

void *testTimer(void *arg) {
    unblockTimer();

    int i = *(int*)arg;
    int loop = TOTAL;

    //the total change to counter value is 0
    //without mutex this wont be so as interrupts
    //may happen in between the counter increase and decrease
    while (loop > -1) {
        //printf("Thread: %d %*s %d\n", i, loop, " ", loop);
        counter+=5;
        loop--;

        //spin to slow down function
        //interrupt probably occurs here
        for (int c = 1; c <= 3270; c++)
            for (int d = 1; d <= 3270; d++)
            {}

        counter-=5;
        printf("Thread: %d %d\n", i, counter);
    }

    blockTimer();
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

    //green_cond_init(&con);

    green_t g0, g1, g2;

    green_create(&g0, testTimer, &a0);
    green_create(&g1, testTimer, &a1);
    green_create(&g2, testTimer, &a2);

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