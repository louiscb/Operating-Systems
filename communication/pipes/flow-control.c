//
// Created by Louis on 2019-04-14.
//

#include <assert.h>
#include <zconf.h>
#include <stdio.h>
#include <sys/wait.h>

#define ITERATIONS 10
#define MESSAGE_SIZE 10

void consumer(int read_pipe) {
    for (int i = 0; i < ITERATIONS; i++) {
        char buffer[MESSAGE_SIZE];
        buffer[MESSAGE_SIZE-1] = 0;
        read(read_pipe, &buffer, MESSAGE_SIZE);
        printf("Received message: %s\n", buffer);
        // we sleep here to simulate the consumer processing the data
        sleep(1);
    }
    printf("Consumer finished...\n");
}

void producer(int write_pipe) {
    int message_count = 0;
    for (int i = 0; i < ITERATIONS; i++) {
        char message[MESSAGE_SIZE];
        message_count++;
        sprintf(message, "%d", message_count);
        write(write_pipe, message, MESSAGE_SIZE);
        printf("Sending message: %s\n", message);
    }
    printf("Producer finished...\n");
}

int main() {
    int pipe_file_descriptor[2];

    // the pipe function returns two file descriptors
    // pipe_file_descriptor[0] will be the fd for the read end of the pipe
    // pipe_file_descriptor[1] will be the fd for the write end of the pipe
    assert(pipe(pipe_file_descriptor) == 0);

    if (fork() == 0) {
        consumer(pipe_file_descriptor[0]);
    } else {
        producer(pipe_file_descriptor[1]);
        wait(NULL);
        printf("Completed...\n");
    }


    return 0;
}