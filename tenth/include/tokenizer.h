#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    UNKNOWN_VALUE,
    OPEN_BRACKET_TOKEN,
    CLOSE_BRACKET_TOKEN,
    AND_TOKEN,
    OR_TOKEN,
    NOT_TOKEN,
    VARIABLE_TOKEN,
    CONSTANT_TOKEN
} token_type_t;

typedef struct token {
    char* value;
    int val_index;
    token_type_t t_type;
    struct token* next;
} token_t;


token_t* tokenize(char* src);
int free_tokens(token_t* head);

#endif