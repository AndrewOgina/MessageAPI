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
#define SA struct sockaddr
#define MAXLEN 4096
#define BACKLOG 10
#define SOCK_ERROR -1
#define SA_IN struct sockaddr_in

/**
 * @brief Sets up the server socket and listens for connections. 
 * @param port - The port number the server will run on.
 * @param sockType - Either SOCK_STREAM (TCP server) or SOCK_DGRAM (UDP server).
 * @return The server's file descriptor.
*/
int setup_server(int port,int sockType,int backlog);

/**
 * @brief Accepts new connections and stores their details.
 * @param serverFD - The file descriptor the server is listening on.
 * @return The connected client's file descriptor.
 * @note FOR TCP SERVERS.
*/
int acceptConnections(int serverFD);

/**
 * @brief Requests the session username of connected clients.
 * @param clientFD - The client's file descriptor.
 * @return void.
*/
void handleLogin(int clientFD);




#endif