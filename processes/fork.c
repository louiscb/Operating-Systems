//
// Created by Louis on 2018-11-21.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int)getpid());
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "FORK FAILED");
        exit(1);
    } else if (rc == 0) {
        printf("tjena, I am a child (pid:%d)\n", (int)getpid());
    } else {
        printf("hej hej, I am the parent of %d (pid:%d)\n", rc, (int)getpid());
    }
    return 0;
}