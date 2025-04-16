#include "../include/commander.h"


/*
Will save to tmp_char
*/
static int _to_lower(value_t* v, int dummy) {
    if (!v || !v->value) return 0;

    if (v->tmp_char) free(v->tmp_char);
    v->tmp_char = strdup(v->value);
    if (!v->tmp_char) return 0;

    for (char* p = v->value; *p; ++p) {
        *p = tolower((unsigned char)*p);
    }

    return 1;
}

/*
Will save to tmp_char
*/
static int _to_upper(value_t* v, int dummy) {
    if (!v || !v->value) return 0;

    if (v->tmp_char) free(v->tmp_char);
    v->tmp_char = strdup(v->value);
    if (!v->tmp_char) return 0;

    for (char* p = v->value; *p; ++p) {
        *p = toupper((unsigned char)*p);
    }

    return 1;
}

/*
Will save to tmp_int
*/
static int _by_char(value_t* v, int idx) {
    if (!v || !v->value) return 0;

    size_t len = strlen(v->value);
    if (len == 0) {
        v->tmp_int = 0;
        return 1;
    }

    int real_idx = idx;
    if (idx < 0) {
        real_idx = (int)len + idx;
        if (real_idx < 0) real_idx = 0;
    }
    else if ((size_t)idx >= len) real_idx = len - 1;
    v->tmp_int = (unsigned char)v->value[real_idx];
    return 1;
}

/*
Will save to tmp_int
*/
static int _count_upper(value_t* v, int dummy) {
    if (!v || !v->value) return 0;
    v->tmp_int = 0;
    for (char* p = v->value; *p; ++p) {
        if (isupper(*p)) v->tmp_int++;
    }

    return 1;
}

/*
Will save to tmp_int
*/
static int _count_lower(value_t* v, int dummy) {
    if (!v || !v->value) return 0;
    v->tmp_int = 0;
    for (char* p = v->value; *p; ++p) {
        if (islower(*p)) v->tmp_int++;
    }

    return 1;
}

/*
Will save to tmp_int
*/
static int _by_len(value_t* v, int dummy) {
    if (!v || !v->value) return 0;
    v->tmp_int = strlen(v->value);
    return 1;
}

/*
Will save to tmp_int
*/
static int _to_int(value_t* v, int dummy) {
    if (!v || !v->value) return 0;
    v->tmp_int = atoi(v->value);
    return 1;
}

/*
Will save to tmp_int
*/
static int _sum_chars(value_t* v, int dummy) {
    if (!v || !v->value) return 0;
    v->tmp_int = 0;
    for (char* p = v->value; *p; ++p) {
        v->tmp_int += *p;
    }

    return 1;
}

/*
Will save to tmp_int
*/
static int _sum_digits(value_t* v, int dummy) {
    if (!v || !v->value) return 0;
    v->tmp_int = 0;
    for (char* p = v->value; *p; ++p) {
        v->tmp_int += *p - '0';
    }

    return 1;
}

static int _go_throught_values(value_t* h, int (*op)(value_t*, int), int val) {
    value_t* head = h;
    while (head) {
        if (!op(head, val)) printf("[WARN] operation error via processing %s\n", head->value);
        head = head->next;
    }

    return 1;
}


int prepare_values(value_t* h, int offset, char* argv[], int argc, commander_result_t* res) {
    int sort_type = 0;
    for (int i = offset; i < argc; i++) {
        char* cmd = argv[i];
        if (!strcmp(cmd, LEN_COMMAND)) _go_throught_values(h, _by_len, DUMMY_VALUE);
        else if (!strcmp(cmd, CHAR_COMMAND)) {
            if (i + 1 < argc) i += 1;
            else return -1;
            _go_throught_values(h, _by_char, atoi(argv[i]));
        }
        else if (!strcmp(cmd, INT_COMMAND)) _go_throught_values(h, _to_int, DUMMY_VALUE);
        else if (!strcmp(cmd, CHARSUM_COMMAND)) _go_throught_values(h, _sum_chars, DUMMY_VALUE);
        else if (!strcmp(cmd, DIGSUM_COMMAND)) _go_throught_values(h, _sum_digits, DUMMY_VALUE);
        else if (!strcmp(cmd, COUNT_LOWER_COMMAND)) _go_throught_values(h, _count_lower, DUMMY_VALUE);
        else if (!strcmp(cmd, COUNT_UPPER_COMMAND)) _go_throught_values(h, _count_upper, DUMMY_VALUE);
        else if (!strcmp(cmd, TO_LOWER)) _go_throught_values(h, _to_lower, DUMMY_VALUE);
        else if (!strcmp(cmd, TO_UPPER)) _go_throught_values(h, _to_upper, DUMMY_VALUE);
        else if (!strcmp(cmd, ASCENDING_TYPE)) sort_type = 1;
        else if (!strcmp(cmd, DESCENDING_TYPE)) sort_type = 0;
    }

    res->sort_type = sort_type;
    res->h = h;

    return 1;
}
