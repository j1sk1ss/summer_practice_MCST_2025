#ifndef VALUES_H_
#define VALUES_H_

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/*
Representation of line in file.
We allocate tmp for temporary results.
*/
typedef struct value {
    char* value;
    char* tmp_char;
    int tmp_int;
    struct value* next;
} value_t;


value_t* generate_values(int fd);
int free_values(value_t* h);

#endif