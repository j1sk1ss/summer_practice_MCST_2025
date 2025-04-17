#include "../include/markup.h"


typedef struct var_map {
    char* name;
    int index;
    struct var_map* next;
} var_map_t;

static var_map_t* _var_map_head = NULL;

typedef struct {
    char* value;
    token_type_t t_type;
} markuper_t;

static markuper_t _markups[] = {
    { .value = AND_MARKUP,  .t_type = AND_TOKEN     },
    { .value = OR_MARKUP,   .t_type = OR_TOKEN      },
    { .value = NOT_MARKUP,  .t_type = NOT_TOKEN     },
    { .value = NULL,        .t_type = UNKNOWN_VALUE }
};

markuper_t* _get_markup(token_t* token) {
    if (!token->value) return NULL;
    for (int i = 0; _markups[i].value; i++) {
        if (!strcmp(token->value, _markups[i].value)) return &_markups[i];
    }

    return NULL;
}

static int _get_add_variable(const char* name) {
    var_map_t* current = _var_map_head;
    while (current) {
        if (!strcmp(current->name, name)) return current->index;
        current = current->next;
    }

    static int current_index = 0;
    var_map_t* new_entry = malloc(sizeof(var_map_t));
    if (!new_entry) return -1;

    new_entry->name  = strdup(name);
    new_entry->index = current_index++;
    new_entry->next  = _var_map_head;
    _var_map_head    = new_entry;

    return new_entry->index;
}

static void _free_var_map() {
    var_map_t* current = _var_map_head;
    while (current) {
        var_map_t* next = current->next;
        free(current->name);
        free(current);
        current = next;
    }

    _var_map_head = NULL;
}


int markup(token_t* h) {
    if (!h) return 0;
    while (h) {
        if (h->value) {
            printf("%s\n", h->value);
            markuper_t* mrkp = _get_markup(h);
            if (mrkp) h->t_type = mrkp->t_type;
            if (h->t_type == UNKNOWN_VALUE) {
                h->t_type    = VARIABLE_TOKEN;
                h->val_index = _get_add_variable(h->value);
            }
        }

        h = h->next;
    }

    _free_var_map();
    return 1;
}
