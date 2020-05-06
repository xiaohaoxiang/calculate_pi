#ifndef TIMER_H
#define TIMER_H

#include <time.h>

typedef unsigned long long timepoint;
typedef long long duration;

timepoint time_now();

#endif // TIMER_H