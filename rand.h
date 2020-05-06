#ifndef RNAD_H
#define RAND_H

#define RAND31_MAX 2147483647

// a copy of rand_r() in glibc
int rand31(unsigned int *seed);

// generate a 52-bit random number
unsigned long long rand52(unsigned int *seed);

// generate a random floating-point number between 0 and 1
double lfrand(unsigned int *seed);

#endif // RNAD_H