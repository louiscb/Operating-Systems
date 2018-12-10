//
// Created by el-plate on 2018-12-10.
//

#ifndef GREEN_QUEUE_H
#define GREEN_QUEUE_H
#include "structs.h"

void enqueue(queue *queue1, green_t *thread);

green_t *dequeue(queue *queue1);

void debugQueue(queue *queue1);

#endif //GREEN_QUEUE_H
