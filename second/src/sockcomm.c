#include "../include/comm.h"


int socket_comm() {
    struct sockaddr_un addr;
    int server_fd = 0, client_fd = 0, sock_fd = 0;

    char socket_name[64] = { 0 };
    sprintf(socket_name, "%d_socket", getpid());

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_name, sizeof(addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind failed");
        close(server_fd);
        unlink(socket_name);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 1) == -1) {
        perror("listen failed");
        close(server_fd);
        unlink(socket_name);
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        close(server_fd);
        unlink(socket_name);
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        /*
        Server logic part. We in a parent process.
        Wait untill client connect. 
        When client connect, accept(2) create fd for comm.
        */
        client_fd = accept(server_fd, NULL, NULL);
        close(server_fd);
        unlink(socket_name);

        if (client_fd < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        chat_fd(client_fd, client_fd, "server", "client");
        close(client_fd);
    } 
    else {
        /*
        Client logic part. Before start, we wait until server starts.
        */
        sleep(1);

        sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sock_fd < 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        /*
        Connectoing to server.
        */
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