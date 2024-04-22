// client.c
#include "proj.h"
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
int sockfd, file_fd;
struct sockaddr_in serv_addr;
struct stat file_stat;
send_msg s_msg;
resp_msg r_msg;

if (argc != 5) {
fprintf(stderr, "Usage: %s <IP> <Port> -s/-r <Filename>\n", argv[0]);
exit(EXIT_FAILURE);
}

// Setup socket
sockfd = socket(AF_INET, SOCK_STREAM, 0);
memset(&serv_addr, 0, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
serv_addr.sin_port = htons(atoi(argv[2]));

// Connect to the server
connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

// Prepare message based on command line
strcpy(s_msg.filename, argv[4]);
s_msg.file_size = 0;

if (strcmp(argv[3], "-s") == 0) {
// Send file to server
s_msg.msg_type = CMD_SEND;
file_fd = open(s_msg.filename, O_RDONLY);
if (file_fd < 0) {
perror("Failed to open file");
exit(EXIT_FAILURE);
}
if (fstat(file_fd, &file_stat) < 0) {
perror("Failed to get file stats");
exit(EXIT_FAILURE);
}
s_msg.file_size = file_stat.st_size;
} else if (strcmp(argv[3], "-r") == 0) {
// Request file from server
s_msg.msg_type = CMD_RECV;
} else {
fprintf(stderr, "Invalid option %s. Use -s to send or -r to receive.\n", argv[3]);
exit(EXIT_FAILURE);
}

// Send the message
send_mesg(sockfd, &s_msg, sizeof(s_msg));

// Handle response from server
receive_msg(sockfd, &r_msg, sizeof(r_msg));
if (r_msg.status == STAT_OK) {
if (s_msg.msg_type == CMD_SEND) {
send_data(file_fd, sockfd, s_msg.file_size);
} else if (s_msg.msg_type == CMD_RECV) {
file_fd = open(s_msg.filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
if (file_fd < 0) {
perror("Failed to open file for writing");
exit(EXIT_FAILURE);
}
recv_data(sockfd, file_fd, r_msg.filesize);
}
} else {
fprintf(stderr, "Server responded with an error.\n");
exit(EXIT_FAILURE);
}

close(file_fd);
close(sockfd);
return 0;
}
