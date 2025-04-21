#include "../include/parser.h"


static tree_t* _parse_factor(token_t**);

static tree_t* _create_node(token_t* token, tree_t* left, tree_t* right) {
    tree_t* node = malloc(sizeof(tree_t));
    if (!node) return NULL;

    node->token = token;
    node->left  = left;
    node->right = right;
    return node;
}

static tree_t* _parse_term(token_t **tokens) {
    tree_t *left = _parse_factor(tokens);
    while (*tokens && (*tokens)->t_type == AND_TOKEN) {
        token_t *op = *tokens;
        *tokens = (*tokens)->next;
        tree_t *right = _parse_factor(tokens);
        left = _create_node(op, left, right);
    }

    return left;
}

static tree_t* _parse_expression(token_t** tokens) {
    tree_t* left = _parse_term(tokens);
    while (*tokens && (*tokens)->t_type == OR_TOKEN) {
        token_t* op = *tokens;
        *tokens = (*tokens)->next;
        tree_t* right = _parse_term(tokens);
        left = _create_node(op, left, right);
    }

    return left;
}

static tree_t* _parse_factor(token_t** tokens) {
    if (!*tokens) return NULL;

    tree_t* node = NULL;
    token_t* current = *tokens;

    switch (current->t_type) {
        case NOT_TOKEN:
            *tokens = current->next;
            tree_t* child = _parse_factor(tokens);
            node = _create_node(current, NULL, child);
            if (!node) {
                free_tree(node);
                return NULL;
            }
        break;
        
        case OPEN_BRACKET_TOKEN:
            *tokens = current->next;
            node = _parse_expression(tokens);
            if ((*tokens && (*tokens)->t_type != CLOSE_BRACKET_TOKEN) || !node) {
                free_tree(node);
                return NULL;
            }

            *tokens = (*tokens)->next;
        break;
        
        case VARIABLE_TOKEN:
        case CONSTANT_TOKEN:
            node = _create_node(current, NULL, NULL);
            if (!node) {
                free_tree(node);
                return NULL;
            }
            
            *tokens = current->next;
        break;
        default: break;
    }

    return node;
}

tree_t* generate_logic_tree(token_t* tokens) {
    token_t* current_pos = tokens;
    tree_t* root = _parse_expression(&current_pos);

    /*
    We not at the end of tokens. That indicates about error in _parse_expression.
    */
    if (current_pos) {
        free_tree(root);
        root = NULL;
    }

    return root;
}

int free_tree(tree_t* r) {
    if (!r) return 0;
    free_tree(r->left);
    free_tree(r->right);
    free(r);

    return 1;
}
