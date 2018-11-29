//
// Created by Louis on 2018-11-29.
//

#include "benchmark.h"
#include "buddy.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

size_t memory[LEVELS];

enum func {
    BALLOC,
    MALLOC,
    BFREE,
    FREE
};

double testFunc(enum func, size_t, long int);

void benchmark() {
    for (size_t i = 0; i < LEVELS; ++i) {
        memory[i] = (size_t)pow(2, i+MIN);
        printf("BALLOC: %ld\n", testFunc(BALLOC, memory[i], NULL));
        printf("MALLOC: %ld\n", testFunc(MALLOC, memory[i], NULL));
    }

}

double testFunc(enum func type, size_t size, long int addr) {
    clock_t start_t, end_t, total_t;

    start_t = clock();

    switch (type) {
        case BALLOC :
            balloc(size);
            break;
        case MALLOC :
            malloc(size);
            break;
        case BFREE :
            bfree(addr);
            break;
        case FREE :
            free(addr);
            break;
    }

    end_t = clock();

    return (double)(end_t - start_t)/CLOCKS_PER_SEC;
}
