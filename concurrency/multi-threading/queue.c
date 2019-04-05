//
// Created by el-plate on 2018-12-10.
//

#include "queue.h"

void enqueue(queue *queue1, green_t *thread) {
    //printf("ENQUEUE %d: %lx \n ", queue1->id, thread);

    if (thread == NULL)
        return;

    if (queue1->tail == NULL || queue1->head == NULL) {
        queue1->head = queue1->tail = thread;
        return;
    }

    //Maybe as a precaution?
  //  thread->next = NULL;

    queue1->tail->next = thread;
    queue1->tail = thread;
}

green_t *dequeue(queue *queue1) {
    if (queue1->head == NULL)
        return NULL;

    green_t *temp = queue1->head;
    queue1->head = queue1->head->next;

    //Just in case there is a zombie in the wrong place
    if (temp->zombie == TRUE) {
        printf("ERROR: Zombie in Ready Queue");
        return dequeue(queue1);
    }

    temp->next = NULL;
    //printf("DEQUEUE %d: %lx \n ", queue1->id, temp);
    return temp;
}

void debugQueue(queue *queue1) {
    green_t *temp = queue1->head;
    if (!temp)
        return;

    green_t use = *temp;

    printf("\n- QUEUE %d-\n", queue1->id);
    printf("0-%lx", temp);
    for (int i = 1; i < 10; ++i) {
        if (use.next == NULL) {
            break;
        }

        printf(" --> %d-%lx ",i, use.next);
        use = *use.next;
    }
    printf("\n--\n\n");
}