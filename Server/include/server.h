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

// @warning: All functions work only with ipV4.

/**
 * @include server.h
 * 
 * @brief Creates a socket and returns the socket file descriptor. Does error handling.
 * 
 * @param serverType - SOCK_STREAM(for TCP) or SOCK_DGRAM(for Datagram).
 * 
 * @return An integer which is the socket file descriptor.
 * @note Ipv4 only.
*/
int getSocket(int serverType);

/**
 * @brief Fills a sockaddr_in  struct and returns it.
 * @param port - The port the server runs on.
 * @return A sockaddr_in struct
*/
struct sockaddr_in get_sockaddr_in(int port);

/**
 * @brief Binds address and listens for connections.
 * 
 * @param serverFD - The socket file descriptor
 * @param serverAddr - A pointer to a sockaddr_in struct containing server address info.
 * 
 * @note Make sure the sockaddr_in struct is filled.
*/
void bind_and_listen(int *serverFD, struct sockaddr_in *serverAddr);

/**
 * @brief Accepts incoming request and returns an accept file descriptor.
 *        Also prints out successful connections.
 * @param serverFD -  The socket file descriptor.
 * @param clientAddr - A pointer to a sockaddr_in struct to store client address.
 * @param clientAddrlen - A pointer to a socklen_t instance to store the client;s address length.
 * 
 * @return Returns 
 * 
 * @note Should be called within a loop.
*/
int acceptConnections(int serverFD, struct sockaddr_in *clientAddr, socklen_t *clientAddrlen);

/**
 * @brief Sends message to a chosen client.
 * @param userName - The username of  the recipient.
 * @param message - The message to be sent.
*/
void sendTCP(char userName, char message);

/**
 * @brief Receives message clients.
 * @param userName - The username of the sender.
 * @return The received message .
 */
char* receiveTCP(int clientFD);

#endif