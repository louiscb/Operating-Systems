//
// Created by Louis on 2018-11-21.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int)getpid());
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "FORK FAILED");
        exit(1);
    } else if (rc == 0) {
        printf("tjena, I am a child (pid:%d)\n", (int)getpid());
        return 20;
    } else {
        int res;
        int rc_wait = wait(&res);
        printf("hej hej, I am the parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait, (int)getpid());
        printf("The result is %d", WEXITSTATUS(res));
    }

    return 0;
}