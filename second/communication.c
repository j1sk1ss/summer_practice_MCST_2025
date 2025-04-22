#include "include/communication.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No args provided! Usage %s <type> (%s, %s)\n", argv[0], FIFO_TYPE, SOCKET_TYPE);
        exit(EXIT_FAILURE);
    }

    register_handler();
    char* comm_type = argv[1];
    if (!strcmp(comm_type, FIFO_TYPE)) fifo_comm();
    else if (!strcmp(comm_type, SOCKET_TYPE)) socket_comm();
    else {
        fprintf(stderr, "Unknown option %s\nUse %s or %s instead.\n", comm_type, FIFO_TYPE, SOCKET_TYPE);
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}