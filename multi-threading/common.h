//
// Created by el-plate on 2018-12-10.
//

#ifndef GREEN_STRUCTS_H
#define GREEN_STRUCTS_H

#include <ucontext.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define FALSE 0
#define TRUE 1

#define STACK_SIZE 4096

//thread structure
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

typedef struct queue {
    //for debugging
    int id;
    struct green_t *head;
    struct green_t *tail;
} queue;

//holds a number of suspended threads for conditions
typedef struct green_cond_t {
    queue *suspendedQueue;
} green_cond_t;

#endif //GREEN_STRUCTS_H
