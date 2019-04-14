//
// Created by Louis on 2019-04-14.
//

/**
 *
 * What will be printed in what order?
 *
 */

#include <pthread.h>
#include <stdio.h>

pthread_t basic_thread;

char *iamglobal = "after you who's last it's DOOM he's the worst known";

void basic_function(char *string) {
    int local = 0;

    if (pthread_equal(basic_thread, pthread_self())) {
        printf("Second thread Parameter: %s\n", string);
        printf("Second thread Global: %s\n", iamglobal);
        local+=50;
        printf("Second thread Local variable: %d\n", local);
    } else {
        printf("Main thread Parameter: %s\n", string);
        printf("Main thread Global: %s\n", iamglobal);
        local+=100;
        printf("Main thread Local variable: %d\n", local);
        iamglobal = "rock it to a worst clone just don't curse the throne";
    }

    return;
}

int main(int argc, char *argv[]) {
    pthread_create(&basic_thread, NULL, basic_function, "let he who is without sin cast the first stone");

    basic_function("own his own microphone bring it everywhere he go");

    pthread_exit(NULL);
}