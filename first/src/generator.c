#include "../include/generator.h"


int generate(int size, rand_arr_t* res) {
    srand(time(NULL));
    int* arr = (int*)malloc(size * sizeof(int));
    if (!arr) return 0;

    for (int i = 0; i < size; i++) {
        arr[i] = rand();
    }

    res->body = arr;
    res->size = size;
    return 1;
}

int free_arr(rand_arr_t* arr) {
    if (arr->body) free(arr->body);
    return 1;
}
