//
// Created by Louis on 2018-11-21.
//

/*
 * Child pipes the output of the wc execution into a new file
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main (int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int)getpid());
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "FORK FAILED");
        exit(1);
    } else if (rc == 0) {
        close(STDOUT_FILENO);

        //Where do we specify execvp to pipe into open.output?
        open("./open.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);

        //executing p4
        char *myargs[3];
        myargs[0] = strdup("wc");
        myargs[1] = strdup("open.c");
        myargs[2] = NULL;
        execvp(myargs[0], myargs);
    } else {
        printf("hej hej, I am the parent of %d (pid:%d)\n", rc, (int)getpid());
        int rc_wait = wait(NULL);
    }
    return 0;
}