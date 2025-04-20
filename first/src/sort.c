#include "../include/sort.h"


int _swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
    return 1;
}

int _work(int* arr, int left, int right, int asc) {
    int i = left - 1;
    int pivot = arr[right];

    for (int j = left; j < right; j++) {
        if ((asc && arr[j] < pivot) || (!asc && arr[j] > pivot)) {
            _swap(&arr[++i], &arr[j]);
        }
    }

    _swap(&arr[i + 1], &arr[right]);
    return i + 1;
}

static int _sort_qsort(int* arr, int left, int right, int sort_type) {
    if (left >= right) return 0;
    int pivot_index = _work(arr, left, right, sort_type);
    _sort_qsort(arr, left, pivot_index - 1, sort_type);
    _sort_qsort(arr, pivot_index + 1, right, sort_type);
    return 1;
}

void* _sort_part(void* args) {
    thread_args_t* targs = (thread_args_t*)args;
    _sort_qsort(targs->info.arr, targs->info.l, targs->info.r, targs->sortt);
    free(args);
    return NULL;
}

/*
Main idea in chunks. We know, that array splitted into chunks like:
0 1 2 | 9 20 100 | 0 1 132
If we create info array about local indexes in every chunk, like:
0 1 2 | 9 20 100 | 0 1 132
|       |          |
l1      l2         l3
We can compare elements between each other. And when we found smallest (largest), we
move pinter in chunk where it was found.
*/
static int _merge(int* dst, int* src, int size, int chunks, int sort_type) {
    int* indices = (int*)calloc(chunks, sizeof(int));
    int* limits = (int*)malloc(chunks * sizeof(int));
    int chunk = size / chunks;
    if (!indices || !limits) {
        free(indices);
        free(limits);
        return 0;
    }

    /*
    Generate bounds for every chunk
    */
    for (int i = 0; i < chunks; i++) {
        limits[i] = (i == chunks - 1) ? size : (i + 1) * chunk;
    }

    for (int i = 0; i < size; i++) {
        int chosen_chunk = -1;
        int chosen_value = 0;

        for (int j = 0; j < chunks; j++) {
            int idx = j * chunk + indices[j];
            if (idx >= limits[j]) continue;

            int val = src[idx];
            if (chosen_chunk == -1 || 
                (sort_type && val < chosen_value) || 
                (!sort_type && val > chosen_value)) {
                chosen_chunk = j;
                chosen_value = val;
            }
        }

        dst[i] = chosen_value;
        indices[chosen_chunk]++;
    }

    free(indices);
    free(limits);
    return 1;
}

static int _threads_lim = 8;

int sort(int* arr, int size, int sort_type) {
    pthread_t* threads = (pthread_t*)malloc(_threads_lim * sizeof(pthread_t));
    if (!threads) return 0;

    int* tmp = malloc(sizeof(int) * size);
    if (!tmp) {
        free(threads);
        return 0;
    }

    /*
    Send array pointer to threads with different left and right bound.
    That means, array will be sorted at _thread_lim part.
    */
    int chunk = size / _threads_lim;
    for (int i = 0; i < _threads_lim; i++) {
        thread_args_t* arg = malloc(sizeof(thread_args_t));
        arg->info.arr = arr;
        arg->info.l   = i * chunk;
        arg->info.r   = (i == _threads_lim - 1) ? size - 1 : (i + 1) * chunk - 1;
        arg->sortt    = sort_type;
        pthread_create(&threads[i], NULL, _sort_part, arg);
    }

    /*
    Wait all threads.
    */
    for (int i = 0; i < _threads_lim; i++) {
        pthread_join(threads[i], NULL);
    }

    _merge(tmp, arr, size, _threads_lim, sort_type);
    memcpy(arr, tmp, sizeof(int) * size);
    free(tmp);

    free(threads);
    return 1;
}

int set_thread_limit(int lim) {
    _threads_lim = lim;
    return 1;
}
