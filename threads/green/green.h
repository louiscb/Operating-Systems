//
// Created by el-plate on 2018-12-07.
//
#ifndef GREEN_H
#define GREEN_H

#include "common.h"

typedef struct green_mutex_t {
    volatile int taken;
    queue *susp;
} green_mutex_t;

/**Initialise a green thread
 * our version of pthread_create()
 */
int green_create(green_t *thread, void *(*fun)(void*), void *arg);

/** Suspends current thread and selects new thread for execution
 *   our version of pthread_yield()
 *
 * Function should put the running thread last in the ready queue
 * then select first thread from the queue as next thread
 */
int green_yield();

/**
 * Current thread is suspended waiting for a thread to terminate
 * our version of pthread_join()
 * This is used so that the main thread doesn't return
 * before a secondary thread has finished processing.
 */
int green_join(green_t *thread);

/**
 * initialse a green condition variable
 *
 * our equivalent to pthread_cond_init
 *
 * @param greenCond
 */
void green_cond_init(green_cond_t *greenCond);

/**
 * suspend the current thread on the condition
 *
 * equivalent to pthread_cond_wait
 *
 * @param greenCond
 */
void green_cond_wait(green_cond_t *greenCond);

/**
 * move the first suspended thread to the ready queue
 *
 * equivalent to pthread_cond_signal
 *
 * @param greenCond
 */
void green_cond_signal(green_cond_t *greenCond);

/**
 * these two functions are used to start and stop the timer
 * so that the interrupts only occur during the threads internal logic
 * and not in one of our functions that manipulate the state of said threads
 */
void unblockTimer();
void blockTimer();

int green_mutex_init(green_mutex_t *mutex);

int green_mutex_lock(green_mutex_t *mutex);

int green_mutex_unlock(green_mutex_t *mutex);

#endif //GREEN_GREEN_H
