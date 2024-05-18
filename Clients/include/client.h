#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
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
 * @return The server's file descriptor.
*/
int connect_to_server(int port,char* server_address);


/**
 * @brief Sends a message over the server file descriptor.
 * @param serverFD - The server's file descriptor.
 * @param message - An array containing the message to be sent.
 * @return void.
*/
void sendTCP(int serverFD,char* message);

/**
 * @brief Receives a message over from the server.
 * @param serverFD - The server's file descriptor.
 * @param msgBuffer - An array buffer where the received message will be stored.
*/
void receiveTCP(int serverFD,char* msgBuffer);


#endif