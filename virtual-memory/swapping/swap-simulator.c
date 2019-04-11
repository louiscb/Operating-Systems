//
// Created by el-plate on 2019-04-10.
//
#include <memory.h>
#include "swap-simulator.h"
#include "swap-algorithms.h"

#define SIZE_OF_PAGE_TABLE 100
#define NUM_OF_REFERENCES 1000
#define SAMPLES 20

/**
 *
 * Here we create our model of addresses that we want to access. We have mimicked
 * a real world system by clustering addresses in a certain range instead of being evenly
 * spread around the memory.
 *
 */

void init (int *references) {
    int freq_high = 80;
    int freq_low = 20;

    //gets the number of pages that are clustered together
    int high = (int)(SIZE_OF_PAGE_TABLE*((float)freq_low/100));

    for (int i = 0; i < NUM_OF_REFERENCES; i++) {
        if (rand()%100 < freq_high) {
            references[i] = rand()%high;
        } else {
            references[i] = high + rand()%(SIZE_OF_PAGE_TABLE - high);
        }
    }
}

void clear_page_table(pte *page_table) {
    for (int i = 0; i < SIZE_OF_PAGE_TABLE; i++) {
        page_table[i].present = FALSE;
    }
}

int main(int argc, char *argv[]) {
    srand((unsigned int) time(0));
    int algorithm_type;

    if (!strcmp(argv[1], "random")) {
        printf("Random page replacement algorithm\n");
        algorithm_type = 1;
    } else if(!strcmp(argv[1], "lru")) {
        printf("LRU page replacement algorithm\n");
        algorithm_type = 2;
    }

    int * references = (int*)malloc(sizeof(int) * NUM_OF_REFERENCES);

    init(references);
    pte *page_table = (pte *)malloc(SIZE_OF_PAGE_TABLE * sizeof(pte));

    printf("# This is a benchmark of random replacement\n");
    printf("# %d page references\n", NUM_OF_REFERENCES);
    printf("# %d page \n", SIZE_OF_PAGE_TABLE);
    printf("#\n#frames\thit ratio\n");

    int incr = SIZE_OF_PAGE_TABLE/SAMPLES;

    /*
     * Here we iteratively repeat the paging simulation,
     * increasing the amount of pages the simulated process is dedicated by OS each iteration
     */
    for(int frames=incr; frames <= SIZE_OF_PAGE_TABLE; frames +=incr) {
        clear_page_table(page_table);
        int hits;

        if (algorithm_type == 1) {
            hits = random_swap(references, page_table, frames, NUM_OF_REFERENCES, SIZE_OF_PAGE_TABLE);
        } else {
            hits = least_recently_used(references, page_table, frames, NUM_OF_REFERENCES);
        }

        float ratio = (float)hits/NUM_OF_REFERENCES;
        int percentage = (int) (ratio * 100);

        printf("%d\t%d\n", frames, percentage);
    }

    return 0;
}