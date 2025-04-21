#include "../include/comm.h"


volatile sig_atomic_t stop = 0;

static void _handle_sigint(int sig) {
    stop = 1;
}

/*
We register handler of SIGINT for correct program stop.
Correct stop means FIFO realise and fd close.
*/
int register_handler() {
    struct sigaction sa;
    sa.sa_handler = _handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1) return 0;
    return 1;
}

int chat_fd(int out_fd, int in_fd, const char* my_label, const char* remote_label) {
    /*
    For working with select(2) we should create fd_set.
    Then, before check of I/O ready, we should refill it by two fd (stdin_fd and in_fd).
    */
    fd_set read_fds;
    char buffer[BUFFER_SIZE] = { 0 };
    int stdin_fd = fileno(stdin);

    while (!stop) {
        FD_ZERO(&read_fds);
        FD_SET(stdin_fd, &read_fds);
        FD_SET(in_fd, &read_fds);
        int max_fd = (stdin_fd > in_fd) ? stdin_fd : in_fd;
        
        /*
        Waiting any fd for ready for read operation.
        Future update can be in moving to poll(2) system call.
        */
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0) {
            if (!stop) perror("select error");
            break;
        }

        /*
        Before send we check, if stdin FD is ready (It can be not, if it occupied by another proccess).
        Sending data from keyboard input.
        */
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

        /*
        Receiving data from FD.
        We check in_fd is ready, and if it is, we read data from.
        */
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
