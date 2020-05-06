#include "timer.h"

timepoint time_now()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (timepoint)(ts.tv_sec * 10000.0 + ts.tv_nsec / 100000.0 + 0.5);
}