//
// Created by Louis on 2018-11-21.
//

/*
* Child process Runs word count on a file
*
* Exec does not create a new process it transforms the currently running program
* into a different program. After the exec in the child, it is as if p3.c never ran,
* a successful call to exec never returns
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int)getpid());
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "FORK FAILED");
        exit(1);
    } else if (rc == 0) {
        printf("tjena, I am a child (pid:%d)\n", (int)getpid());
        char *myargs[3];
        myargs[0] = strdup("wc");
        myargs[1] = strdup("exec.c");
        myargs[2] = NULL;

        execvp(myargs[0], myargs);
    } else {
        printf("hej hej, I am the parent of %d (pid:%d)\n", rc, (int)getpid());
    }
    return 0;
}