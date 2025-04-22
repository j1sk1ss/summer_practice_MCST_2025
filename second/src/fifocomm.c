#include "../include/comm.h"


int _destroy_link(char* src, char* dst) {
    int fres = unlink(src);
    int sres = unlink(dst);
    if (!fres || !sres) printf("Try to unlink FIFO, if they exists: [%s %s]\n", src, dst);
    return 1;
}

int fifo_comm() {
    char src[64] = { 0 };
    char dst[64] = { 0 };
    sprintf(src, "%d_fifo_master", getpid());
    sprintf(dst, "%d_fifo_slave", getpid());
    
    _destroy_link(src, dst);
    if (mkfifo(src, 0666) == -1 || mkfifo(dst, 0666) == -1) {
        perror("mkfifo failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        _destroy_link(src, dst);
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    
    int write_fd = -1, read_fd = -1;
    if (pid > 0) {
        /*
        Master process 
        */
        write_fd = open(src, O_WRONLY);
        read_fd  = open(dst, O_RDONLY);
        if (write_fd == -1 || read_fd == -1) {
            _destroy_link(src, dst);
            perror("open failed");
            exit(EXIT_FAILURE);
        }
        
        chat_fd(write_fd, read_fd, "master", "slave");

        /*
        Only master unlink fifo channel.
        */
        _destroy_link(src, dst);
    } 
    else {
        /*
        Same as in socketcomm, this is a slave process.
        */
        read_fd  = open(src, O_RDONLY);
        write_fd = open(dst, O_WRONLY);
        if (write_fd == -1 || read_fd == -1) {
            _destroy_link(src, dst);
            perror("open failed");
            exit(EXIT_FAILURE);
        }

        chat_fd(write_fd, read_fd, "slave", "master");
    }

    close(write_fd);
    close(read_fd);

    return 1;
}