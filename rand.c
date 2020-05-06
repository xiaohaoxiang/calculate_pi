#include "rand.h"

// a copy of rand_r() in glibc
int rand31(unsigned int *seed)
{
    unsigned int next = *seed;
    int result;

    next *= 1103515245;
    next += 12345;
    result = (unsigned int)(next / 65536) % 2048;

    next *= 1103515245;
    next += 12345;
    result <<= 10;
    result ^= (unsigned int)(next / 65536) % 1024;

    next *= 1103515245;
    next += 12345;
    result <<= 10;
    result ^= (unsigned int)(next / 65536) % 1024;

    *seed = next;

    return result;
}

// generate a 52-bit random number
unsigned long long rand52(unsigned int *seed)
{
    return (((unsigned long long)rand31(seed)) << 21) ^ (unsigned long long)rand31(seed);
}

// generate a random floating-point number between 0 and 1
double lfrand(unsigned int *seed)
{
    union {
        unsigned long long u;
        double d;
    } res;
    res.u = rand52(seed) | 0x3FF0000000000000ULL;
    return res.d - 1.0;
}