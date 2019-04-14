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
    int client_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    assert(client_socket != -1);

    struct sockaddr_un socket_address = {AF_UNIX, (sa_family_t) NAME};

    assert(connect(client_socket, (const struct sockaddr *) &socket_address, sizeof(socket_address)) != -1);

    char buffer[5];
    send(client_socket, "bye", 4, 0);
    recv(client_socket, buffer, 4, 0);
    printf("Received %s\n", buffer);

    return 0;
}