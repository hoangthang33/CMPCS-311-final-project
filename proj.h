#ifndef PROJ_H
#define PROJ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_DATA_SIZE 1024
#define STAT_OK 1
#define STAT_FAIL -1

#define CMD_SEND 1
#define CMD_RECV 2
#define CMD_RESP 3
#define CMD_DATA 4

struct send_msg {
int msg_type;
int file_size;
char filename[128];
};

struct resp_msg {
int msg_type;
int status;
int filesize;
};

struct data_msg {
int msg_type;
int data_leng;
char buffer[MAX_DATA_SIZE];
};

int setup_server_socket(const char* ip, int port);
int accept_connection(int server_sock);
void handle_client(int client_sock);
void receive_msg(int sock, void *msg, size_t size);
void send_msg(int sock, const void *msg, size_t size);
void receive_file(int client_sock, char *filename, int filesize);
void send_file(int client_sock, char *filename);

#endif