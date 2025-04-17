#ifndef PARSER_H_
#define PARSER_H_

#include "tokenizer.h"

typedef struct tree {
    struct token* token;
    struct tree* left;
    struct tree* right;
} tree_t;


tree_t* generate_logic_tree(token_t* tokens);
int free_tree(tree_t* r);

#endif