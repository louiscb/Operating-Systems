//
// Created by nascosto on 2019-04-06.
//

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

char global[] = "Global text";
const int readOnly = 123;


void bar(unsigned long *ferma) {
    unsigned long r = 0x3;

    unsigned long *i;

    printf("Address           Value\n");
    for(i = &r; i <= ferma; i++) {
        printf("%p    0x%lx \n", i, *i);
    }
}

void foo(unsigned long *ferma) {
    unsigned long q = 0x2;
    bar(ferma);
}


int main () {
    int pid = getpid();
    void* ptr = malloc(100);
    char maps[20];
    sprintf(maps, "/proc/%d/maps", pid);

    label:
        printf("\n");
        printf("Global variable: %p\n", &global);
        printf("The code segment: %p\n", &&label);
        printf("Read only global variable: %p\n", &readOnly);
        printf("Malloc pointer: %p - This should be on the stack\n", &ptr);
        printf("Malloc memory: %p - This should be on the heap\n", ptr);

        printf("\nStack trace - pay attention to the location of the values we have assigned:\n\n");
        unsigned long localVar = 0x1;
        foo(&localVar);

        printf("\n\nBelow are the contents of %s \n\n", maps);

        char command[20];
        sprintf(command, "cat %s", maps);
        system(command);

    return 0x0;

}
