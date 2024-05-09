#include "../include/client.h"

int getSocket(int serverType)
{
    int serverFD;
    if ((serverFD = socket(AF_INET, serverType, 0)) == -1)
    {
        perror("Socket: client side!");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "Socket created successfully!\n");
    return serverFD;
}

struct sockaddr_in get_serverAddr(int port, char *address)
{
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if ((inet_pton(AF_INET, address, &serverAddr.sin_addr)) == -1)
    {
        perror("inet_pton: Failed to get address!");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "Server address retrieved successfully!\n");
    return serverAddr;
}

void connectTCP(int serverFD, struct sockaddr_in serverAddr)
{
    if ((connect(serverFD, (SA *)&serverAddr, sizeof(serverAddr))) == -1)
    {
        perror("Connect: client side!");
        close(serverFD);
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "Connection established successfully!\n");
}

void sendTCP(int serverFD, char *message)
{
    if((send(serverFD,message,strlen(message),0))==-1)
    {
        close(serverFD);
        perror("Send: client side!");
        exit(EXIT_FAILURE);
    }
}

void receiveTCP(int serverFD, char *msgBuffer)
{
    int msgLen;
    if ((msgLen = recv(serverFD, msgBuffer, sizeof(msgBuffer), 0)) == -1)
    {
        close(serverFD);
        perror("recv: client side!");
        exit(EXIT_FAILURE);
    }
    msgBuffer[msgLen] = '\0';
    printf("Message received: %s\n", msgBuffer);
}
