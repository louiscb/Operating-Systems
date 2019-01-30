#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

int main() {
    //making done a register will result in a infinite loop
    int done = 0;
    ucontext_t one;
    ucontext_t two;

    //saves current state of program in context one
    getcontext(&one);

    printf("Hello, World!\n");

    if (!done) {
        done = 1;
        //saves current context in two
        //executes from saved context one
        swapcontext(&two, &one);

        printf("We should not see this...");
    }

    return 0;
}