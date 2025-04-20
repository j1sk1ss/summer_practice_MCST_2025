#include "include/file_copy.h"


static int _fork_copy(int fd, char* bpath, char* spath) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork()");
        close(fd);
        return 0;
    }

    char resolved_path[128] = { 0 };
    if (realpath(bpath, resolved_path) == NULL) {
        perror("realpath");
        return 0;
    }
    
    const char* filename = strrchr(resolved_path, '/');
    filename = filename ? filename + 1 : resolved_path;

    char save_name[256] = { 0 };
    snprintf(save_name, 255, "%s%d_%s", spath, getpid(), filename);

    int new_fd = open(save_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (new_fd < 0) {
        perror("open for write");
        return 0;
    }

    ssize_t bytes = 0;
    char buf[4096] = { 0 };
    while ((bytes = pread(fd, buf, sizeof(buf), bytes)) > 0) {
        pwrite(new_fd, buf, bytes, 0);
    }

    close(new_fd);
    if (pid > 0) {
        waitpid(pid, NULL, 0);
    }

    int print_fd = open(save_name, O_RDONLY);
    if (print_fd < 0) {
        perror("open for read");
        return 0;
    }

    printf("PID %d created file '%s'. Contents:\n", getpid(), save_name);
    fflush(stdout);
    while ((bytes = read(print_fd, buf, sizeof(buf))) > 0) write(STDOUT_FILENO, buf, bytes);
    close(print_fd);

    if (!pid) _exit(EXIT_SUCCESS);
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No args provided. Usage: %s <path> <args (-o)>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* save_path = "";
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], OUTPUT_OPTION)) {
            if (i + 1 < argc) save_path = argv[i++ + 1];
        }
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "File not found. Path: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (!_fork_copy(fd, argv[1], save_path)) {
        fprintf(stderr, "File copy corrupted!\n");
        exit(EXIT_FAILURE);
    }

    close(fd);
    return EXIT_SUCCESS;
}