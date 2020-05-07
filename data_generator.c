#include "cpuinfo.h"
#include "defs.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
    int repeat;
    if (argc <= 1 || sscanf(argv[1], "%d", &repeat) != 1)
    {
        repeat = 4;
    }

    unsigned max_hc = hardware_concurrency();
    if (max_hc <= 1)
    {
        max_hc = 2;
    }
    max_hc *= 2;
    if (max_hc > MAX_THREADS)
    {
        max_hc = MAX_THREADS;
    }

    for (int mod = 0; mod < MAX_MODELS; ++mod)
    {
        for (unsigned hc = 1; hc <= max_hc; hc += (hc << 1) < max_hc ? 1 : 2)
        {
            printf("%d %u %llu %d", mod, hc, MAX_THREADS * MAX_THREADS, repeat);
        }
    }
    return 0;
}