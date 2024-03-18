#include "config.h"

int main() {
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
    fprintf(stderr, "listening on: http://localhost:%d\n", PORT);
    int client_socket;
    while(1) {
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("failed to accept client");
            exit(EXIT_FAILURE);
        }
        handle_request(client_socket);
        close(client_socket);
    }
    close(server_socket);
    return EXIT_SUCCESS;
}

void handle_request(int client_socket) {
    FILE *client_file = fdopen(client_socket, "r");
    char client_request[CLIENT_REQUEST_LENGTH];
    if (fgets(client_request, CLIENT_REQUEST_LENGTH - 1, client_file) == NULL) {
        perror("failed to read request from client");
        exit(EXIT_FAILURE);
    }
    char *delimeter = " \n\r\t";
    char *method = strtok(client_request, delimeter);
    char path[PATH_LENGTH] = ".";
    char *path_r = strtok(NULL, delimeter);
    strcat(path, path_r);
    char http_header[HTTP_HEADER_LENGTH] = "HTTP/1.1 200 OK\r\n\n";
    if (strcmp(path, "./favicon.ico") != 0) {
        char *res_data = calloc(SERVER_RESPONSE_LENGTH, sizeof(char));
        if (res_data == NULL) {
            perror("failed to allocate memory for response data");
            exit(EXIT_FAILURE);
        }
        render_html(res_data, method, path);
        strcat(http_header, res_data);
        free(res_data);
    }
    if (send(client_socket, http_header, sizeof(http_header), 0) < 0) {
        perror("failed to send http header to client");
        exit(EXIT_FAILURE);
    }
    fclose(client_file);
}

void render_html(char *res_data, char *method, char *path) {
    FILE *html_data;
    if (strcmp(method, "GET") == 0) {
        if (strcmp(path, "./") == 0) {
            render_index(res_data);
            return;
        }
        else {
            html_data = fopen(path, "r");
        }
    }
    if (html_data == NULL) {
        perror("failed to open file");
        html_data = fopen("./template/notfound.html", "r");
    }
    fread(res_data, sizeof(char), SERVER_RESPONSE_LENGTH - 1, html_data);
    fclose(html_data);
}

void render_index(char *res_data) {
    char head_buff[SERVER_RESPONSE_LENGTH];
    FILE* head_data = fopen("index-head.html", "r");
    fread(head_buff, sizeof(char), SERVER_RESPONSE_LENGTH - 1, head_data);
    char foot_buff[SERVER_RESPONSE_LENGTH];
    FILE* foot_data = fopen("index-foot.html", "r");
    fread(foot_buff, sizeof(char), SERVER_RESPONSE_LENGTH - 1, foot_data);
    strcat(res_data, head_buff);
    strcat(res_data, foot_buff);
    fclose(head_data);
    fclose(foot_data);
}