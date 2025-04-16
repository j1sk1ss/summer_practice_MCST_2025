#include "../include/comm.h"


volatile sig_atomic_t stop = 0;

void _handle_sigint(int sig) {
    stop = 1;
}

int register_handler() {
    signal(SIGINT, _handle_sigint);
    return 1;
}

int chat_fd(int out_fd, int in_fd, const char* my_label, const char* remote_label) {
    fd_set read_fds;
    char buffer[BUFFER_SIZE] = { 0 };
    int stdin_fd = fileno(stdin);

    while (!stop) {
        FD_ZERO(&read_fds);
        FD_SET(stdin_fd, &read_fds);
        FD_SET(in_fd, &read_fds);
        int max_fd = (stdin_fd > in_fd) ? stdin_fd : in_fd;
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0) {
            if (!stop) perror("select error");
            break;
        }

        if (FD_ISSET(stdin_fd, &read_fds)) {
            ssize_t bytes_read = read(stdin_fd, buffer, BUFFER_SIZE);
            if (bytes_read > 0) {
                printf("[%s/%i] Sent: %.*s", my_label, getpid(), (int)bytes_read, buffer);
                fflush(stdout);
                write(out_fd, buffer, bytes_read);
            } 
            else if (bytes_read == 0) break;
            else {
                if (!stop) perror("read stdin error");
                break;
            }
        }

        if (FD_ISSET(in_fd, &read_fds)) {
            ssize_t bytes_read = read(in_fd, buffer, BUFFER_SIZE);
            if (bytes_read > 0) {
                printf("[%s/%i] Received from %s: %.*s", my_label, getpid(), remote_label, (int)bytes_read, buffer);
                fflush(stdout);
            } 
            else if (bytes_read == 0) break;
            else {
                if (!stop) perror("read in_fd error");
                break;
            }
        }
    }

    return 1;
}
