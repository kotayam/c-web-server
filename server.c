#include "config.h"

int main() {
    char server_message[SERVER_MESSAGE_LENGTH] = "You have reached the server!";
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("failed to create socket descriptor");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        perror("failed to bind socket to address and port");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, 5) < 0) {
        perror("failed to listen");
        exit(EXIT_FAILURE);
    }
    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket < 0) {
        perror("failed to accept client");
        exit(EXIT_FAILURE);
    }
    if (send(client_socket, server_message, sizeof(server_message), 0) < 0) {
        perror("failed to send message to client");
        exit(EXIT_FAILURE);
    }
    close(server_socket);
    return EXIT_SUCCESS;
}