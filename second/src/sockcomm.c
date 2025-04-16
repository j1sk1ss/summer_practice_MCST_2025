#include "../include/comm.h"


int socket_comm() {
    struct sockaddr_un addr;
    int server_fd = 0, client_fd = 0, sock_fd = 0;

    char socket_path[] = "/tmp/socket_XXXXXX";
    if (mkstemp(socket_path) == -1) {
        perror("mkstemp failed");
        exit(EXIT_FAILURE);
    }

    unlink(socket_path);

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind failed");
        close(server_fd);
        unlink(socket_path);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) == -1) {
        perror("listen failed");
        close(server_fd);
        unlink(socket_path);
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        close(server_fd);
        unlink(socket_path);
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        /*
        Server logic part.
        */
        client_fd = accept(server_fd, NULL, NULL);
        if (client_fd == -1) {
            perror("accept failed");
            close(server_fd);
            unlink(socket_path);
            exit(EXIT_FAILURE);
        }

        close(server_fd);
        unlink(socket_path);

        chat_fd(client_fd, client_fd, "server", "client");
        close(client_fd);
    } 
    else {
        /*
        Client logic part. Before start, we wait until server starts.
        */
        sleep(1);

        sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sock_fd == -1) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        if (connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            perror("connect failed");
            close(sock_fd);
            exit(EXIT_FAILURE);
        }

        chat_fd(sock_fd, sock_fd, "client", "server");
        close(sock_fd);
    }

    return 1;
}