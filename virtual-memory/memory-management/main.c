#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "buddy.h"

int main(int argc, char *argv[]) {

    char command[50];
    sprintf(command, "cat /proc/%d/maps | grep heap", getpid());
    system(command);

    int *addr = balloc(5*100);

    for(int j = 0; j < 5; j++) {
        addr[j] = j+1;
        printf("%d: %p - %d\n", j, &addr[j], addr[j]);
    }

    system(command);

    printf("1");

    system(command);

    return 1;
}