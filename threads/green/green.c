//
// Created by el-plate on 2018-12-07.
//

#include "green.h"
#include <stdlib.h>
#include <assert.h>
#include <ucontext.h>

#define FALSE 0
#define TRUE 1

#define STACK_SIZE 4096

static ucontext_t main_cntx = {0};
static green_t main_green = {&main_cntx, NULL, NULL, NULL, NULL, FALSE};
static green_t *running = &main_green;

struct green_t *readyList

//What do all of these parameteres mean?
static void init() __attribute__((constructor));

//this function initializes the main context before compile time
void init() {
    getcontext(&main_cntx);
}

void green_thread() {
    green_t *this = running;
    (*this->fun)(this->arg);

    //do some shit

    running = next;
    setcontext(next);
}

int green_create(green_t *thread, void *(*fun)(void*), void *arg) {
    ucontext_t *cntx = (ucontext_t *)malloc(sizeof(ucontext_t));
    getcontext(cntx);

    void *stack = malloc(STACK_SIZE);

    cntx->uc_stack.ss_sp = stack;
    cntx->uc_stack.ss_size = STACK_SIZE;

    makecontext(cntx, green_thread, 0);
    thread->context = cntx;
    thread->fun = fun;
    thread->arg = arg;
    thread->next = NULL;
    thread->join = NULL;
    thread->zombie = FALSE;

    //add thread to ready queue
    return 0;
}

int green_yield() {
    green_t * susp = running;

    //do some shit

    running = next;
    swapcontext(susp->context, next->context);

    return 0;
}

int green_join(green_t *thread) {
    if (thread->zombie)
        return 0;

    green_t *susp = running;

    //do some shit

    running = next;

    swapcontext(susp->context, next->context);

    return 0;
}