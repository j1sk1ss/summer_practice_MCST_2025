#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPTION_TOKEN    '-'
#define ASIGN_TOKEN     '='
#define DELIMITER_TOKEN ','

typedef struct {
    char* option;
    int prefix;
    char** accept_values;
} option_t;


#endif