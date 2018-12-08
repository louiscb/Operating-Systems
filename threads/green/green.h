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
int green_create(green_t *thread, void *(*fun)(void*), void *arg);

//suspends current thread and selects new thread for execution
int green_yield();

//current thread is suspended waiting for a thread to terminate
int green_join(green_t *thread);

#endif //GREEN_GREEN_H
