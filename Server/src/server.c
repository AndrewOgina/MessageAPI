#include "../include/server.h"

int getSocket(int serverType)
{
    int serverFD = socket(AF_INET, serverType, 0);
    if (serverFD == -1)
    {
        perror("socket: server side!");
        exit(EXIT_FAILURE);
    }
    return serverFD;
}

struct sockaddr_in get_sockaddr_in(int port)
{
    struct sockaddr_in serverInfo;
    memset(&serverInfo, 0, sizeof(serverInfo));
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    serverInfo.sin_port = htons(port);
    return serverInfo;
}

void bind_and_listen(int *serverFD, struct sockaddr_in *serverInfo)
{
    if ((bind(*serverFD, (SA *)serverInfo, sizeof(*serverInfo))) == -1)
    {
        perror("bind: server side");
        close(*serverFD);
        exit(EXIT_FAILURE);
    }

    if ((listen(*serverFD, 10)) == -1)
    {
        perror("listen: server side");
        close(*serverFD);
        exit(EXIT_FAILURE);
    }
}

int acceptConnections(int serverFD, struct sockaddr_in *clientInfo, socklen_t *clientAddrlen)
{
    char addrinfo[INET_ADDRSTRLEN];
    int clientFD = accept(serverFD, (SA *)&clientInfo, clientAddrlen);
    if (clientFD == -1)
    {
        perror("accept: server side");
        close(serverFD);
        exit(EXIT_FAILURE);
    }
    inet_ntop(AF_INET, &clientInfo, addrinfo, sizeof(addrinfo));

    return clientFD;
}

/**
 * @brief Receives message clients.
 * @param userName - The username of the sender.
 * @param msgBuffer - An array where the received message will be stored.
 * @return The received message .
 */
char* receiveTCP(int clientFD,char* msgBuffer)
{
    int msgLen;
    // TODO
    // 10 is a randomly selected number for now...I'll update it.
    if ((msgLen = recv(clientFD, msgBuffer, MAXLEN - 10, 0)) == -1)
    {
        perror("recv(): server side");
        close(clientFD);
        exit(EXIT_FAILURE);
    }
    // null terminating it.
    msgBuffer[msgLen] = '\0';
    return msgBuffer;
}

/**
 * @brief Sends message to a chosen client.
 * @param userName - The username of  the recipient.
 * @param message - The message to be sent.
 */
void sendTCP(int clientFD,char* message)
{
    int msgSize = sizeof(message);
    if((send(clientFD,message,msgSize,0))==-1)
    {
        perror("send: server side");
        close(clientFD);
        exit(EXIT_FAILURE);
    }
}

