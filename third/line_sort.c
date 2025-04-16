#include "include/line_sort.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No args provided! Usage: ./%s <path_to_file> <args> ... \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "File [%s] not found\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    value_t* values = generate_values(fd);
    if (!values) {
        fprintf(stderr, "Values gen error!\n");
        exit(EXIT_FAILURE);
    }

    commander_result_t non_sorted;
    if (prepare_values(values, 2, argv, argc, &non_sorted) == 1) {
        sorter_sort(&non_sorted);
        for (value_t* c = non_sorted.h; c; c = c->next) {
            printf("Value: %s, processed: %s, int: %i (char %c)\n", c->value, c->tmp_char, c->tmp_int, c->tmp_int);
        }
    }
    else {
        fprintf(stderr, "Values process error!\n");
    }

    close(fd);
    free_values(non_sorted.h);
    return 1;
}
