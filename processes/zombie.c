//
// Created by nascosto on 2019-04-06.
//

/*
 *
 * Zombie procss is a process that has been terminated but whose parent process has not yet been informed.
 *
 * The parent process picks up the exit status of the child process with the wait() call.
 *
 * If we delay the wait call in the parent process, and terminate the child in that time, we will create a zombie.
 *
 */

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
        sleep(20);
        int res;
        int rc_wait = wait(&res);
        printf("hej hej, I am the parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait, (int)getpid());
        printf("The result is %d", WEXITSTATUS(res));
    }

    return 0;
}