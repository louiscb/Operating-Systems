//
// Created by nascosto on 2019-04-07.
//

/*
 *
 * Program will not immediatley terminate when the ctrl-c interrupt is sent from the terminal shell.
 *
 * Sending SIGKILL will terminate the process immediately.
 *
 * Sending a kill -2 from a different terminal window will show the PID of the process trying to kill this program
 *
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int volatile count;

void handler(int sig, siginfo_t *siginfo, void *context) {
    printf("\nsignal %d look alive look alive\n", sig);
    printf("Process %d tried to kill me!\n", siginfo->si_uid);
    count++;
}

int main() {
    struct sigaction sa;

    int pid = getpid();

    printf("Do your worst, my PID is %d\n", pid);

    //we tell the signal action structure to forward signals to this process to our custom handler
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);

    if(sigaction(SIGINT, &sa, NULL) != 0) {
        return 1;
    }

    while(count < 5) {
    }

    printf("yOu WiN\n");
    return 0;
}
