#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_RESPONSE_LENGTH 1024
#define CLIENT_RESPONSE_LENGTH 4096
#define CLIENT_REQUEST_LENGTH 256
#define HTTP_HEADER_LENGTH 2048
#define PATH_LENGTH 128

typedef struct {
    char **files;
    ssize_t num_files;
} file_arr;

// handles the request from the client
void handle_request(int client_socket, file_arr *files);
// render html and store as string data
void render_html(char *res_data, char *method, char* path, file_arr *files);
// render index.html with dynamic content
void render_index(char *res_data, file_arr *files);