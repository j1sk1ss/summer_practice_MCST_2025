#include "../include/semantic.h"


static token_type_t _stack[STACK_SIZE] = { 0 };
static int _stack_pointer = 0;

static int _push(token_type_t c) {
    _stack[_stack_pointer++] = c;
    return 1;
}

static token_type_t _top() {
    return _stack[_stack_pointer - 1];
}

static token_type_t _pop() {
    return _stack[--_stack_pointer];
}

static int _is_empty() {
    return _stack_pointer == 0;
}

static int _is_logic_gate(token_type_t t) {
    if (t == AND_TOKEN || t == OR_TOKEN) return 1;
    return 0;
}

static int _is_variable(token_type_t t) {
    if (t == VARIABLE_TOKEN || t == CONSTANT_TOKEN) return 1;
    return 0;
}

static int _is_same_type(token_type_t a, token_type_t b) {
    return (_is_variable(a) && _is_variable(b)) || (_is_logic_gate(a) && _is_logic_gate(b));
}

static int _is_valid_sequence(token_type_t a, token_type_t b) {
    if (_is_same_type(a, b)) return 0;
    if ((_is_variable(a) && b == OPEN_BRACKET_TOKEN)) return 0;
    return 1;
}


/*
Return -1 if semantic correct
Return any positiove integer as position, were mistake found.
*/
int check_semantic(token_t* h) {
    int position = 0;
    while (h) {
        position++;
        if (h->next) {
            /*
            Check if we have pair of identical elements.
            Like var var, or and and and etc.
            Return -1 if we have pair.
            */
            if (!_is_valid_sequence(h->t_type, h->next->t_type)) {
                return position;
            }
        }

        if (h->t_type == OPEN_BRACKET_TOKEN) {
            _push(h->t_type);
        }
        else if (h->t_type == CLOSE_BRACKET_TOKEN) {
            if (_is_empty()) return position;
            if (_top() != OPEN_BRACKET_TOKEN) return position;
            _pop();
        }

        h = h->next;
    }

    return _is_empty() ? -1 : position;
}
