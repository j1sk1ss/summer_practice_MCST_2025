#include "../include/truth_table.h"


static void _find_max_index(const tree_t* node, int* max_index) {
    if (!node) return;
    if (node->token->t_type == VARIABLE_TOKEN && node->token->val_index > *max_index) *max_index = node->token->val_index;
    _find_max_index(node->left, max_index);
    _find_max_index(node->right, max_index);
}

static int _get_table_value(const tree_t* node, const int* values) {
    if (!node) return 0;
    switch (node->token->t_type) {
        case CONSTANT_TOKEN:    return strcmp(node->token->value, "1") == 0 ? 1 : 0;
        case VARIABLE_TOKEN:    return values[node->token->val_index];
        case NOT_TOKEN:         return !_get_table_value(node->right, values);
        case AND_TOKEN:         return _get_table_value(node->left, values) && _get_table_value(node->right, values);
        case OR_TOKEN:          return _get_table_value(node->left, values) || _get_table_value(node->right, values);
        default:                return 0;
    }
}

int create_table(table_t* t, const tree_t* r) {
    if (!t || !r) return 0;

    int max_index = -1;
    _find_max_index(r, &max_index);
    int var_count = max_index + 1;

    int row_count = 1 << var_count;
    int* body = malloc(sizeof(int) * row_count);
    if (!body) return 0;

    int* values = (int*)malloc(var_count * sizeof(int));
    if (!values) {
        free(body);
        return 0;
    }

    for (int i = 0; i < row_count; ++i) {
        for (int j = 0; j < var_count; ++j) values[j] = (i >> (var_count - j - 1)) & 1;
        body[i] = _get_table_value(r, values);
    }

    free(values);

    t->body = body;
    t->body_size = row_count;
    t->vars_count = var_count;

    return 1;
}

int free_table(table_t* t) {
    if (t->body) free(t->body);
    return 1;
}
