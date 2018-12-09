//
// Created by el-plate on 2018-12-07.
//

#include "green.h"
#include <stdlib.h>
#include <assert.h>
#include <ucontext.h>
#include <stdio.h>

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

void enqueue(green_t *thread);

green_t *dequeue();

void debugQueue();

//this function initializes the main context before compile time
void init() {
    readyQueue = malloc(sizeof(queue));
    getcontext(&main_cntx);
}

void green_thread() {
    green_t *this = running;

    //calls function of this with its arguments?
    (*this->fun)(this->arg);
    //printf("FINISHED function\n");
    //should we be adding to the second position in ready queue?
    if (this->join) {
        enqueue(this->join);
        this->join = NULL;
    }

    //free allocated stack
    free(this->context->uc_stack.ss_sp);
    free(this->context);

    this->zombie = TRUE;

    green_t *next = dequeue();

    //What happens if Next is null?
    if (next == NULL)
        printf("NEXT IS NULL\n");

    if (running == next) {
      //  next = dequeue();
    }

   // printf("RUNNING - %lx\n", running);
    running = next;
   // printf("SETTING CONTEXT\n");
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

    enqueue(thread);
    return 0;
}

void enqueue(green_t *thread) {
   // printf("EN-QUEUE\n");

    if (readyQueue->tail == NULL ) {
        readyQueue->head = readyQueue->tail = thread;
        return;
    }

    readyQueue->tail->next = thread;
    readyQueue->tail = thread;

    //debugQueue();
}

green_t *dequeue() {
    //printf("DE-QUEUE\n");
    if (readyQueue->head == NULL)
        return NULL;

    green_t *temp = readyQueue->head;
    readyQueue->head = readyQueue->head->next;

    if (temp->zombie == TRUE) {
        return dequeue();
    }

   // debugQueue();
    temp->next = NULL;
    return temp;
}

/*
 * Function should put the running thread last in the ready queue
 * then select first thread from the queue as next thread
 */
int green_yield() {
    green_t *susp = running;

    enqueue(susp);
    green_t *next = dequeue();

    running = next;

    //when the susp thread is scheduled again,
    // it will continue from exactly this point!
    swapcontext(susp->context, next->context);
    return 0;
}

/*
 * This is used so that the main thread doesn't return
 * before a secondary thread has finished processing.
 *
 */
int green_join(green_t *thread) {
    if (thread->zombie || thread == running)
        return 0;

    green_t *susp = running;
    thread->join = susp;

    running = thread;

    swapcontext(susp->context, thread->context);

    return 0;
}

void debugQueue() {
    green_t *temp = readyQueue->head;
    if (!temp)
        return;

    green_t use = *temp;
    printf("0 - %lx\n", temp);

    for (int i = 1; i < 10; ++i) {
        printf("%d - %lx \n",i, use.next);

        if (use.next == NULL)
            return;

        use = *use.next;
    }
}