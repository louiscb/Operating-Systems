//
// Created by nascosto on 2019-04-07.
//

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    printf("We caught this signal: %d\n", sig);
    exit(1);
    return;
}

int main() {
    struct sigaction sa;

    printf("Catching an error with signals!\n");

    // If you comment out setting the handler, the system will respond with a "core dumped" error instead of our one
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    //Here we register the handler under the SIGFPE singal
    sigaction(SIGFPE, &sa, NULL);

    int zero = 0;
    int spanner = 1 / zero;

    printf("You shouldn't see me");
    return 0;
}
