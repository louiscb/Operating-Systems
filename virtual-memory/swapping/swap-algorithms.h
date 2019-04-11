//
// Created by Louis on 2019-04-11.
//

#ifndef OPERATING_SYSTEMS_SWAP_ALGORITHMS_H
#define OPERATING_SYSTEMS_SWAP_ALGORITHMS_H

#include "swap-simulator.h"

int random_swap(int *addresses, pte *table, int frames, int references, int pages);

int least_recently_used(int *addresses, pte *table, int frames, int references);


#endif //OPERATING_SYSTEMS_SWAP_ALGORITHMS_H
