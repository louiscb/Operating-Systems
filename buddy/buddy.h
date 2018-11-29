//
// Created by Louis on 2018-11-26.
//

#ifndef BUDDY_BUDDY_H

#define BUDDY_BUDDY_H
#include <stdio.h>

#define MIN  5
#define LEVELS 8
#define PAGE 4096

void test();
void bfree(void *memory);
void *balloc(size_t size);

#endif //BUDDY_BUDDY_H
