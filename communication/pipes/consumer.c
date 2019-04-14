//
// Created by Louis on 2019-04-14.
//
#include <assert.h>
#include <zconf.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    int flag = O_RDONLY;
    int pipe = open("our_pipe", flag);

    char buffer[10];
    read(pipe, buffer, 10);

    printf("Output of the pipe: %s\n", buffer);

    printf("Done\n");
    return 0;
}