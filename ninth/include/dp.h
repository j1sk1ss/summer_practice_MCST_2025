#ifndef DP_H_
#define DP_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MIN(a,b)    ((a) < (b) ? (a) : (b))
#define MAX(a,b)    ((a) > (b) ? (a) : (b))

typedef struct {
    int* array;
    int arr_size;
} dp_t;

#endif