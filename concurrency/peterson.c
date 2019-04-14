//
// Created by nascosto on 2019-04-14.
//

/**
 * This program implements peterson algorithm which allows multiple executions to access the same resource
 *
 * Run the program with `50000 on` compared to `50000 off` and see which version is more accurate
 *
 * We still have an margin of error with our locked program, this is because the steps that occur in "lock"
 * and "unlock" are not atomic
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/wait.h>
#include <zconf.h>

int count = 0;
int request[2] = {0, 0};
int turn = 0;
int locked = 1;

typedef struct args {
    int inc;
    int id;
} args;

void lock(int id) {
    request[id] = 1;

    int other_thread = 1-id;
    turn = other_thread;

    while (request[other_thread] == 1 && turn == other_thread) {}
}

void unlock(int id) {
    request[id] = 0;
}

void *increment(void *arg) {
    int inc = ((args*)arg)->inc;
    int id = ((args*)arg)->id;

    for (int i = 0; i < inc; i++) {
        if (locked == 1) {
            lock(id);
            count++;
            unlock(id);
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

    //set the id of each thread so we can differentiate them
    first_args.id = 0;
    second_args.id = 1;

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
