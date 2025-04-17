#include "include/logic_parser.h"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid count of args provided. Usage: %s \"<expression>\"\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    token_t* h = tokenize(argv[1]);
    if (!markup(h)) {
        /*
        Creating syntax tree
        */
    }

    free_tokens(h);
    return 1;
}
