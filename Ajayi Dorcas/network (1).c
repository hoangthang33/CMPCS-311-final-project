#include "proj.h"

int setup_server_socket(const char* ip, int port) {
int sockfd;
struct sockaddr_in addr;

if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
perror("Socket creation failed");
exit(EXIT_FAILURE);
}

memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_port = htons(port);
addr.sin_addr.s_addr = inet_addr(ip);

if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
perror("Socket bind failed");
exit(EXIT_FAILURE);
}

if (listen(sockfd, 5) < 0) {
perror("Listen failed");
exit(EXIT_FAILURE);
}

return sockfd;
}

int accept_connection(int server_sock) {
struct sockaddr_in client_addr;
socklen_t client_len = sizeof(client_addr);
int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
if (client_sock < 0) {
perror("Accept failed");
exit(EXIT_FAILURE);
}
return client_sock;
}


