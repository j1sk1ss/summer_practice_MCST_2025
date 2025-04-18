#include "include/line_sort.h"


static int _print_help() {
    printf("||========================================================\n"   );
    printf("|| Usage:\n"                                                    );
    printf("|| --bylen option calculate line len.\n"                        );

    printf(
        "|| %s\n|| \t%s\n|| \t%s\n",
        "--bychar_at <idx> option select char at index in len.",
        "If provide negative value, will take chars from end.",
        "Example: --bychar_at -1 will take last character from line."
    );

    printf("|| --toint option convert every line to int.\n"                 );
    printf("|| --letsum option get sum of all letters in line.\n"           );
    printf("|| --digsum option get sum of all digits in line.\n"            );
    printf("|| --chsum option get sum of all characters in line.\n"         );
    printf("|| --lcount option get count of lower characters in line.\n"    );
    printf("|| --ucount option get count of upper characters in line.\n"    );
    printf("|| --tlow option convert line to lower case.\n"                 );
    printf("|| --tupp option convert line to upper case.\n"                 );
    printf("|| --asc option select ascending sort type.\n"                  );
    printf("|| --desc option selct descending sort type.\n"                 );
    printf("|| -o option select save location.\n"                           );
    printf("||\n|| Example: ./prog <path> <args> -o <save>\n"               );
    printf("||========================================================\n"   );

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
                char tmpname[] = "line_sort_output_XXXXXX.txt";
                int out_fd = mkstemp(tmpname);
                if (out_fd > 0) f = fdopen(out_fd, "w");
                else {
                    fprintf(stderr, "mkstemp return negative FD!\n");
                    exit(EXIT_FAILURE);
                }
            }

            if (!f) {
                fprintf(stderr, "Can't open or create file for save!\n");
                exit(EXIT_FAILURE);
            }

            printf("Sorted lines:\n");
            for (value_t* c = non_sorted.h; c; c = c->next) {
                printf("%s\n", c->value);
                fprintf(f, "%s\n", c->value);
            }

            fclose(f);
        }
    }

    close(fd);
    if (non_sorted.h) free_values(non_sorted.h);
    if (non_sorted.output) free(non_sorted.output);
    return EXIT_SUCCESS;
}
