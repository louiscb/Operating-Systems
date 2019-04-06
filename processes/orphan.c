//
// Created by nascosto on 2019-04-06.
//

/*
 *
 * An orphan is a process whose parent process terminates before it does.
 *
 * Notice in this program how the parent process of the child change when the parent terminates due to a runtime error.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pid = fork();

    if(pid==0) {
        int child = getpid();
        printf("child: parent %d, group %d\n", getppid(),getpgid(child));
        sleep(4);
        printf("child: parent %d, group %d\n", getppid(),getpgid(child));
        sleep(4);
        printf("child: parent %d, group %d\n", getppid(),getpgid(child));
    } else {
        int parent = getpid();
        printf("parent: parent %d, group %d\n", getppid(),getpgid(parent));
        sleep(2);
        int zero = 0;
        int i = 3 / zero;
    }
    return 0;
}
