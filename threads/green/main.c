#define _GNU_SOURCE

#include <stdio.h>
#include "green.h"
#include <pthread.h>

//Loop total
static int TOTAL = 1000;
static int NUM_OF_THREADS;
int flag = 0;

green_cond_t con;
green_mutex_t mutex;

//test concurrency
int counter = 0;

void *testYield(void *arg) {
    int i = *(int*)arg;
    int loop = TOTAL;

    while (loop > -1) {
        //printf("Thread: %d- %*s %d\n", i, loop, " ", loop);

        if (loop == 0)
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
           // printf("Thread: %d Flag: %d- %*s %d\n", i, flag, loop, " ", loop);
            loop--;
            flag = (i + 1)%NUM_OF_THREADS;
            green_cond_signal(&con);
        } else {
//            printf("WAIT\n");
            green_cond_wait(&con, NULL);
        }
    }
    printf("Thread: %d Flag: %d- %*s %d\n", i, flag, loop, " ", loop);
}

void *testTimer(void *arg) {
    unblockTimer();

    int i = *(int*)arg;

    //the total change to counter value is 0
    //without mutex this wont be so as interrupts
    //may happen in between the counter increase and decrease

    green_mutex_lock(&mutex);
    counter+=1;

    //spin to slow down function
    //interrupt probably occurs here
    for (int c = 1; c <= 3270; c++)
        for (int d = 1; d <= 3270; d++)
        {}

    counter-=1;
    green_mutex_unlock(&mutex);

    printf("Thread: %d Counter: %d\n", i, counter);
    blockTimer();
}

void *testTimerAtomic (void *arg) {
    int i = *(int*)arg;
    //printf(" thread here\n");
    green_mutex_lock(&mutex);
    while (1) {
        if (flag == 0) {
            flag = 1;
            green_cond_signal(&con);
            green_mutex_unlock(&mutex);
            printf("Thread: %d got flag\n", i);
            break;
        } else {
            printf("Thread: %d waiting \n", i);
            green_cond_wait(&con, &mutex);
        }
    }
}

void greenTest(int threads, void *fun) {
    TIMER_ON = TRUE;
    NUM_OF_THREADS = threads;
    green_mutex_init(&mutex);

    green_t *thread = malloc(sizeof(green_t)*NUM_OF_THREADS);
    int num[NUM_OF_THREADS];

    for (int i = 0; i < NUM_OF_THREADS; i++) {
        num[i] = i;

        int code = green_create(&thread[i], fun, &num[i]);

        if (code) {
            printf("ERROR CREATING THREADS\n");
            return;
        }
    }

    for (int i = 0; i < NUM_OF_THREADS; i++) {
        green_join(&thread[i]);
    }

    return;
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
    green_cond_init(&con);

    //provide the number of threads and function to test
    greenTest(4, &testTimer);

   //pthreadTest();

    printf("- END -\n");

    return 0;
}