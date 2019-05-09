//
// Created by Louis on 2019-04-13.
//

#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <assert.h>
#include <unistd.h>
#include <sys/un.h>

#define NAME "pingpong"

int main(int argc, char* argv[]) {
    // Here we define 1) domain 2) type of socket 3) protocol - 0 indicates IP
    int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    assert(server_socket != -1);

    struct sockaddr_un socket_address = {AF_UNIX, NAME};

    if (bind(server_socket, (struct sockaddr *) &socket_address, sizeof(socket_address)) == -1) {
        perror("Could not bind\n");
        exit(1);
    }

    int *test = malloc(sizeof(int));



    assert(listen(server_socket, 5) !=1);

    int socket_descriptor; // = accept(server_socket, NULL, NULL);

    assert((socket_descriptor = accept(server_socket, NULL, NULL)) != -1);

    char buffer[5];
    send(socket_descriptor, "hello", 4, 0);
    recv(socket_descriptor, buffer, 4, 0);
    printf("Received %s\n", buffer);

    return 0;
}