#include "include/file_copy.h"


static int _fork_copy(int fd, char* bpath) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork()");
        close(fd);
        return 0;
    }

    char resolved_path[128] = { 0 };
    if (realpath(bpath, resolved_path) == NULL) {
        perror("realpath");
        return 1;
    }
    
    const char* filename = strrchr(resolved_path, '/');
    filename = filename ? filename + 1 : resolved_path;

    char save_name[128] = { 0 };
    sprintf(save_name, "%d_%s", getpid(), filename);

    int new_fd = open(save_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (new_fd < 0) {
        perror("open for write");
        return 1;
    }

    ssize_t bytes = 0;
    char buf[4096] = { 0 };
    while ((bytes = pread(fd, buf, sizeof(buf), bytes)) > 0) {
        pwrite(new_fd, buf, bytes, 0);
    }

    close(new_fd);
    if (!pid) _exit(0);
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No args provided. Usage: ./prog <path>\n");
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "File not find. Path: %s\n", argv[1]);
        exit(1);
    }

    if (!_fork_copy(fd, argv[1])) {
        fprintf(stderr, "File copy corrupted!\n");
        exit(1);
    }

    close(fd);

    return 1;
}