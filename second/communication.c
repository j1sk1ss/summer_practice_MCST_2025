#include "include/communication.h"


static void __chat_fd(int out_fd, int in_fd) {
    fd_set read_fds;
    char buffer[BUFFER_SIZE] = { 0 };
    int stdin_fd = fileno(stdin);

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(stdin_fd, &read_fds);
        FD_SET(in_fd, &read_fds);

        int max_fd = (stdin_fd > in_fd) ? stdin_fd : in_fd;
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select error");
            break;
        }

        if (FD_ISSET(stdin_fd, &read_fds)) {
            ssize_t bytes_read = read(stdin_fd, buffer, BUFFER_SIZE);
            if (bytes_read > 0) write(out_fd, buffer, bytes_read);
            else if (bytes_read == 0) break;
            else {
                perror("read stdin error");
                break;
            }
        }

        if (FD_ISSET(in_fd, &read_fds)) {
            ssize_t bytes_read = read(in_fd, buffer, BUFFER_SIZE);
            if (bytes_read > 0) {
                printf("Received: %.*s", (int)bytes_read, buffer);
                fflush(stdout);
            } 
            else if (bytes_read == 0) break;
            else {
                perror("read in_fd error");
                break;
            }
        }
    }
}

static int __socket_comm() {
    return 1;
}

static int __fifo_comm(char* src, char* dst) {
    unlink(src);
    unlink(dst);

    if (mkfifo(src, 0666) == -1 || mkfifo(dst, 0666) == -1) {
        perror("mkfifo failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    int write_fd = -1, read_fd = -1;
    if (pid > 0) {
        write_fd = open(src, O_WRONLY);
        read_fd  = open(dst, O_RDONLY);
        if (write_fd == -1 || read_fd == -1) {
            perror("open failed");
            exit(EXIT_FAILURE);
        }

        __chat_fd(write_fd, read_fd);
    } 
    else {
        write_fd = open(dst, O_WRONLY);
        read_fd  = open(src, O_RDONLY);
        if (read_fd == -1 || write_fd == -1) {
            perror("open failed");
            exit(EXIT_FAILURE);
        }

        __chat_fd(write_fd, read_fd);
    }

    close(write_fd);
    close(read_fd);
    unlink(src);
    unlink(dst);

    return 1;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No args provided! Usage ./prog <type>\n");
        exit(1);
    }

    char* comm_type = argv[1];
    if (!strcmp(comm_type, FIFO_TYPE)) {
        if (argc < 4) {
            fprintf(stderr, "No args provided! Usage ./prog fifo <src> <dst>\n");
            exit(1);
        }

        char* fifo_src = argv[2];
        char* fifo_dst = argv[3];
        __fifo_comm(fifo_src, fifo_dst);
    }
    else if (!strcmp(comm_type, SOCKET_TYPE)) {

    }
    else {
        fprintf(stderr, "Uknown option %s\n", comm_type);
        exit(1);
    }

    return 1;
}