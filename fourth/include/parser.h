#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define ELBRUS_OPTION   "elbrus"
#define SHORT_OPTIONS   "mcst"

typedef struct {
    char* option;
    int (*validate)(const char*);
} option_t;

#endif