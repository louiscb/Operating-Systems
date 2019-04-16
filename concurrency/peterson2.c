//
// Created by nascosto on 2019-04-14.
//

/**
 * This is the updated version of the peterson algorithm implementation where
 * we now use the atomic operation "sync_val_compare_and_swap" instead of them being several
 * different commands
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/wait.h>
#include <zconf.h>

int count = 0;
int locked = 1;

int mutual_lock = 0;


typedef struct args {
    int inc;
    int *mutex;
} args;

int try(int *mutex) {
    // if the value of mutex is 0, turn it into 1
    // we return the value of mutex
    return __sync_val_compare_and_swap(mutex, 0 ,1);
}

int lock(int *mutex) {
    //try returns the value of the mutex, we will loop here until
    //the mutex return a 0 meaning it is unlocked
    int spin_count = 0;

    while(try(mutex) != 0) {
        spin_count+=1;
        sched_yield();
    }

    return spin_count;
}

void unlock(int *mutex) {
    *mutex = 0;
}

void *increment(void *arg) {
    int inc = ((args*)arg)->inc;
    int *mutex = ((args*)arg)->mutex;

    for (int i = 0; i < inc; i++) {
        if (locked == 1) {
            int spin_count = lock(mutex);
            //printf("Spin count: %d\n", spin_count);
            count++;
            unlock(mutex);
        } else {
            count++;
        }
    }

    return NULL;
}

int main(int argc, char * argv[]) {
    if (argc < 2){
        printf("Must supply parameters\n");
        exit(0);
    }

    int inc = atoi(argv[1]);
    char *locked_string = argv[2];

    if (!strcmp(locked_string, "off")) {
        locked = 0;
    }

    pthread_t first_thread, second_thread;
    args second_args;
    args first_args;

    first_args.inc = inc;
    second_args.inc = inc;

    first_args.mutex = &mutual_lock;
    second_args.mutex = &mutual_lock;

    for (int i = 0; i < 10; ++i) {
        pthread_create(&first_thread, NULL, increment, &first_args);
        pthread_create(&second_thread, NULL, increment, &second_args);

        //this tells the main thread to wait to on the newly spawned threads before returning
        pthread_join(first_thread, NULL);
        pthread_join(second_thread, NULL);

        printf("Result: %d\n", count);
        count = 0;
    }

    return 0;
}
