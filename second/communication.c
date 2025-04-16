#include "include/communication.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No args provided! Usage ./prog <type>\n");
        exit(EXIT_FAILURE);
    }

    register_handler();
    char* comm_type = argv[1];
    if (!strcmp(comm_type, FIFO_TYPE)) fifo_comm();
    else if (!strcmp(comm_type, SOCKET_TYPE)) socket_comm();
    else {
        fprintf(stderr, "Unknown option %s\n", comm_type);
        exit(EXIT_FAILURE);
    }

    return 1;
}