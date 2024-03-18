#include "config.h"

int main() {
    FILE *html_data = fopen("index.html", "r");
    if (html_data == NULL) {
        perror("failed to open index.html");
        exit(EXIT_FAILURE);
    }
    char res_data[SERVER_RESPONSE_LENGTH];
    fread(res_data, sizeof(char), SERVER_RESPONSE_LENGTH, html_data);
    char http_header[HTTP_HEADER_LENGTH] = "HTTP/1.1 200 OK\r\n\n";
    strcat(http_header, res_data);

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
    int client_socket;
    while(1) {
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("failed to accept client");
            exit(EXIT_FAILURE);
        }
        if (send(client_socket, http_header, sizeof(http_header), 0) < 0) {
            perror("failed to send http header to client");
            exit(EXIT_FAILURE);
        }
        close(client_socket);
    }
    close(server_socket);
    fclose(html_data);
    return EXIT_SUCCESS;
}