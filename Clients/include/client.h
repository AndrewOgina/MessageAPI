#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define MAXLEN 4096
#define SA struct sockaddr
#define SA_IN struct sockaddr_in
#define SOCK_ERROR -1
#define USERNAME_SIZE 25
/**
 * @brief Connects the client to the server.
 * @param port - The port the server is listening on.
 * @param server_address - The server's address.
 * @param username - The client's session username.
 * @return The server's file descriptor.
*/
int connect_to_server(int port,char* ip_address,char* username);


/**
 * @details Allows the client to send and receive messages.
 * @param server_fd - The server's file descriptor.
 * @return void!
 * @note Uses Threads.
*/
void join_broadcast(int* server_fd);

#endif