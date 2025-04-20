#include "../include/values.h"


value_t* generate_values(FILE* f) {
    if (!f) return NULL;

    value_t* head = NULL;
    value_t* tail = NULL;

    size_t len = 0;
    char* line = NULL;

    while (getline(&line, &len, f) != -1) {
        int has_visible = 0;
        for (char* p = line; *p; ++p) {
            if (!isspace((unsigned char)*p)) {
                has_visible = 1;
                break;
            }
        }

        if (!has_visible) continue;
        size_t line_len = strlen(line);
        if (line_len > 0 && line[line_len - 1] == '\n')
            line[line_len - 1] = '\0';

        value_t* node = malloc(sizeof(value_t));
        if (!node) {
            /*
            Error case.
            */
            value_t* tmp = head;
            while (tmp) {
                value_t* next = tmp->next;
                free(tmp->value);
                free(tmp);
                tmp = next;
            }

            fclose(f);
            free(line);
            return NULL;
        }

        node->tmp_int  = 0;
        node->tmp_char = NULL;
        node->next     = NULL;
        node->value    = strdup(line);

        if (!head) head = node;
        else tail->next = node;
        tail = node;
    }

    if (!line) free(line);
    return head;
}

int free_values(value_t* h) {
    while (h) {
        value_t* next = h->next;
        if (h->tmp_char) free(h->tmp_char);
        free(h->value);
        free(h);
        h = next;
    }
    
    return 1;
}
