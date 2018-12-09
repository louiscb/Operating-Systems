#include <stdio.h>
#include "green.h"

void *test(void *arg) {
    int i = *(int*)arg;
    int loop = 10;

    while (loop > 0) {
        printf("thread %d: %d\n", i, loop);
        loop--;
        green_yield();
    }
}

int main() {
    printf(" - Testing -\n");
    green_t g0, g1;
    int a0 = 0;
    int a1 = 1;

    green_create(&g0, test, &a0);
    green_create(&g1, test, &a1);

    green_join(&g0);
    green_join(&g1);

    printf("done\n");

    return 0;
}