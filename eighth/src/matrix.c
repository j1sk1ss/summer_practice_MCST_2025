#include "../include/matrix.h"


volatile sig_atomic_t _sigint_received = 0;
volatile sig_atomic_t _sigint_count    = 0;

volatile int _current_i = -1;
volatile int _current_j = -1;


static void _sigint_handler(int signo) {
    if (_sigint_count++ == 0) {
        printf("\n[SIGINT] Caught SIGINT. Current indices: i=%d, j=%d\n", _current_i, _current_j);
        fflush(stdout);
        _sigint_received = 1;
    } 
    else {
        printf("\n[SIGINT] Second SIGINT received. Exiting. Current indices: i=%d, j=%d\n", _current_i, _current_j);
        exit(EXIT_FAILURE);
    }
}

/*
Return 1 if success setup.
Return 0 if something goes wrong.
*/
int setup_sigaction() {
    struct sigaction sa;
    sa.sa_handler = _sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    return !sigaction(SIGINT, &sa, NULL);
}

/*
Return 1 if success setup.
Return 0 if something goes wrong.
*/
int setup_signal() {
    return signal(SIGINT, _sigint_handler) != SIG_ERR;
}

int new_matrix(matrix_t* mtrx, int x, int y) {
    unsigned int* matrix_raw = (unsigned int*)malloc(x * y * sizeof(unsigned int));
    if (!matrix_raw) return 0;

    for (int i = 0; i < x * y; i++)matrix_raw[i] = 0;
    mtrx->body = matrix_raw;
    mtrx->x = x;
    mtrx->y = y;

    return 1;
}

int print_matrix(matrix_t* matrix, char* method, FILE* fd) {
    for (int i = 0; i < matrix->x; i++) {
        for (int j = 0; j < matrix->y; j++) fprintf(fd, method, matrix->body[i * matrix->y + j]);
        fprintf(fd, "\n");
    }

    return 1;
}

int fill_matrix_random(matrix_t* matrix, int min, int max) {
    srand(time(0));
    for (int i = 0; i < matrix->x; i++) {
        for (int j = 0; j < matrix->y; j++) {
            matrix->body[i * matrix->y + j] = min + rand() % (max - min + 1);
        }
    }

    return 1;
}

int free_matrix(matrix_t* matrix) {
    free(matrix->body);
    return 1;
}

int matrix_multiply(matrix_t* a, matrix_t* b, matrix_t* out) {
    if (a->y != b->x) return -2;
    if (!new_matrix(out, a->x, b->y)) return -1;
    for (_current_i = 0; _current_i < a->x; _current_i++) {
        for (_current_j = 0; _current_j < b->y; _current_j++) {
            unsigned int sum = 0;
            for (int k = 0; k < a->y; k++) sum += a->body[_current_i * a->y + k] * b->body[k * b->y + _current_j];
            out->body[_current_i * b->y + _current_j] = sum;
            sleep(1);
        }
    }

    return 1;
}