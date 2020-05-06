#include "test.h"
#include "file.h"
#include "pi.h"
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    testcase_list testcl;
    { // read arguments and initialize testcl
        size_type repeat_time, problem_size, thread_count, capacity = 16;
        int model;

        if (argc > 1)
        {
            if (freopen(argv[1], "r", stdin) == NULL)
            {
                fprintf(stderr, "invalid path\n");
            }
        }
        testcl.n = 0;
        testcl.cases = (testcase *)malloc(capacity * sizeof(testcase));
        for (;;)
        {
            if (scanf("%d%llu%llu%llu \n", &model, &thread_count, &problem_size, &repeat_time) != 4)
            {
                break;
            }
            else if (!(0 <= model && model <= MAX_MODELS) || !(0 < thread_count && thread_count <= MAX_THREADS) ||
                     problem_size < thread_count || repeat_time == 0)
            {
                fprintf(stderr, "invalid line %llu: %d %llu %llu %llu", testcl.n, model, thread_count, problem_size,
                        repeat_time);
                return -1;
            }

            testcl.cases[testcl.n].repeat_times = repeat_time;
            testcl.cases[testcl.n].problem_size = problem_size;
            testcl.cases[testcl.n].thread_count = thread_count;
            testcl.cases[testcl.n].model = model;

            if (++testcl.n == capacity)
            {
                capacity <<= 1;
                testcl.cases = (testcase *)realloc(testcl.cases, capacity * sizeof(testcase));
            }
        }
    }
    testcase_list_init(&testcl);
    test_all(&testcl);
    output_rawdata(&testcl);
    printf("\n\n--------------------------\n\n");
    output_result(&testcl);
    testcase_list_release(&testcl);
    return 0;
}

void output_rawdata(testcase_list *tcl)
{
    // FILE *os = get_outfile("rawdata", ".txt");
    FILE *os = stdout;
    fprintf(os, "%-6s %-5s %-7s %-16s %-7s %s\n", "num", "model", "threads", "problem_size", "repeat",
            "(time/ms, result)");
    for (size_type i = 0; i < tcl->n; ++i, fputc('\n', os))
    {
        fprintf(os, "%-6llu %-5d %-7llu %-16llu %-8llu ", i, tcl->cases[i].model, tcl->cases[i].thread_count,
                tcl->cases[i].problem_size, tcl->cases[i].repeat_times);
        for (size_type j = 0; j < tcl->cases[i].repeat_times; ++j)
        {
            fprintf(os, " (%.1f, %.20f)", tcl->cases[i].time[j] / 10.0, tcl->cases[i].result[j]);
        }
    }
    fclose(os);
}

void output_result(testcase_list *tcl)
{
    // FILE *os = get_outfile("result", ".txt");
    FILE *os = stdout;
    fprintf(os, "%-6s %-5s %-7s %-16s %-7s %-12s  %-22s %s\n", "num", "model", "threads", "problem_size", "repeat",
            "mean_time/ms", "mean_value", "mean_precision");
    for (size_type i = 0; i < tcl->n; ++i, fputc('\n', os))
    {
        fprintf(os, "%-6llu %-5d %-7llu %-16llu %-2llu ", i, tcl->cases[i].model, tcl->cases[i].thread_count,
                tcl->cases[i].problem_size, tcl->cases[i].repeat_times);
        size_type sumt = 0;
        value_type sumv = 0;
        for (size_type j = 0; j < tcl->cases[i].repeat_times; ++j)
        {
            sumt += tcl->cases[i].time[j];
            sumv += tcl->cases[i].result[j];
        }
        sumv /= tcl->cases[i].repeat_times;
        fprintf(os, "        %6.2f    %.20f    %d", sumt / (10.0 * tcl->cases[i].repeat_times), sumv, same_bits(sumv));
    }
    fclose(os);
}

void test_all(testcase_list *tcl)
{
    for (size_type i = 0; i < tcl->n; ++i)
    {
        test_single(tcl->cases + i);
    }
}

void test_single(testcase *const tc)
{
    for (size_type i = 0; i < tc->repeat_times; ++i)
    {
        timepoint t0 = time_now();
        tc->result[i] = functions[tc->model](tc->problem_size, tc->thread_count);
        tc->time[i] = time_now() - t0;
    }
}

void testcase_list_init(testcase_list *const tcl)
{
    size_type sum = 0;
    for (size_type i = 0; i < tcl->n; ++i)
    {
        sum += tcl->cases[i].repeat_times;
    }
    void *buffer = malloc(sum * (sizeof(duration) + sizeof(value_type)));
    sum = 0;
    for (size_type i = 0; i < tcl->n; ++i)
    {
        tcl->cases[i].time = buffer + sum;
        tcl->cases[i].result = buffer + sum + tcl->cases[i].repeat_times * sizeof(duration);
        sum += tcl->cases[i].repeat_times * (sizeof(duration) + sizeof(value_type));
    }
}

void testcase_list_release(testcase_list *const tcl)
{
    free(tcl->cases[0].time);
    return free(tcl->cases);
}

// calculate how many bits are the same as pi, up to sizeof(pi_str)
size_type same_bits(double x)
{
    for (size_type i = 0; i < sizeof(pi_str); ++i)
    {
        int a = (int)x;
        if (a + '0' != pi_str[i])
        {
            return i;
        }
        x = (x - a) * 10;
    }
    return sizeof(pi_str);
}