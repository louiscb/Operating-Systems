//
// Created by Louis on 2018-11-19.
//

#ifndef EXERCISES_COMMON_H
#define EXERCISES_COMMON_H

#include <sys/time.h>
#include <sys/stat.h>
#include <assert.h>

double GetTime() {
    struct timeval t;
    int rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (double) t.tv_sec + (double) t.tv_usec/1e6;
}

void Spin(int howlong) {
    double t = GetTime();
    while ((GetTime() - t) < (double) howlong)
        ; // do nothing in loop
}

#endif //EXERCISES_COMMON_H
