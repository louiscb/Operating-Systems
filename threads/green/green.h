//
// Created by el-plate on 2018-12-07.
//

#ifndef GREEN_GREEN_H

#define GREEN_GREEN_H

#include <ucontext.h>

typedef struct green_t {
    //context of our thread
    ucontext_t *context;
    //function of our context
    void *(*fun)(void*);
    //args for function
    void *arg;
    //used for when thread is added to linked list of suspended threads
    struct green_t *next;
    //other thread that is waiting to join our thread
    struct green_t *join;
    //indicates if thread has terminated or not
    int zombie;
} green_t;

//initialise a green thread
//our version of pthread_create()
int green_create(green_t *thread, void *(*fun)(void*), void *arg);

//suspends current thread and selects new thread for execution
//our version of pthread_yield()
int green_yield();

/*
 * Current thread is suspended waiting for a thread to terminate
 * our version of pthread_join()
 * This is used so that the main thread doesn't return
 * before a secondary thread has finished processing.
 */
int green_join(green_t *thread);

#endif //GREEN_GREEN_H
