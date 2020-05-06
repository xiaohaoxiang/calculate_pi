#ifndef TEST_H
#define TEST_H

#include "calculate.h"
#include "defs.h"
#include "timer.h"
#include <stdio.h>

typedef struct
{
    // input data
    size_type repeat_times; // time to repeat for each test
    size_type problem_size; // size for one calculating model
    size_type thread_count; // count of the threads
    calc_model model;       // which model to use

    // output data
    duration *time;     // time spent of each test/0.1ms
    value_type *result; // result of each test
} testcase;

typedef struct
{
    // input data
    size_type n;     // size of test set
    testcase *cases; // testcases
} testcase_list;

void output_rawdata(testcase_list *tcl);

void output_result(testcase_list *tcl);

void test_all(testcase_list *tcl);

void test_single(testcase *const tc);

void testcase_list_init(testcase_list *const tcl);

void testcase_list_release(testcase_list *const tcl);

// calculate how many bits are the same as pi, up to sizeof(pi_str)
size_type same_bits(double x);

#endif // TEST_H