#include "../include/client.h"

int getSocket(int serverType)
{
    int serverFD;
    if((serverFD = socket(AF_INET,serverFD,0))==-1)
    {
        perror("Socket: client side!");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout,"Socket created succesfully!\n");
    return serverFD;
}

struct sockaddr_in get_serverAddr(int port,char address)
{
    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if(inet_pton(AF_INET,address,&serverAddr.sin_addr))
    {
        perror("inet_pton: Failed to get address!");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout,"Server address retreived successfully!\n");
    return serverAddr;

}

void connectTCP(int serverFD,struct sockaddr_in serverAddr)
{
    if((connect(serverFD,(SA*)&serverAddr,sizeof(serverAddr)))==-1)
    {
        perror("Connect: client side!");
        close(serverFD);
        exit(EXIT_FAILURE);
    }
    fprintf(stdout,"Connection established succesfully!\n");
}


