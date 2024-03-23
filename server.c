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
    file_arr *files = malloc(sizeof(file_arr));
    if (files == NULL) {
        perror("failed to allocate memory for file array");
        exit(EXIT_FAILURE);
    }
    files->files = NULL;
    files->num_files = 0;
    fprintf(stderr, "listening on: http://localhost:%d\n", PORT);
    while(1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("failed to accept client");
            free_files(files);
            exit(EXIT_FAILURE);
        }
        handle_request(client_socket, files);
        close(client_socket);
    }
    close(server_socket);
    free_files(files);
    return EXIT_SUCCESS;
}

void handle_request(int client_socket, file_arr *files) {
    FILE *client_file = fdopen(client_socket, "r");

    char client_request[CLIENT_REQUEST_LENGTH];
    if ((fgets(client_request, CLIENT_REQUEST_LENGTH - 1, client_file)) == 0) {
        perror("failed to read request from client");
        exit(EXIT_FAILURE);
    }
    char *delimeter = " \n\r\t";
    char *method = strtok(client_request, delimeter);
    char *path = strtok(NULL, delimeter);
    char response[SERVER_RESPONSE_LENGTH] = "HTTP/1.1 200 OK\r\n\n";
    if (strcmp(path, "/favicon.ico") == 0) {
        fclose(client_file);
        return;
    }
    else {
        char res_data[SERVER_RESPONSE_DATA_LENGTH] = {0};
        render_html(res_data, method, path, files);
        strcat(response, res_data);
    }
    if (send(client_socket, response, SERVER_RESPONSE_LENGTH, 0) < 0) {
        perror("failed to send http header to client");
        exit(EXIT_FAILURE);
    }
    fclose(client_file);
}

void render_html(char *res_data, char *method, char *path, file_arr *files) {
    FILE *html_data;
    if (strcmp(method, "GET") == 0) {
        if (strcmp(path, "/") == 0) {
            render_index(res_data, files);
            return;
        }
        else {
            char *sub_path = strtok(path, "?");
            if (strcmp(sub_path, "/add") != 0) {
                render_file(res_data, sub_path);
                return;
            }
            char *key = strtok(NULL, "=");
            if (key == NULL || strcmp(key, "filename") != 0) {
                html_data = fopen("./template/notfound.html", "r");
            }
            else {
                char *value = strtok(NULL, "");
                if (value == NULL) {
                    render_index(res_data, files);
                    return;
                }
                files->num_files++;
                files->files = realloc(files->files, sizeof(char*) * files->num_files);
                files->files[files->num_files - 1] = malloc(strlen(value));
                strcpy(files->files[files->num_files - 1], value);
                render_index(res_data, files);
                return;
            }
        }
    }
    if (html_data == NULL) {
        perror("failed to open file");
        html_data = fopen("./template/notfound.html", "r");
    }
    size_t res_size = fread(res_data, sizeof(char), SERVER_RESPONSE_DATA_LENGTH, html_data);
    res_data[res_size] = '\0';
    fclose(html_data);
}

void render_index(char *res_data, file_arr *files) {
    char index_hd_buff[SERVER_RESPONSE_DATA_LENGTH];
    FILE* index_hd_data = fopen("index-head.html", "r");
    size_t ihd_size = fread(index_hd_buff, sizeof(char), SERVER_RESPONSE_DATA_LENGTH - 1, index_hd_data);
    index_hd_buff[ihd_size] = '\0';
    // fprintf(stderr, "head: %s\n", index_hd_buff);
    char index_ft_buff[SERVER_RESPONSE_DATA_LENGTH];
    FILE* index_ft_data = fopen("index-foot.html", "r");
    size_t ift_size = fread(index_ft_buff, sizeof(char), SERVER_RESPONSE_DATA_LENGTH - 1, index_ft_data);
    index_ft_buff[ift_size] = '\0';
    // fprintf(stderr, "foot: %s\n", index_ft_buff);
    strcat(res_data, index_hd_buff);
    char *open_tag_hd = "<li class=\"file\"><a href=\"";
    char *open_tag_tl = "\">";
    char *delete_button = "</a><a href=\"/delete?filename=";
    char *close_tag = "\"><button>delete</button></a></li>";
    for (int i = 0; i < files->num_files; i++) {
        strcat(res_data, open_tag_hd);
        strcat(res_data, files->files[i]);
        strcat(res_data, open_tag_tl);
        strcat(res_data, files->files[i]);
        strcat(res_data, delete_button);
        strcat(res_data, files->files[i]);
        strcat(res_data, close_tag);
    }
    strcat(res_data, index_ft_buff);
    fclose(index_hd_data);
    fclose(index_ft_data);
}

void render_file(char *res_data, char *sub_path) {
    char file_hd_buff[SERVER_RESPONSE_DATA_LENGTH];
    FILE* file_hd_data = fopen("./template/file-head.html", "r");
    size_t fhd_size = fread(file_hd_buff, sizeof(char), SERVER_RESPONSE_DATA_LENGTH - 1, file_hd_data);
    file_hd_buff[fhd_size] = '\0';

    char file_ft_buff[SERVER_RESPONSE_DATA_LENGTH];
    FILE* file_ft_data = fopen("./template/file-foot.html", "r");
    size_t fft_size = fread(file_ft_buff, sizeof(char), SERVER_RESPONSE_DATA_LENGTH - 1, file_ft_data);
    file_ft_buff[fft_size] = '\0';

    char path[PATH_LENGTH];
    substring(sub_path, path, 1);
    strcat(res_data, file_hd_buff);
    strcat(res_data, path);
    strcat(res_data, file_ft_buff);

    fclose(file_hd_data);
    fclose(file_ft_data);
}

void free_files(file_arr *files) {
    for (int i = 0; i < files->num_files; i++) {
        free(files->files[i]);
    }
    free(files->files);
    free(files);
}