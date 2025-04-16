#include "include/matrix_mult.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No args provided! Usage ./%s <sig_type> (%s, %s) \n", argv[0], SIGACTION_TYPE, SIGNAL_TYPE);
        exit(EXIT_FAILURE);
    }

    /*
    Set type of registering handler.
    */
    int success = 1;
    if (!strcmp(argv[1], SIGACTION_TYPE)) {
        if (!setup_sigaction()) success = 0;
    }
    else if (!strcmp(argv[1], SIGNAL_TYPE)) {
        if (!setup_signal()) success = 0;
    }
    else {
        fprintf(stderr, "Unknown option! %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (!success) {
        fprintf(stderr, "Signal handler register error! %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /*
    Create two matrixs and fill them with random values.
    */
    matrix_t a, b, result;
    if (!new_matrix(&a, 1000, 1000)) {
        fprintf(stderr, "Matrix 'A' memory allocation error!\n");
        exit(EXIT_FAILURE);
    }

    if (!new_matrix(&b, 1000, 1000)) {
        free_matrix(&a);
        fprintf(stderr, "Matrix 'B' memory allocation error!\n");
        exit(EXIT_FAILURE);
    }

    fill_matrix_random(&a, 0, 9);
    fill_matrix_random(&b, 0, 9);

    /*
    Start multiplication.
    */
    printf("Starting matrix multiplication. Press Ctrl+C to test signal handling...\n");
    if (matrix_multiply(&a, &b, &result) != 1) {
        fprintf(stderr, "Matrix operation 'A' X 'B' error!\n");
        exit(EXIT_FAILURE);
    }

    printf("Multiplication complete:\n");
    print_matrix(&result, "%i\t", stdout);

    free_matrix(&a);
    free_matrix(&b);
    free_matrix(&result);

    return 0;
}