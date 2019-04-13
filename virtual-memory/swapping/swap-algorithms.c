//
// Created by Louis on 2019-04-11.
//

//
// Collection of page replacement algorithms that simulate paging in a system.
//
// The efficiency varies between algorithms based on which page they select to evict once the
// page table (which is the RAM) is filled up and the process requires a new page from disk to be put
// on the main memory.
//

#include "swap-algorithms.h"


/**
 * This is the simplest of the simulators.
 * Here our "Page replacement Algorithm" consists of randomly selecting a page.
 */

int random_swap(int *addresses, pte *table, int frames, int references, int pages) {
    int hits = 0;
    int allocated = 0;

    for (int i = 0; i < references; i++) {
        int next = addresses[i];
        pte *entry = &table[next];
        //printf("ref: %d present %d\n", next, entry->present);

        if (entry->present == TRUE) {
            // the reference is in the main memory
            hits++;
        } else {
            // Page fault occurs here
            if (allocated < frames) {
                allocated++;
                entry->present = TRUE;
            } else {
                // Page replacement algorithm is here
                // we are randomly selecting a page to remove once the page table is full
                pte *evict;

                do {
                    int rn = rand()%pages;
                    evict = &table[rn];
                } while(evict->present == FALSE);

                evict->present = FALSE;
                entry->present = TRUE;
            }
        }
    }

    return hits;
}

int least_recently_used(int *addresses, pte *table, int frames, int references) {
    int hits = 0;
    int allocated = 0;
    pte *first = NULL;
    pte *last = NULL;

    for (int i = 0; i < references; i++) {
        int next = addresses[i];
        pte *entry = &table[next];

        //printf("ref: %d present %d\n", next, entry->present);

        if (entry->present == TRUE) {
            // the reference is in the main memory
            hits++;

            if(entry->next != NULL) {
                if(first == entry) {
                    first = entry->next;
                } else {
                    entry->previous->next = entry->next;
                }
                entry->next->previous = entry->previous;

                entry->previous = last;
                entry->next = NULL;

                last->next = entry;
            }
        } else {
            // Page fault occurs here
            if (allocated < frames) {
                allocated++;
                entry->present = TRUE;
                entry->previous = last;
                entry->next = NULL;

                if(last != NULL)
                    last->next = entry;

                if(first == NULL)
                    first = entry;

            } else {
                pte *evict;
                //printf("here %d, %p \n", evict->present, evict->previous);

                assert(first !=NULL);

                evict = first;
                first = evict->next;
                evict->present = FALSE;
                entry->present = TRUE;
                entry->previous = last;
                entry->next = NULL;

                if (last!= entry)
                    last->next = entry;

                if (first==NULL)
                    first = entry;

            }
        }

        last = entry;
    }

    return hits;
}