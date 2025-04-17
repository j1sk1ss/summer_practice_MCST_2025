#include "../include/commander.h"


/*
Will save to tmp_char
*/
static int _to_lower(value_t* v, int dummy) {
    if (!v || !v->value) return 0;

    if (v->tmp_char) free(v->tmp_char);
    v->tmp_char = strdup(v->value);
    if (!v->tmp_char) return 0;

    for (char* p = v->tmp_char; *p; ++p) {
        unsigned char ch = (unsigned char)*p;
        if (ch >= 0xC0 && ch <= 0xDF) *p = *p + 32; // cp1251
        else *p = tolower((unsigned char)*p);
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

    for (char* p = v->tmp_char; *p; ++p) {
        unsigned char ch = (unsigned char)*p;
        if (ch >= 0xE0 && ch <= 0xFF) *p = *p - 32; // cp1251
        else *p = toupper((unsigned char)*p);
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
    char* val = v->tmp_char ? v->tmp_char : v->value;
    if (idx < 0) {
        real_idx = (int)len + idx;
        if (real_idx < 0) real_idx = 0;
    }
    else if ((size_t)idx >= len) real_idx = len - 1;
    v->tmp_int = (unsigned char)val[real_idx];
    return 1;
}

/*
Will save to tmp_int
*/
static int _count_upper(value_t* v, int dummy) {
    if (!v || !v->value) return 0;
    v->tmp_int = 0;
    char* val = v->tmp_char ? v->tmp_char : v->value;
    for (char* p = val; *p; ++p) {
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
    char* val = v->tmp_char ? v->tmp_char : v->value;
    for (char* p = val; *p; ++p) {
        if (islower(*p)) v->tmp_int++;
    }

    return 1;
}

/*
Will save to tmp_int
*/
static int _by_len(value_t* v, int dummy) {
    if (!v || !v->value) return 0;
    char* val = v->tmp_char ? v->tmp_char : v->value;
    v->tmp_int = strlen(val);
    return 1;
}

/*
Will save to tmp_int
*/
static int _to_int(value_t* v, int dummy) {
    if (!v || !v->value) return 0;
    char* val = v->tmp_char ? v->tmp_char : v->value;
    v->tmp_int = atoi(val);
    return 1;
}

/*
Will save to tmp_int
*/
static int _sum_chars(value_t* v, int dummy) {
    if (!v || !v->value) return 0;
    char* val = v->tmp_char ? v->tmp_char : v->value;
    for (char* p = val; *p; ++p) {
        v->tmp_int += *p;
    }

    return 1;
}

/*
Will save to tmp_int
*/
static int _sum_digits(value_t* v, int dummy) {
    if (!v || !v->value) return 0;
    char* val = v->tmp_char ? v->tmp_char : v->value;
    for (char* p = val; *p; ++p) {
        if (isdigit(*p)) v->tmp_int += *p - '0';
    }

    return 1;
}

static int _go_through_values(value_t* h, commander_command_t* cmds) {
    while (h) {
        commander_command_t* cur_comm = cmds;
        while (cur_comm) {
            if (!cur_comm->op(h, cur_comm->dummy)) printf("[WARN] operation error via processing %s\n", h->value);
            cur_comm = cur_comm->next;
        }

        h = h->next;
    }

    return 1;
}

static int _free_commands(commander_command_t* cmd) {
    while (cmd) {
        commander_command_t* next = cmd->next;
        free(cmd);
        cmd = next;
    }

    return 1;
}

int prepare_values(value_t* h, int offset, char* argv[], int argc, commander_result_t* res) {
    int sort_type = 0;
    commander_command_t* head = NULL;
    commander_command_t* tail = NULL;

    for (int i = offset; i < argc; i++) {
        char* cmd = argv[i];
        int val = DUMMY_VALUE;
        int (*op)(value_t*, int) = NULL;

        if (!strcmp(cmd, LEN_COMMAND)) op = _by_len;
        else if (!strcmp(cmd, CHAR_COMMAND)) {
            if (i + 1 < argc) i += 1;
            else goto error;
            op  = _by_char;
            val = atoi(argv[i]);
        }
        else if (!strcmp(cmd, INT_COMMAND))         op = _to_int;
        else if (!strcmp(cmd, CHARSUM_COMMAND))     op = _sum_chars;
        else if (!strcmp(cmd, DIGSUM_COMMAND))      op = _sum_digits;
        else if (!strcmp(cmd, COUNT_LOWER_COMMAND)) op = _count_lower;
        else if (!strcmp(cmd, COUNT_UPPER_COMMAND)) op = _count_upper;
        else if (!strcmp(cmd, TO_LOWER))            op = _to_lower;
        else if (!strcmp(cmd, TO_UPPER))            op = _to_upper;
        else if (!strcmp(cmd, OUTPUT)) {
            if (i + 1 < argc) i += 1;
            else goto error;
            res->output = strdup(argv[i]);
        }
        else if (!strcmp(cmd, ASCENDING_TYPE))  sort_type = 1;
        else if (!strcmp(cmd, DESCENDING_TYPE)) sort_type = 0;

        if (op) {
            commander_command_t* new_cmd = malloc(sizeof(commander_command_t));
            if (!new_cmd) {
                fprintf(stderr, "[ERROR] Memory allocation for command failed\n");
                goto error;
            }

            new_cmd->op    = op;
            new_cmd->dummy = val;
            new_cmd->next  = NULL;

            if (!head) head = new_cmd;
            else tail->next = new_cmd;
            tail = new_cmd;
        }
    }

    if (!_go_through_values(h, head)) {
        fprintf(stderr, "[ERROR] Command application failed\n");
        goto error;
    }

    _free_commands(head);
    res->sort_type = sort_type;
    res->h = h;

    return 1;

error:
    _free_commands(head);
    return -1;
}
