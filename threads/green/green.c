//
// Created by el-plate on 2018-12-07.
//

#include "green.h"
#include "queue.h"

#include <signal.h>
#include <sys/time.h>
#include <time.h>

#define PERIOD 10
#define TIMER_ON TRUE

static sigset_t block;

static ucontext_t main_cntx = {0};
static green_t main_green = {&main_cntx, NULL, NULL, NULL, NULL, FALSE};
static green_t *running = &main_green;

queue *readyQueue;

static sigset_t block;

void timer_handler(int);

static void init() __attribute__((constructor));

//this function initializes the main context before compile time
void init() {
    //init our ready queue
    readyQueue = malloc(sizeof(queue));
    readyQueue->id = 1;

    if (TIMER_ON) {
        //init our timer
        sigemptyset(&block);
        sigaddset(&block, SIGVTALRM);

        struct sigaction act = {0};
        struct timeval interval;
        struct itimerval period;

        act.sa_handler = timer_handler;
        assert(sigaction(SIGVTALRM, &act, NULL) == 0);

        interval.tv_sec = 0;
        interval.tv_usec = PERIOD;
        period.it_interval = interval;
        period.it_value = interval;
        setitimer(ITIMER_VIRTUAL, &period, NULL);

        //start with timer blocked so that our thread handling doesnt get interrupted
        sigprocmask(SIG_BLOCK, &block, NULL);
    }

    //start main thread
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

    //make join loop
    thread->join = susp;

    running = dequeue(readyQueue);

    swapcontext(susp->context, thread->context);

    return 0;
}

void green_cond_init(green_cond_t *con) {
    con->suspendedQueue = malloc(sizeof(queue));
    con->suspendedQueue->id = 2;
}

void green_cond_wait(green_cond_t *con) {
    green_t *susp = running;

    enqueue(con->suspendedQueue, susp);

    running = dequeue(readyQueue);
    swapcontext(susp->context, running->context);
}

void green_cond_signal(green_cond_t *con) {
    //when this is first called the suspeneded queue is null
    if (con->suspendedQueue->head == NULL)
        return;

    green_t *unsuspend = dequeue(con->suspendedQueue);
    enqueue(readyQueue, unsuspend);
}

void timer_handler(int sig) {
    //printf("TIME INTERRUPT\n");
    green_t *susp = running;

    enqueue(readyQueue, susp);
    running = dequeue(readyQueue);

    swapcontext(susp->context, running->context);
}

void unblockTimer() {
    sigprocmask(SIG_UNBLOCK, &block, NULL);
}

void blockTimer() {
    sigprocmask(SIG_BLOCK, &block, NULL);
}

int green_mutex_init(green_mutex_t *mutex) {
    mutex->taken = FALSE;
    mutex->susp = NULL;
    return 0;
}

int green_mutex_lock(green_mutex_t *mutex) {
    blockTimer();

    green_t *susp = running;

    while (mutex->taken) {
        if (mutex->susp == NULL) {
            mutex->susp = susp;
        } else {
            mutex->susp->next = susp;
        }

        running = dequeue(readyQueue);
        swapcontext(susp->context, running->context);
    }

    mutex->taken = TRUE;

    unblockTimer();

    return 0;
}

int green_mutex_unlock(green_mutex_t *mutex) {
    blockTimer();

    enqueue(readyQueue, mutex->susp);

    mutex->taken = FALSE;

    unblockTimer();

    return 0;
}