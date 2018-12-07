//
// Created by el-plate on 2018-12-07.
//
#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>

#define MAX 10

static int running;
static int done1;
static int done2;

static ucontext_t cntx_one;
static ucontext_t cntx_two;
static ucontext_t cntx_main;
static ucontext_t cntx_done;

void done() {
    int done = 0;

    while(!done) {
        if(running == 1) {
            printf(" - process one terminating -\n");
            done1 = 1;
            if(!done2) {
                running = 2;
                swapcontext(&cntx_done, &cntx_two);
            } else {
                done = 1;
            }
        } else {
            printf(" - process two terminating -\n");
            done2 = 1;
            if (!done1) {
                running = 1;
                swapcontext(&cntx_done, &cntx_one);
            } else {
                done = 1;
            }
        }
    }
    printf(" - done terminating -\n");
}

void yield() {
    printf("- yield  -\n");
    if (running == 1) {
        running = 2;
        swapcontext(&cntx_one, &cntx_two);
    } else {
        running = 1;
        swapcontext(&cntx_two, &cntx_one);
    }
}

void push(int p, int i) {
    if (i<MAX) {
        printf("%d%*s push\n", p, i, " ");
        //yield();
        push(p, i+1);
        printf("%d%*s pop\n", p, i, " ");
        //yield();
    } else {
        printf("%d%*s top\n", p, i, " ");
        yield();
    }
    //why doesnt this just infinitely loop?
}

int main() {
    char stack1[8*1024];
    char stack2[8*1024];
    char stackDone[8*1024];

    //The first context
    getcontext(&cntx_one);
    cntx_one.uc_stack.ss_sp = stack1;
    cntx_one.uc_stack.ss_size = sizeof stack1;
    //when context one terminates it switches to done context
    cntx_one.uc_link = &cntx_done;

    //create a context and tell it which function to address
    //the first number refers to number of arguments for chosen function - in this case 2
    //the proceeding arguments are the parameters to pass to push function
    makecontext(&cntx_one, (void (*) (void)) push, 2, 1, 1);

    //second context
    getcontext(&cntx_two);
    cntx_two.uc_stack.ss_sp = stack2;
    cntx_two.uc_stack.ss_size = sizeof stack2;
    cntx_two.uc_link = &cntx_done;
    //the second number is two as this is to tell the push function we are second context
    makecontext(&cntx_two, (void (*) (void)) push, 2, 2, 1);

    //setting up done context
    getcontext(&cntx_done);
    cntx_done.uc_stack.ss_sp = stackDone;
    cntx_done.uc_stack.ss_size = sizeof stackDone;
    makecontext(&cntx_done,(void (*) (void)) done, 0);

    running = 1;
    printf(" - BORJAN -\n");
    swapcontext(&cntx_main, &cntx_one);
    printf(" - SLUTET -\n");

    return 0;
}



