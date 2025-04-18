#ifndef SORT_H_
#define SORT_H_

#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int l;
    int r;
    int* arr;
} arr_info_t;

typedef struct {
    arr_info_t info;
    int sortt;
} thread_args_t;


int sort(int* arr, int size, int sort_type);
int set_thread_limit(int lim);

#endif