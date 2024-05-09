#include "../include/server.h"
#define PORT 8080


int main(int argc,char* argv[])
{
    int serverFD;
    int clientFD;
    char msgBuffer[MAXLEN];
    struct sockaddr_in serverInfo;
    struct sockaddr_in clientInfo;
    socklen_t clientAddrlen;

    serverFD = getSocket(SOCK_STREAM);
    printf("%i serverFD\n",serverFD);
    serverInfo = get_sockaddr_in(PORT);
    bind_and_listen(&serverFD,&serverInfo);

    while(1)
    {
        clientFD = acceptConnections(serverFD,&clientInfo,&clientAddrlen);
        send(clientFD,"Hello",sizeof("Hello"),0);
        
        while(1)
        {
        receive_and_send(clientFD,msgBuffer);
        }
    }
    close(serverFD);
    close(clientFD);
} 