#include "include/parser.h"


static char* __elbrus_values[] = {
    "1c+", "2c+", "2c3", "4c", "8c", "16c", NULL
};

static option_t __options[] = {
    { .option = "m", .accept_values = NULL, .prefix = 1 },
    { .option = "c", .accept_values = NULL, .prefix = 1 },
    { .option = "s", .accept_values = NULL, .prefix = 1 },
    { .option = "t", .accept_values = NULL, .prefix = 1 },
    { .option = "elbrus", .accept_values = __elbrus_values, .prefix = 2 },
    { .option = NULL, .prefix = -1 }
};


/*
Is option function, check input arg for option status.
If arg is a single or combination of single chars, will return 1.
If arg is a long optin, will return non-zero prefix size.
If arg is not an option, will return 0.
If arg too small or not an option (Don't have prefix), will return -2 and -1.
*/
static int __is_option(char* option) {
    /*
    We know, that if input option has len lower then 2, 
    it looks like one symbol.
    */
    size_t option_size = strlen(option);
    if (option_size < 2) return -2;
    if (option[0] != OPTION_TOKEN) return -1;

    /*
    Count prefix size for faster compare
    in next part.
    */
    int option_prefix = 1;
    char* head = option + 1;
    while (*head && *head == OPTION_TOKEN) {
        option_prefix++; 
        head++;
    }
    
    if (option_prefix == 1) {
        /*
        This is a single option, that can be combined or not.
        */
        char* option_head = option + option_prefix;
        while (*option_head) {
            int is_valid_seq = 0;
            for (int i = 0; __options[i].option; i++) {
                if (__options[i].prefix != option_prefix) continue;
                if (*option_head == __options[i].option[0]) {
                    is_valid_seq = 1;
                    break;
                }
            }

            option_head++;
            if (!is_valid_seq) return 0;
        }

        return 1;
    }
    else if (option_prefix > 1) {
        /*
        This is a long option.
        */
        for (int i = 0; __options[i].option; i++) {
            if (__options[i].prefix != option_prefix) continue;
            if (!strncmp(option + __options[i].prefix, __options[i].option, strlen(__options[i].option))) return option_prefix;
        }

        return 0;
    }

    return 0;
}

static int __is_long_option(char* option, int prefix) {
    /*
    We move pointer from option head to option body.
    Then we move to the body end (End marked by ASIGN token).
    --option=
      |_body|
            |_body end
    */
    char* body = option + prefix;
    char* params = strchr(body, ASIGN_TOKEN);
    if (!params) return 0;

    option_t* opt = NULL;
    for (int i = 0; __options[i].option; i++) {
        if (__options[i].prefix != prefix) continue;
        if (strncmp(__options[i].option, body, params - body) == 0) {
            opt = &__options[i];
            break;
        }
    }

    if (!opt || !opt->accept_values) return 0;
    params += 1;
    while (*params) {
        int len = 0;
        char* sub_option = params;
        while (sub_option[len] && sub_option[len] != DELIMITER_TOKEN) {
            len++;
        }

        int valid = 0;
        for (int j = 0; opt->accept_values[j]; j++) {
            if (strncmp(params, opt->accept_values[j], len) == 0 &&
                strlen(opt->accept_values[j]) == len) {
                valid = 1;
                break;
            }
        }

        if (!valid) return 0;
        params += len;
        if (*params == DELIMITER_TOKEN) params++;
    }

    return prefix;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No args provided!\n");
        exit(1);
    }

    int current_option = 0, current_noption = 0;
    char* options[256] = { NULL };
    char* noptions[256] = { NULL };

    int is_valid = 1;
    for (int i = 1; i < argc; i++) {
        int valid = 1;
        int prefix = __is_option(argv[i]);
        if (prefix < 0) {
            noptions[current_noption++] = argv[i];
        }
        else {
            if (!prefix) valid = 0;
            else if (prefix > 1) valid = __is_long_option(argv[i], prefix);
            if (!valid) fprintf(stderr, "Invalid argument at %i index (%s).\n", i, argv[i]);
            else options[current_option++] = argv[i] + prefix;
        }

        is_valid = valid && is_valid;
    }

    printf("Options are correct: ");
    while (--current_option >= 0) printf("%s, ", options[current_option]);
    printf(" non-options: ");
    while (--current_noption >= 0) printf("%s%s", noptions[current_noption], current_noption - 1 >= 0 ? ", " : ".\n");

    return is_valid;
}
