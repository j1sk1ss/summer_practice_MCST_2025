#ifndef MATRIX_H_
#define MATRIX_H_

#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    int x;
    int y;
    unsigned int* body;
} matrix_t;


int setup_signal();
int setup_sigaction();

int new_matrix(matrix_t* mtrx, int x, int y);
int print_matrix(matrix_t* matrix, char* method, FILE* fd);
int fill_matrix_random(matrix_t* matrix, int min, int max);
int free_matrix(matrix_t* matrix);
int matrix_multiply(matrix_t* a, matrix_t* b, matrix_t* out);

#endif 