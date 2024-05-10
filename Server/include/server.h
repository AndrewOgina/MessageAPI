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
 * @brief Receive data from a TCP server and sends it back to sender
 * @param clientFD - The client's file descriptor.
 * @return void.
*/
void receive_and_send(int clientFD,char* msgBuffer);

#endif