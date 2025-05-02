#ifndef PETERSONLOCK_H
#define PETERSONLOCK_H

struct petersonlock {
    int interested[2];
    int barrier;
    int acquired;
};

#endif

