#ifndef COMM_H_
#define COMM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>

#define BUFFER_SIZE     512


int register_handler();
int chat_fd(int out_fd, int in_fd, const char* my_label, const char* remote_label);
int socket_comm();
int fifo_comm();

#endif