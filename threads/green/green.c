//
// Created by el-plate on 2018-12-07.
//

#include "green.h"
#include "queue.h"

static ucontext_t main_cntx = {0};
static green_t main_green = {&main_cntx, NULL, NULL, NULL, NULL, FALSE};
static green_t *running = &main_green;

queue *readyQueue;

//What do all of these parameteres mean?
static void init() __attribute__((constructor));

//this function initializes the main context before compile time
void init() {
    readyQueue = malloc(sizeof(queue));
    getcontext(&main_cntx);
}

void green_thread() {
    green_t *this = running;

    //calls function with arguments
    (*this->fun)(this->arg);

    if (this->join) {
        enqueue(readyQueue, this->join);
        this->join = NULL;
    }

    //free allocated stack
    free(this->context->uc_stack.ss_sp);
    free(this->context);

    this->zombie = TRUE;

    green_t *next = dequeue(readyQueue);

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
    thread->join = susp;

    running = dequeue(readyQueue);

    swapcontext(susp->context, thread->context);

    return 0;
}

void green_cond_init(green_cond_t *con) {
    con->queue1 = malloc(sizeof(queue));
}

void green_cond_wait(green_cond_t *con) {
    green_t *susp = running;
    enqueue(con->queue1, susp);

    running = dequeue(readyQueue);

    swapcontext(susp->context, running->context);
}

void green_cond_signal(green_cond_t *con) {
    green_t *susp = dequeue(con->queue1);
    enqueue(readyQueue, susp);
}
