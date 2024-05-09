#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define MAXLEN 4096
#define SA struct sockaddr

/**
 * @brief Creates a socket and returns the socket file descriptor.
 * @param serverType - SOCK_STREAM(for TCP) or SOCK_DGRAM(for Datagram).
 * @return An integer which is the socket file descriptor.
 * @note Ipv4 only.
*/
int getSocket(int serverType);

/**
 * @brief Fills a sockaddr_in  struct and returns it.
 * @param port - The port the server runs on.
 * @return A sockaddr_in struct
 */
struct sockaddr_in get_serverAddr(int port,char* address);

/**
 * @brief Connects to a TCP server.
 * @param serverFD - The socket file descriptor for the server.
 * @param serverAddr -  A sockaddr_in struct containing the server address info.
 * @return void.
 * @note The serverAddr struct must be filled. 
*/
void connectTCP(int serverFD,struct sockaddr_in serverAddr);

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