#include "config.h"

int main() {
    int network_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (network_socket < 0) {
        perror("failed to create socket descriptor");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        perror("failed to connect socket and address");
        exit(EXIT_FAILURE);
    }
    char server_res[RESPONSE_LENGTH];
    if (recv(network_socket, &server_res, RESPONSE_LENGTH, 0) < 0) {
        perror("failed to receive response from server");
        exit(EXIT_FAILURE);
    }
    printf("The server sent the data: %s\n", server_res);
    close(network_socket);
    return EXIT_SUCCESS;
}