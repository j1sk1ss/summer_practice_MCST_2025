#include "include/parser.h"


int _get_prefix_length(const char* arg) {
    size_t arg_size = strlen(arg);
    if (arg_size >= 2 && (arg[0] == '-' && arg[1] == '-')) return 2;
    if (arg_size >= 1 && arg[0] == '-') return 1;
    return 0;
}

static const char* _elbrus_values[] = {
    "1c+", "2c+", "2c3", "4c", "8c", "16c", NULL
};

static int _validate_elbrus(const char* arg) {
    for (int i = 0; _elbrus_values[i]; i++) {
        if (strcmp(arg, _elbrus_values[i]) == 0) return 1;
    }

    return 0;
}

static option_t _options[] = {
    { .option = ELBRUS_OPTION, .validate = _validate_elbrus },
    { .option = NULL }
};


int main(int argc, char* argv[]) {
    int opt = 0;
    int option_index = 0;

    static struct option long_options[] = {
        { ELBRUS_OPTION, required_argument, 0, 0 },
        { 0, 0, 0, 0 }
    };

    int current_option = 0, current_noption = 0;
    char* options[256] = { NULL };
    char* noptions[256] = { NULL };

    while ((opt = getopt_long(argc, argv, SHORT_OPTIONS, long_options, &option_index)) != -1) {
        int valid = 1;
        switch (opt) {
            case 0:
                for (int i = 0; _options[i].option; i++) {
                    if (strcmp(long_options[option_index].name, _options[i].option) == 0) {
                        if (!_options[i].validate(optarg)) valid = 0;
                        else options[current_option++] = strdup(argv[optind - 1] + _get_prefix_length(argv[optind - 1]));
                    }
                }

                break;
            case 'm': options[current_option++] = strdup("m"); break;
            case 'c': options[current_option++] = strdup("c"); break;
            case 's': options[current_option++] = strdup("s"); break;
            case 't': options[current_option++] = strdup("t"); break;
            case '?': valid = 0; break;
            default: abort();
        }

        if (!valid) fprintf(stderr, "Invalid argument at %i index (%s).\n", optind - 1, argv[optind - 1]);
    }

    for (int i = optind; i < argc; i++) {
        noptions[current_noption++] = argv[i];
    }

    printf("Options are correct: ");
    for (int i = 0; i < current_option; i++) printf("%s, ", options[i]);
    printf("non-options: ");
    for (int i = 0; i < current_noption; i++) printf("%s%s", noptions[i], i + 1 < current_noption ? ", " : ".\n");

    return 0;
}
