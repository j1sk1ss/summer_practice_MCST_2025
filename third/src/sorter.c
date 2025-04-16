#include "../include/sorter.h"


static int _desc_compare(const void* a, const void* b) {
    return (*(value_t**)b)->tmp_int - (*(value_t**)a)->tmp_int;
}

static int _asc_compare(const void* a, const void* b) {
    return (*(value_t**)a)->tmp_int - (*(value_t**)b)->tmp_int;
}

int sorter_sort(commander_result_t* res) {
    if (!res || !res->h) return 0;

    int len = 0;
    value_t* curr = res->h;
    while (curr) {
        curr = curr->next;
        len++;
    }

    if (len < 2) return 1;
    value_t** arr = malloc(sizeof(value_t*) * len);
    if (!arr) return 0;

    curr = res->h;
    for (int i = 0; i < len; ++i) {
        arr[i] = curr;
        curr = curr->next;
    }

    qsort(arr, len, sizeof(value_t*), res->sort_type == 1 ? _asc_compare : _desc_compare);
    for (int i = 0; i < len - 1; ++i) arr[i]->next = arr[i + 1];
    arr[len - 1]->next = NULL;
    res->h = arr[0];

    free(arr);
    return 1;
}

