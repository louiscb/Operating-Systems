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

typedef struct queue {
    struct green_t *head;
    struct green_t *tail;
} queue;

queue *readyQueue;

//What do all of these parameteres mean?
static void init() __attribute__((constructor));

void enqueue(queue *queue1, green_t *thread);

green_t *dequeue(queue *queue1);

//this function initializes the main context before compile time
void init() {
    getcontext(&main_cntx);
}

void green_thread() {
    green_t *this = running;

    //calls function of this with its arguments?
    (*this->fun)(this->arg);

    //should we be adding to the second position in ready queue?
    if (this->join)
        enqueue(readyQueue, this->join);

    //free allocated stack
    free(this->context->uc_stack.ss_sp);
    free(this->context);

    this->zombie = TRUE;

    green_t *next = dequeue(readyQueue);

    //What happens if Next is null?
    // if (next == NULL)
    //    return;

    running = next;
    setcontext(next->context);
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

    enqueue(readyQueue, thread);

    return 0;
}

void enqueue(queue *queue1, green_t *thread) {
    if (queue1->head == NULL) {
        queue1->head = thread;
        queue1->tail = thread;
        return;
    }

    queue1->tail->next = thread;
    queue1->tail = thread;
}

green_t *dequeue(queue *queue1) {
    green_t *temp = readyQueue->head;
    readyQueue->head = readyQueue->head->next;
    return temp;
}

/*
 * Function should put the running thread last in the ready queue
 * then select first thread from the queue as next thread
 */
int green_yield() {
    green_t *susp = running;

    enqueue(readyQueue, susp);
    green_t *next = dequeue(readyQueue);

    running = next;

    //when the susp thread is scheduled again,
    // it will continue from exactly this point!
    swapcontext(susp->context, next->context);

    return 0;
}

int green_join(green_t *thread) {
    if (thread->zombie)
        return 0;

    green_t *susp = running;


   // Does susp go to thread's join?
   // Or thread to susp's join?????

    green_t *next = susp->join;

    

    // Does

//    while(next->join) {
//        next = next->join;
//    }
//
//    next->join = thread;

    running = next;

    swapcontext(susp->context, next->context);

    return 0;
}