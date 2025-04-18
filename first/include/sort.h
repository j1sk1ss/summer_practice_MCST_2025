#ifndef SORT_H_
#define SORT_H_

#include <pthread.h>

typedef struct {
    int* arr;
    int left;
    int right;
    int sortt;
} thread_args_t;


int sort(int* arr, int size, int sort_type);
int set_thread_limit(int lim);

#endif