#include "include/error_demo.h"


int main(int argc, char* argv[]) {
    const char* filename = "something.abc";
    int fd = open(filename, O_RDONLY);

    if (fd < 0) {
        printf("Error code: %d\n", errno);
        if (errno < sys_nerr) printf("sys_errlist: %s\n", sys_errlist[errno]);
        else printf("Out of sys_errlist\n");
        perror("Error opening file");
    } 
    else {
        close(fd);
    }
    
    return 1;
}