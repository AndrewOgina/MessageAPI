#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <sys/epoll.h>

#define SA struct sockaddr
#define MAXLEN 4096
#define BACKLOG 10
#define SOCK_ERROR -1
#define SA_IN struct sockaddr_in
#define USERNAME_SIZE 26
#define MAX_CLIENTS 10
#define ONLINE true
#define OFFLINE false

/**
 * @brief The struct holding the client's information.
 * @param clientFD - The client's File descriptor.
 * @param userName - The username of the client.
 * @param state - Either ONLINE or OFFLINE.
 */
typedef struct Client
{
    int client_fd;                //-> The client's File descriptor.
    char username[USERNAME_SIZE]; //-> The username of the client.
    bool state;                   //-> Either ONLINE or OFFLINE.
} Client;

/**
 * @brief Sets up the server socket and listens for connections.
 * @param port - The port number the server will run on.
 * @param sock_type - Either SOCK_STREAM (TCP server) or SOCK_DGRAM (UDP server).
 * @param backlog- The number of clients the server can listen at a time.
 * @return The server's file descriptor.
 */
int setup_server(int port, int sock_type, int backlog);

/**
 * @brief Accepts new connections,stores their details and receives and broadcasts messages.
 * @param serverFD - The file descriptor the server is listening on.
 * @return void!
 * @note FOR TCP SERVERS & FOR BROADCASTS.
 */
void handle_connections(int serverFD);

#endif