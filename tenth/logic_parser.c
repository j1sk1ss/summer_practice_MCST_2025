#include "include/logic_parser.h"


static const char* _t_type_str(token_type_t type) {
    switch (type) {
        case AND_TOKEN:             return "and";
        case OR_TOKEN:              return "or";
        case NOT_TOKEN:             return "not";
        case OPEN_BRACKET_TOKEN:    return "(";
        case CLOSE_BRACKET_TOKEN:   return ")";
        case VARIABLE_TOKEN:        return "var";
        case CONSTANT_TOKEN:        return "const";
        default:                    return "?";
    }
}

static int _print_ltree(const tree_t* node, int depth) {
    if (!node) return 0;
    for (int i = 0; i < depth; ++i) printf("  ");
    printf("%s", _t_type_str(node->token->t_type));
    if (node->token->value) printf(" [%s]", node->token->value);
    printf("\n");

    if (node->left)  _print_ltree(node->left, depth + 1);
    if (node->right) _print_ltree(node->right, depth + 1);
    return 1;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Invalid count of args provided. Usage: %s \"<expression>\"\n <show_tree (--st)>", argv[0]);
        exit(EXIT_FAILURE);
    }

    int show_tree = 0;
    if (argc > 2) {
        if (!strcmp(argv[2], SHOW_TREE_OPTION)) show_tree = 1;
    }

    token_t* h = tokenize(argv[1]);
    if (!markup(h)) {
        fprintf(stderr, "Markup error!\n");
        exit(EXIT_FAILURE);
    }
    
    int sem_res = check_semantic(h);
    if (sem_res != -1) {
        fprintf(stderr, "Expression not correct! Check token at %i index\n", sem_res);
        exit(EXIT_FAILURE);
    }

    tree_t* r = generate_logic_tree(h);
    if (!r) {
        fprintf(stderr, "Logic tree gen error!\n");
        free_tokens(h);
        exit(EXIT_FAILURE);
    }

    if (show_tree) _print_ltree(r, 0);

    table_t t;
    if (create_table(&t, r)) {
        printf("Truth Table:\n");
        for (int i = 0; i < t.vars_count; ++i)
            printf("v%d ", i);

        printf("| result\n");
        for (int i = 0; i < t.vars_count; ++i)
            printf("---");

        printf("|--------\n");
        for (int i = 0; i < t.body_size; ++i) {
            for (int j = 0; j < t.vars_count; ++j) {
                int bit = (i >> (t.vars_count - j - 1)) & 1;
                printf(" %d ", bit);
            }

            printf("|   %d\n", t.body[i]);
        }
    }

    free_table(&t);
    free_tree(r);
    return 1;
}
