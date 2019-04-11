//
// Created by Louis on 2019-04-11.
//

#ifndef OPERATING_SYSTEMS_SWAP_SIMULATOR_H
#define OPERATING_SYSTEMS_SWAP_SIMULATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

typedef struct pte {
    int present;
    struct pte *next;
    struct pte *previous;
} pte;

#endif //OPERATING_SYSTEMS_SWAP_SIMULATOR_H
