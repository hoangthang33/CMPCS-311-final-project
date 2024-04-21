#include "proj.h"

void receive_msg(int sock, void *msg, size_t size) {
if (recv(sock, msg, size, 0) < 0) {
perror("Failed to receive message");
close(sock);
exit(EXIT_FAILURE);
}
}

void send_msg(int sock, const void *msg, size_t size) {
if (send(sock, msg, size, 0) < 0) {
perror("Failed to send message");
close(sock);
exit(EXIT_FAILURE);
}
}

void receive_file(int client_sock, char *filename, int filesize) {
FILE *file = fopen(filename, "wb");
if (!file) {
perror("Failed to open file");
close(client_sock);
exit(EXIT_FAILURE);
}

struct data_msg data;
int received = 0;
while (received < filesize) {
receive_msg(client_sock, &data, sizeof(data));
fwrite(data.buffer, 1, data.data_leng, file);
received += data.data_leng;
}
fclose(file);
}

void send_file(int client_sock, char *filename) {
FILE *file = fopen(filename, "rb");
if (!file) {
perror("Failed to open file");
close(client_sock);
exit(EXIT_FAILURE);
}

struct data_msg data;
data.msg_type = CMD_DATA;
int read_bytes;
while ((read_bytes = fread(data.buffer, 1, sizeof(data.buffer), file)) > 0) {
data.data_leng = read_bytes;
send_msg(client_sock, &data, sizeof(data));
}
fclose(file);
}