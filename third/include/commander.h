#ifndef COMMANDER_H_
#define COMMANDER_H_

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "values.h"

#define DUMMY_VALUE 0xDEADBEEF

/*
Command syntax is bylen <type> (asc / desc).
*/
#define LEN_COMMAND         "bylen"
/*
Command syntax is bychar_at <index> <type> (asc / desc).
If character index out of bounds, will used closed one.
*/
#define CHAR_COMMAND        "bychar_at"
/*
Command syntax is toint <type> (asc / desc).
*/
#define INT_COMMAND         "toint"
/*
Command syntax is digsum <type> (asc / desc).
Command syntax is chsum <type> (asc / desc).
*/
#define DIGSUM_COMMAND      "digsum"
#define CHARSUM_COMMAND     "chsum"
/*
Command syntax is lcount <type> (asc / desc).
*/
#define COUNT_LOWER_COMMAND "lcount"
/*
Command syntax is ucount <type> (asc / desc).
*/
#define COUNT_UPPER_COMMAND "ucount"

/*
This commands will transform line to upper or lower case.
*/
#define TO_LOWER "tlow"
#define TO_UPPER "tupp"

/*
This commands will select type of sorting.
*/
#define ASCENDING_TYPE   "asc"
#define DESCENDING_TYPE  "desc"

typedef struct {
    value_t* h;
    int sort_type;
} commander_result_t;

#endif