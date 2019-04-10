//
// Created by Louis on 2018-11-21.
//

/*
 *
 * Two processes that fork share a file descriptor table therefore will write to the same file.
 *
 * What happens if we open the file after forking as opposed to before forking?
 *
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

    FILE *fp;
    fp = fopen("./file.output", "w+");

    int rc = fork();

    //FILE *fp;
    //fp = fopen("./file.output", "w+");

    if (rc < 0) {
        fprintf(stderr, "FORK FAILED");
        exit(1);
    } else if (rc == 0) {
        fprintf(fp, "Suzy screw had a partner named sasha\n");
    } else {
        fprintf(fp, "I remember her number like the summer\n");
    }

    fclose(fp);
    return 0;
}