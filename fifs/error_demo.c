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
            else fprintf(stderr, "Unknown option [%s] at index [%i]\n", argv[i], i);
        }
    }

    if (fd < 0) {
        if (errno_flag) fprintf(stderr, "Error code: %d\n", errno);
        if (perror_flag) perror("Error opening file");
        if (errlist_flag) {
/*
sys_errlist and sys_nerr was removed from glibc since 2.32 version.
Here we check glibc version. Also, i can note, that this vars are defined
in macOsx and linux alpine in WSL.
*/
#if defined(__GLIBC__) && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 32))
            if (errno < sys_nerr) fprintf(stderr, "sys_errlist: %s\n", sys_errlist[errno]);
            else fprintf(stderr, "Out of sys_errlist\n");
#else
            fprintf(stderr, "strerror: %s\n", strerror(errno));
#endif
        }
    } 
    else {
        fprintf(stderr, "No error! File [%s] exists!", filename);
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    return EXIT_SUCCESS;
}