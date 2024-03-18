#include "config.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        perror("please provided an address");
        exit(EXIT_FAILURE);
    } 
    char *address = argv[1];
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("failed to create socket descriptor");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
    // inet_aton(address, (struct in_addr*) &server_address.sin_addr.s_addr);

    if (connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        perror("failed to connect socket and address");
        exit(EXIT_FAILURE);
    }
    char request[] = "GET / HTTP/1.1\r\n\n";
    char response[CLIENT_RESPONSE_LENGTH];
    if (send(client_socket, request, sizeof(request), 0) < 0) {
        perror("failed to send request to server");
        exit(EXIT_FAILURE);
    }
    if (recv(client_socket, &response, CLIENT_RESPONSE_LENGTH, 0) < 0) {
        perror("failed to receive response from server");
        exit(EXIT_FAILURE);
    }
    printf("Response from the server:\n %s\n", response);
    close(client_socket);
    return EXIT_SUCCESS;
}