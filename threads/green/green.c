//
// Created by el-plate on 2018-12-07.
//

#include "green.h"

int green_create(green_t *thread, void *(*fun)(void*), void *arg) {
    return 1;
}

int green_yield() {
    return 1;
}

int green_join(green_t *thread) {
    return 1;
}