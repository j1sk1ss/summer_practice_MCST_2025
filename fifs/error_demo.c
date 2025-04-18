#include "include/error_demo.h"


int main(int argc, char* argv[]) {
    const char* filename = "something.abcdegklm";
    int fd = open(filename, O_RDONLY);

    int errlist_flag = 1, errno_flag = 1, perror_flag = 1;
    if (argc > 1) {
        errlist_flag = errno_flag = perror_flag = 0;
        for (int i = 1; i < argc; i++) {
            if (!strcmp(argv[i], ERRLIST_OPTION)) errlist_flag = 1;
            else if (!strcmp(argv[i], ERRNO_OPTION)) errno_flag = 1;
            else if (!strcmp(argv[i], PERROR_OPTION)) perror_flag = 1;
        }
    }

    if (fd < 0) {
        if (errno_flag) printf("Error code: %d\n", errno);
        if (perror_flag) perror("Error opening file");
        if (errlist_flag) {
            if (errno < sys_nerr) printf("sys_errlist: %s\n", sys_errlist[errno]);
            else printf("Out of sys_errlist\n");
        }
    } 
    else {
        fprintf(stderr, "No error! File [%s] exists!", filename);
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    return EXIT_SUCCESS;
}