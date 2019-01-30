#define _GNU_SOURCE

#include <stdio.h>
#include "green.h"
#include <pthread.h>
#include <time.h>

//Loop total
static int TOTAL = 10;
static int NUM_OF_THREADS;
int flag = 0;

green_cond_t con;
green_mutex_t mutex;
pthread_mutex_t pthreadMutex;
pthread_cond_t pCon;

//test concurrency
int counter = 0;

void *testYield(void *arg) {
    int i = *(int*)arg;
    int loop = TOTAL;

    while (loop > -1) {
        //printf("Thread: %d- %*s %d\n", i, loop, " ", loop);

        //if (loop == 0)
            //printf("Thread Complete: %d- %*s %d\n", i, loop, " ", loop);

        loop--;
        if (i == NUM_OF_THREADS-1 && loop == 0) {
            printf("PASS\n");
        } else {
            green_yield();
        }
    }
    printf("COmplete %d\n", i);
}

void *testCondition(void *arg) {
    int i = *(int*)arg;
    int loop = TOTAL;

    while (loop > -1) {
        if (counter == i) {
            printf("Thread: %d Flag: %d- %*s %d\n", i, flag, loop, " ", loop);
            loop--;

            if (counter<NUM_OF_THREADS-1) {
                counter++;
            } else {
                counter = 0;
            }

            flag = (i + 1)%NUM_OF_THREADS;
            green_cond_signal(&con);
        } else {
            green_cond_wait(&con, NULL);
        }
        // printf("Thread: %d Flag: %d- %*s %d\n", i, flag, loop, " ", loop);
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

    if (i == NUM_OF_THREADS-1 && counter == 0) {
        printf("PASSED\n");
    } else {
        green_mutex_unlock(&mutex);
        blockTimer();
    }


   // printf("Thread: %d Counter: %d\n", i, counter);


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
    //initialise stuff
    TIMER_ON = TRUE;
    NUM_OF_THREADS = threads;
    green_cond_init(&con);
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

void *testPYield(void *arg) {
    int i = *(int*)arg;
    int loop = TOTAL;

    while (loop > -1) {
        //printf("Thread: %d- %*s %d\n", i, loop, " ", loop);

        //if (loop == 0)
          //  printf("Thread: %d- %*s %d\n", i, loop, " ", loop);
        loop--;
        pthread_yield();
    }
}

void *testPCondition(void *arg) {
    int i = *(int*)arg;
    int loop = TOTAL;

    while (loop > -1) {
        if (flag == i) {
             printf("Thread: %d Flag: %d- %*s %d\n", i, flag, loop, " ", loop);
            loop--;
            flag = (i + 1)%NUM_OF_THREADS;
            pthread_cond_signal(&pCon);
            //green_cond_signal(&con);
        } else {
            printf("WAIT\n");
            pthread_cond_wait(&pCon, NULL);
           // green_cond_wait(&con, NULL);
        }
    }
    printf("Thread: %d Flag: %d- %*s %d\n", i, flag, loop, " ", loop);
}

void pthreadTest(int threads, void *fun) {
    NUM_OF_THREADS = threads;
    pthread_mutex_init(&pthreadMutex, NULL);
    pthread_cond_init(&pCon, NULL);

    pthread_t *thread = malloc(sizeof(pthread_t)*NUM_OF_THREADS);
    int num[NUM_OF_THREADS];

    for (int i = 0; i < NUM_OF_THREADS; i++) {
        num[i] = i;

        int code = pthread_create(&thread[i],NULL, fun, &num[i]);

        if (code) {
            printf("ERROR CREATING THREADS\n");
            return;
        }
    }

    for (int i = 0; i < NUM_OF_THREADS; i++) {
        pthread_join(thread[i], NULL);
    }

    return;
}

int main(int argc, char *argv[]) {
    printf(" - START -\n");

    int threads = atoi(argv[1]);

    clock_t start = clock();

   // greenTest(threads, &testTimer);
    pthreadTest(threads, &testPCondition);

    clock_t end = clock();

    printf("- END -\n");

    long int time_spent = (long int)(end - start) / CLOCKS_PER_SEC;

    printf("Took %lds\n", time_spent);
    return 0;
}