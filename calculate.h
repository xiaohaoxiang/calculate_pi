#ifndef CALCULATE_H
#define CALCULATE_H

#include "defs.h"
#include <pthread.h>

typedef value_type (*calc_func_t)(const size_type N, const size_type HC);
typedef struct
{
    value_type *p_res;
    pthread_mutex_t *p_mtx;
    size_type begin;
    size_type end;
    value_type N1;
} data_integral;
typedef struct
{
    size_type *p_res;
    pthread_mutex_t *p_mtx;
    size_type N;
} data_probability;
typedef struct
{
    value_type *p_res;
    pthread_mutex_t *p_mtx;
    size_type begin;
    size_type end;
} data_series;
typedef enum
{
    integral = 0,
    probability = 1,
    series = 2
} calc_model;

value_type calc_integral(const size_type N, const size_type HC);
value_type calc_probability(const size_type N, const size_type HC);
value_type calc_series(const size_type N, const size_type HC);

void calc_integral_thread(data_integral *const data);
void calc_probability_thread(data_probability *const data);
void calc_series_thread(data_series *const data);

extern const calc_func_t functions[MAX_MODELS];
extern const char *const func_names[MAX_MODELS];

#endif // CALCULATE_H