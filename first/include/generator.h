#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <time.h>
#include <stdlib.h>


typedef struct {
    int* body;
    int size;
} rand_arr_t;


int generate(int size, rand_arr_t* res);
int free_arr(rand_arr_t* arr);

#endif