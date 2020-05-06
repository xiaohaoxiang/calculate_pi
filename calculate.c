#include "calculate.h"
#include "rand.h"
#include <stdlib.h>

value_type calc_integral(const size_type N, const size_type HC)
{
    typedef data_integral data_type;

    void *const buffer = malloc(HC * (sizeof(pthread_t) + sizeof(data_type)));
    pthread_t *const threads = (pthread_t *)buffer;
    data_type *const data = (data_type *)(buffer + HC * sizeof(pthread_t));

    value_type res = 0;
    pthread_mutex_t mtx;
    pthread_mutex_init(&mtx, NULL);
    const size_type d = (N + HC - 1) / HC;
    data->N1 = 1.0 / N;
    for (size_type i = 0; i < HC; ++i)
    {
        data_type *const cur = data + i;
        cur->begin = i * d;
        cur->end = i + 1 == HC ? N + 1 : (i + 1) * d;
        cur->N1 = data->N1;
        cur->p_mtx = &mtx;
        cur->p_res = &res;
        pthread_create(threads + i, NULL, (void *(*)(void *))calc_integral_thread, (void *)cur);
    }
    for (size_type i = 0; i < HC; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    free(buffer);
    return res / N;
}

value_type calc_probability(const size_type N, const size_type HC)
{
    typedef data_probability data_type;

    void *const buffer = malloc(HC * (sizeof(pthread_t) + sizeof(data_type)));
    pthread_t *const threads = (pthread_t *)buffer;
    data_type *const data = (data_type *)(buffer + HC * sizeof(pthread_t));

    size_type res = 0;
    pthread_mutex_t mtx;
    pthread_mutex_init(&mtx, NULL);

    const size_type d = (N + HC - 1) / HC;
    for (size_type i = 0; i < HC; ++i)
    {
        data_type *const cur = data + i;
        cur->N = i + 1 == HC ? N - i * d : d;
        cur->p_mtx = &mtx;
        cur->p_res = &res;
        pthread_create(threads + i, NULL, (void *(*)(void *))calc_probability_thread, (void *)cur);
    }
    for (size_type i = 0; i < HC; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    free(buffer);
    return 4.0 * res / N;
}

value_type calc_series(const size_type N, const size_type HC)
{
    typedef data_series data_type;

    void *const buffer = malloc(HC * (sizeof(pthread_t) + sizeof(data_type)));
    pthread_t *const threads = (pthread_t *)buffer;
    data_type *const data = (data_type *)(buffer + HC * sizeof(pthread_t));

    value_type res = 0;
    pthread_mutex_t mtx;
    pthread_mutex_init(&mtx, NULL);
    const size_type d = (N + HC - 1) / HC;
    for (size_type i = 0; i < HC; ++i)
    {
        data_type *const cur = data + i;
        cur->begin = 2 * i * d + 1;
        cur->end = i + 1 == HC ? 2 * N + 1 : 2 * (i + 1) * d + 1;
        cur->p_mtx = &mtx;
        cur->p_res = &res;
        pthread_create(threads + i, NULL, (void *(*)(void *))calc_series_thread, (void *)cur);
    }
    for (size_type i = 0; i < HC; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    free(buffer);
    return 4.0 * res;
}

void calc_integral_thread(data_integral *const data)
{
    value_type sum = 0;
    for (; data->begin < data->end; ++data->begin)
    {
        value_type tmp = (data->begin + 0.5) * data->N1;
        sum += 4.0 / (1.0 + tmp * tmp);
    }
    pthread_mutex_lock(data->p_mtx);
    *(data->p_res) += sum;
    pthread_mutex_unlock(data->p_mtx);
}

void calc_probability_thread(data_probability *const data)
{
    size_type cnt = 0;
    unsigned seed = (unsigned)(pthread_self() ^ clock());
    while (data->N--)
    {
        value_type x = lfrand(&seed), y = lfrand(&seed);
        if (x * x + y * y <= 1.0)
        {
            ++cnt;
        }
    }
    pthread_mutex_lock(data->p_mtx);
    *(data->p_res) += cnt;
    pthread_mutex_unlock(data->p_mtx);
}

void calc_series_thread(data_series *const data)
{
    value_type sum = 0;
    for (; data->begin < data->end; data->begin += 2)
    {
        if (data->begin & 2)
        {
            sum -= 1.0 / data->begin;
        }
        else
        {
            sum += 1.0 / data->begin;
        }
    }
    pthread_mutex_lock(data->p_mtx);
    *(data->p_res) += sum;
    pthread_mutex_unlock(data->p_mtx);
}

const calc_func_t functions[MAX_MODELS] = {calc_integral, calc_probability, calc_series};
const char *const func_names[MAX_MODELS] = {"Integral", "Probability", "Series"};