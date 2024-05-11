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
#define MAX_USERNAME_SIZE 26
#define MAX_CLIENTS 10

/**
 * @brief Stores the client details.
 * @param clientFD - The client's File descriptor.
 * @param userName - The username of the client. 
*/
typedef struct Clients
{
    int clientFD; //-> The client's File descriptor.
    char userName[MAX_USERNAME_SIZE+10]; //--> The username of the client. 
}Clients;

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
struct Clients acceptConnections(int serverFD);

void handleMessaging(struct Clients clients);






#endif