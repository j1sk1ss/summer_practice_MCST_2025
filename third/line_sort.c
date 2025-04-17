#include "include/line_sort.h"


static int _print_help() {
    printf("Usage:\n");
    printf("1) --bylen option calculate line len.\n");

    printf(
        "2) %s\n\t%s\n\t%s\n",
        "--bychar_at <idx> option select char at index in len.",
        "If provide negative value, will take chars from end.",
        "Example: --bychar_at -1 will take last character from line."
    );

    printf("3) --toint option convert every line to int.\n");
    printf("4) --digsum option get sum of all digits in line.\n");
    printf("5) --chsum option get sum of all characters in line.\n");
    printf("6) --lcount option get count of lower characters in line.\n");
    printf("7) --ucount option get count of upper characters in line.\n");
    printf("8) --tlow option convert line to lower case.\n");
    printf("9) --tupp option convert line to upper case.\n");
    printf("10) --asc option select ascending sort type.\n");
    printf("11) --desc option selct descending sort type.\n");
    printf("12) -o option select save location.\n");

    return 1;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        _print_help();
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "File [%s] not found\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    value_t* values = generate_values(fd);
    if (!values) {
        fprintf(stderr, "Values gen error!\n");
        exit(EXIT_FAILURE);
    }

    commander_result_t non_sorted = { .h = NULL, .output = NULL, .sort_type = 0 };
    if (prepare_values(values, 2, argv, argc, &non_sorted) != 1) fprintf(stderr, "Values process error!\n");
    else {
        if (!sorter_sort(&non_sorted)) fprintf(stderr, "Values sorting error!\n");
        else {
            /*
            Preparing file new file for writing.
            */
            FILE* f = NULL;
            char* path = non_sorted.output;
            if (path) f = fopen(path, "w");
            else {
                char tmpname[] = "line_sort_output_XXXXXX";
                int fd = mkstemp(tmpname);
                if (fd > 0) f = fdopen(fd, "w");
            }

            if (!f) {
                fprintf(stderr, "Can't open or create file for save!\n");
                exit(EXIT_FAILURE);
            }

            for (value_t* c = non_sorted.h; c; c = c->next) {
                printf("%s\n", c->tmp_char ? c->tmp_char : c->value);
                fprintf(f, "%s\n", c->tmp_char ? c->tmp_char : c->value);
            }

            fclose(f);
        }
    }

    close(fd);
    free_values(non_sorted.h);
    return 1;
}
