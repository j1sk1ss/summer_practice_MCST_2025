#include "include/sorter.h"


static int _count_lines(FILE* f) {
    int count = 0;
    for (char c = getc(f); c != EOF; c = getc(f)) {
        if (c == '\n') count++;
    }

    return count;
}

static int _generate_array(char* path, arr_t* arr) {
    if (!path) return 0;
    FILE* f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "File [%s] not found!\n", path);
        return 0;
    }

    arr->size = _count_lines(f);
    fseek(f, 0, SEEK_SET);
    arr->body = (int*)malloc(arr->size * sizeof(int));
    if (!arr->body) {
        fclose(f);
        return 0;
    } 

    size_t line_len = 0;
    char* line = NULL;
    int num_index = 0;
    while (getline(&line, &line_len, f) != -1) {
        arr->body[num_index++] = atoi(line);
        free(line);
        line = NULL;
    }

    if (line) free(line);
    return 1;
}

static int _free_arr(arr_t* arr) {
    if (arr->body) free(arr->body);
    return 1;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No args provided! Usage ./prog <path> <args>\n");
        exit(EXIT_FAILURE);
    }

    char* input_file = NULL;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], SET_MAX_THREADS_OPTION)) {
            if (i + 1 >= argc) continue;
            else set_thread_limit(atoi(argv[i++ + 1]));
        }
        else {
            input_file = argv[i];
        }
    }

    arr_t arr;
    if (!_generate_array(input_file, &arr)) {
        fprintf(stderr, "File read or array gen error!\n");
        exit(EXIT_FAILURE);
    }

    if (!sort(arr.body, arr.size)) {
        _free_arr(&arr);
        fprintf(stderr, "Sorting error!\n");
        exit(EXIT_FAILURE);
    }

    printf("Sorted array is:\n");
    for (int i = 0; i < arr.size; i++) {
        if (i % 10 == 0) printf("\n");
        printf("%d ", arr.body[i]);
    }

    printf("\nSort complete.\n");
    _free_arr(&arr);
    return 1;
}
