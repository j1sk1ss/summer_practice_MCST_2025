#include "include/sorter.h"


int main(int argc, char* argv[]) {
    int arr_size = 10000;
    int sort_type = 0;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], SET_MAX_THREADS_OPTION)) {
            if (i + 1 >= argc) continue;
            else {
                int chunks = atoi(argv[i++ + 1]);
                if (chunks <= 0) {
                    printf("[WARN] %i value is incorrect for --max_threads. Changed to 1!\n", chunks);
                    chunks = 1;
                }

                set_thread_limit(chunks);
            }
        }
        else if (!strcmp(argv[i], ARR_SIZE)) {
            if (i + 1 >= argc) continue;
            else arr_size = atoi(argv[i + 1]);
        }
        else if (!strcmp(argv[i], ASCENDING_SORT_OPTION))   sort_type = 0;
        else if (!strcmp(argv[i], DESCENDING_SORT_OPTION))  sort_type = 1;
    }

    rand_arr_t arr;
    if (!generate(arr_size, &arr)) {
        fprintf(stderr, "Array gen error!\n");
        exit(EXIT_FAILURE);
    }

    clock_t start_time = clock();
    if (!sort(arr.body, arr.size, sort_type)) {
        free_arr(&arr);
        fprintf(stderr, "Sorting error!\n");
        exit(EXIT_FAILURE);
    }

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Sorted array is:\n");
    for (int i = 0; i < arr.size; i++) {
        if (i % 10 == 0) printf("\n");
        printf("%d ", arr.body[i]);
    }

    printf("\nSorting took %.6f seconds.\n", elapsed_time);
    free_arr(&arr);
    return EXIT_SUCCESS;
}
