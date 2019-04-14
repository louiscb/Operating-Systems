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
    int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    mkfifo("our_pipe", (mode_t) mode);

    int flag = O_WRONLY;
    int pipe = open("our_pipe", flag);

    write(pipe, "abcd", 10);

    printf("Done\n");

    return 0;
}