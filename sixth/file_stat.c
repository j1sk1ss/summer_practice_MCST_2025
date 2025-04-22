#include "include/file_stat.h"


void _print_file_type(mode_t mode) {
    printf("|| Type: ");
    if (S_ISREG(mode))        printf("default file");
    else if (S_ISDIR(mode))   printf("catalog");
    else if (S_ISCHR(mode))   printf("symbol device");
    else if (S_ISBLK(mode))   printf("block device");
    else if (S_ISFIFO(mode))  printf("FIFO");
    else if (S_ISLNK(mode))   printf("symlink");
    else if (S_ISSOCK(mode))  printf("socket");
    else                      printf("unknown");
    printf("\n");
}

void _print_permissions(mode_t mode) {
    printf("|| Permissions: ");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
    printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No args provided. Usage: %s <path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct stat file_stat;
    const char* filename = argv[1];
    if (stat(filename, &file_stat) != 0) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    printf("\n|=================================================\n"      );
    printf("|| File: %s\n",            filename                          );
    _print_file_type(file_stat.st_mode);
    _print_permissions(file_stat.st_mode);
    printf("|| Size: %ld bytes\n",     (long int)file_stat.st_size       );
    printf("|| inode: %lu\n",          (unsigned long)file_stat.st_ino   );
    printf("|| Count links: %lu\n",    (unsigned long)file_stat.st_nlink );
    printf("|| Last changed: %s",      ctime(&file_stat.st_mtime)        );
    printf("|=================================================\n\n"      );

    return EXIT_SUCCESS;
}
