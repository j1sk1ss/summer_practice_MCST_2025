#ifndef TRUTH_TABLE_H_
#define TRUTH_TABLE_H_

#include <string.h>
#include "parser.h"

typedef struct {
    int* body;
    int body_size;
    int vars_count;
} table_t;


int create_table(table_t* t, const tree_t* r);
int free_table(table_t* t);

#endif