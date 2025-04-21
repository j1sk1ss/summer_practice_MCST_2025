#include "../include/tokenizer.h"


typedef enum {
    OPEN_BRACKET_CHAR,
    CLOSE_BRACKET_CHAR,
    DIGIT_CHAR,
    LETTER_CHAR,
    SPACE_CHAR,
    UNKNOWN_CHAR
} char_type_t;

static int _to_lower(char* val) {
    if (!val) return 0;
    for (char* p = val; *p; ++p) {
        *p = tolower((unsigned char)*p);
    }

    return 1;
}

static char_type_t _get_char_type(char c) {
    if (isdigit(c))         return DIGIT_CHAR;
    else if (isalpha(c))    return LETTER_CHAR;
    else if (c == '(')      return OPEN_BRACKET_CHAR;
    else if (c == ')')      return CLOSE_BRACKET_CHAR;
    else if (isspace(c))    return SPACE_CHAR;
    return UNKNOWN_CHAR;
}

static token_t* _create_token(char* value, token_type_t t_type) {
    token_t* token = (token_t*)malloc(sizeof(token_t));
    if (!token) return NULL;
    
    if (value) {
        char* lower  = strdup(value);
        token->value = lower;
        _to_lower(token->value);
    }

    token->next      = NULL;
    token->t_type    = t_type;
    token->val_index = -1;

    return token;
}


token_t* tokenize(char* src) {
    token_t* head = NULL;
    token_t* tail = NULL;

    while (*src) {
        char_type_t char_type = _get_char_type(*src);
        if (char_type != SPACE_CHAR) {
            int val_len = 0;
            char val_buffer[256] = { 0 };
            token_t* new_token = NULL;
            switch (char_type) {
                case OPEN_BRACKET_CHAR: new_token = _create_token(NULL, OPEN_BRACKET_TOKEN); src++; break;
                case CLOSE_BRACKET_CHAR: new_token = _create_token(NULL, CLOSE_BRACKET_TOKEN); src++; break;
                case DIGIT_CHAR:
                    while (_get_char_type(*src) == DIGIT_CHAR) val_buffer[val_len++] = *(src++);
                    new_token = _create_token(val_buffer, CONSTANT_TOKEN);
                break;
                case LETTER_CHAR:
                    while (_get_char_type(*src) == LETTER_CHAR) val_buffer[val_len++] = *(src++);
                    new_token = _create_token(val_buffer, UNKNOWN_VALUE);
                break;
                default: break;
            }

            if (new_token) {
                if (!head) head = tail = new_token;
                else {
                    tail->next = new_token;
                    tail = new_token;
                }
            }
        }
        else src++;
    }

    return head;
}

int free_tokens(token_t* head) {
    if (!head) return 0;
    while (head) {
        token_t* next = head->next;
        if (head->value) free(head->value);
        free(head);
        head = next;
    }

    return 1;
}
