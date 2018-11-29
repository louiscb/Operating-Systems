//
// Created by Louis on 2018-11-29.
//

#include "benchmark.h"
#include "buddy.h"

int memory[LEVELS];

void benchmark() {
    for (int i = 0; i < LEVELS; ++i) {
        memory[i] = 2^(i+MIN);
    }
    
}
