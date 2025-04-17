#ifndef MARKUP_H_
#define MARKUP_H_

#include <string.h>
#include "tokenizer.h"

#define AND_MARKUP  "and"
#define OR_MARKUP   "or"
#define NOT_MARKUP  "not"

int markup(token_t* h);

#endif