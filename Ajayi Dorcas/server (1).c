#include "proj.h"

int main(int argc, char *argv[]) {
if (argc != 3) {
fprintf(stderr, "Usage: %s <IP address> <Port>\n", argv[0]);
exit(EXIT_FAILURE);
}

int server_sock = setup_server_socket(argv[1], atoi(argv[2]));
printf("Server: listening on %s:%d\n", argv[1], atoi(argv[2]));

while (1) {
int client_sock = accept_connection(server_sock);
printf("Server: Connected to client\n");

// Handle client in a separate function or thread if threading were allowed
handle_client(client_sock);
}

// Close server socket (technically unreachable code in an infinite loop)
close(server_sock);
return 0;
}

void handle_client(int client_sock) {
struct send_msg msg;
receive_msg(client_sock, &msg, sizeof(msg));

printf("Server: Received message type %d from client\n", msg.msg_type);

switch (msg.msg_type) {
case CMD_SEND:
printf("Server: Preparing to receive file '%s' of size %d bytes\n", msg.filename, msg.file_size);
receive_file(client_sock, msg.filename, msg.file_size);
break;
case CMD_RECV:
printf("Server: Preparing to send file '%s'\n", msg.filename);
send_file(client_sock, msg.filename);
break;
default:
fprintf(stderr, "Server: Unknown command received.\n");
}

// Close the client socket after handling the request
close(client_sock);
printf("Server: Connection closed\n");
}
