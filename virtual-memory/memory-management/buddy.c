//
// Created by Louis on 2018-11-26.
//

#include "buddy.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <assert.h>
#include <errno.h>

enum flag {Free, Taken};

struct head {
    enum flag status;
    short int level;
    struct head *next;
    struct head *prev;
};

struct head *flists[LEVELS] = {NULL};

void removeFromFlist(struct head *pHead);

struct head *new() {
    struct head *new = mmap(NULL, PAGE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(new == MAP_FAILED) {
        printf("mmap failed: error %d\n", errno);
        return NULL;
    }
    assert(((long int)new & 0xfff) == 0);  // 12 last bits should be zero
    new->status = Free;
    new->level = LEVELS -1;
    return new;
}


struct head *buddy(struct head* block) {
    int index = block->level;
    long int mask =  0x1 << (index + MIN);
    return (struct head*)((long int)block ^ mask);
}

struct head *primary(struct head* block) {
    int index = block->level;
    long int mask =  0xffffffffffffffff << (1 + index + MIN);
    return (struct head*)((long int)block & mask);
}

struct head *split(struct head *block) {
    int index = block->level - 1;
    int mask =  0x1 << (index + MIN);
    return (struct head *)((long int)block | mask );
}

void *hide(struct head* block) {
    return (void*)(block + 1);
}

struct head *magic(void *memory) {
    return (struct head*)(((struct head*)memory) - 1);
}

int level(int req) {
    int total = req  + sizeof(struct head);

    int i = 0;
    int size = 1 << MIN;
    while(total > size) {
        size <<= 1;
        i += 1;
    }
    return i;
}

void addToFlist (struct head *block) {
    int l = block->level;
    block->status = Free;

    if (flists[l] == NULL) {
        flists[l] = block;
    } else {
        struct head *temp = flists[l];
        temp->prev = block;
        block->next = temp;
        flists[l] = block;
    }
}

struct head *find(int index) {
    //base case
    if (index == LEVELS-1) {
        if (flists[LEVELS-1] != NULL) {
            struct head *block = flists[LEVELS-1];
            removeFromFlist(block);
            return block;
        }
        return new();
    }

    if (flists[index] == NULL) {
        //Check next level up
        struct head *originalBlock = find(index+1);

        // setting new splitted blocks
        struct head *splitBlock = split(originalBlock);
        splitBlock->level = index;
        struct head *primaryBlock = buddy(splitBlock);
        primaryBlock->level = index;
        primaryBlock->status = Taken;

        addToFlist(splitBlock);

        return buddy(splitBlock);
    } else {
        //Get block from list and remove it
        struct head *listBlock = flists[index];
        listBlock->status = Taken;
        flists[index] = listBlock->next;
        return listBlock;
    }
}

void insert(struct head *block) {
    block->status = Free;

    //base case
    if (block->level == LEVELS-1) {
        flists[LEVELS-1] = block;
        return;
    }

    struct head *buddyBlock = buddy(block);

    if (buddyBlock->status == Taken || buddyBlock->level == block->level) {
        //Buddy is taken or not on same level so cannot merge
        addToFlist(block);
    } else {
        //Merge blocks
        block->level++;
        removeFromFlist(buddyBlock);
        insert(block);
    }

    return;
}

void removeFromFlist(struct head *block) {
    struct head *listHead = flists[block->level];

    do {
        if ((long int)listHead==(long int)block) {
            if (block->prev == NULL && block->next == NULL) {
                // Why can't i write this -> listHead = NULL;
                flists[block->level] = NULL;
                return;
            }

            if (block->prev != NULL) {
                block->prev->next = block->next;
            }

            if (block->next != NULL) {
                block->next->prev = block->prev;
            }

            return;
        }
    } while (listHead->next != NULL);

    return;
}

void *balloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    int index = level(size);
    struct head *taken = find(index);
    return hide(taken);
}

void bfree(void *memory) {
    if (memory != NULL) {
        struct head *block = magic(memory);
        insert(block);
    }
    return;
}

// Test sequences
void test() {
    printf("Running tests...\n");
    balloc(10);
    balloc(10);
    struct head *f2 =balloc(10);
    bfree(f2);
    balloc(10);
    printf("Need some logs! \n");
}