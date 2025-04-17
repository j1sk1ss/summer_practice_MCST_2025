#ifndef SORTER_H_
#define SORTER_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sort.h"

#define SET_MAX_THREADS_OPTION  "--max_threads"

typedef struct {
    int* body;
    int size;
} arr_t;

#endif