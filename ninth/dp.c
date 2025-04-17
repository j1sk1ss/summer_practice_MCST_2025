#include "include/dp.h"


static int _count_lines(FILE* f) {
    int count = 0;
    for (char c = getc(f); c != EOF; c = getc(f)) {
        if (c == '\n') count++;
    }

    return count;
}

static int _generate_array(char* path, dp_t* dp) {
    FILE* f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "File [%s] not found!\n", path);
        return 0;
    }

    dp->arr_size = _count_lines(f);
    fseek(f, 0, SEEK_SET);
    dp->array = (int*)malloc(dp->arr_size * sizeof(int));
    if (!dp->array) {
        fclose(f);
        return 0;
    } 

    size_t line_len = 0;
    char* line = NULL;
    int num_index = 0;
    while (getline(&line, &line_len, f) != -1) {
        dp->array[num_index++] = atoi(line);
        free(line);
        line = NULL;
    }

    if (line) free(line);
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No args provided! Usage ./prog <path>\n");
        exit(EXIT_FAILURE);
    }

    dp_t dp;
    if (!_generate_array(argv[1], &dp)) {
        fprintf(stderr, "File read or array generation error!\n");
        exit(EXIT_FAILURE);
    }

    int* dp_arr = (int*)malloc(dp.arr_size * sizeof(int));
    if (!dp_arr) {
        fprintf(stderr, "Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }

    dp_arr[0] = dp.array[0];
    int l = 0, curr_l = 0, r = dp.arr_size, max = 0;
    for (int i = 1; i < dp.arr_size; i++) {
        if (dp.array[i - 1] <= dp.array[i]) dp_arr[i] = dp_arr[i - 1] + dp.array[i];
        else {
            dp_arr[i] = dp.array[i];
            curr_l = i;
        }

        if (dp_arr[i] > max) {
            max = dp_arr[i];
            l = curr_l;
            r = i;
        }
    }

    for (int i = 0; i < dp.arr_size; i++) {
        printf("[DEBUG] DP arr index=%i, val=%i\n", i, dp_arr[i]);
    }

    printf("L=%i, R=%i, MAX=%i\n", l, r, max);

    free(dp_arr);
    return 1;
}