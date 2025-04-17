#include "../include/sort.h"


int _swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
    return 1;
}

int _work(int* arr, int left, int right) {
    int i = left - 1;
    int pivot = arr[right];

    for (int j = left; j < right; j++) {
        if (arr[j] < pivot) _swap(&arr[++i], &arr[j]);
    }

    _swap(&arr[i + 1], &arr[right]);
    return i + 1;
}


int _threads_lim = 50;
pthread_mutex_t _count_lock = PTHREAD_MUTEX_INITIALIZER;
static int _sort_qsort(int* arr, int left, int right);

static void* _sort_thread_qsort(void* args) {
    thread_args_t* targs = (thread_args_t*)args;
    _sort_qsort(targs->arr, targs->left, targs->right);
    return NULL;
}

static int _sort_qsort(int* arr, int left, int right) {
    if (left >= right) return 0;
    int pivot_index = _work(arr, left, right);

    pthread_t fthread, sthread;
    int fthread_stat = 0, sthread_stat = 0;
    thread_args_t fargs = { .arr = arr, .left = left, .right = pivot_index - 1  };
    thread_args_t sargs = { .arr = arr, .left = pivot_index + 1, .right = right };

    pthread_mutex_lock(&_count_lock);
    if (_threads_lim >= 2) {
        _threads_lim -= 2;
        pthread_mutex_unlock(&_count_lock);

        fthread_stat = (pthread_create(&fthread, NULL, _sort_thread_qsort, &fargs) == 0);
        if (fthread_stat) pthread_join(fthread, NULL);
        sthread_stat = (pthread_create(&sthread, NULL, _sort_thread_qsort, &sargs) == 0);
        if (sthread_stat) pthread_join(sthread, NULL);

        pthread_mutex_lock(&_count_lock);
        _threads_lim += fthread_stat + sthread_stat;
        pthread_mutex_unlock(&_count_lock);
    } 
    else {
        pthread_mutex_unlock(&_count_lock);
        _sort_qsort(arr, left, pivot_index - 1);
        _sort_qsort(arr, pivot_index + 1, right);
    }

    return 1;
}


int sort(int* arr, int size) {
    return _sort_qsort(arr, 0, size - 1);
}

int set_thread_limit(int lim) {
    _threads_lim = lim;
    return 1;
}
