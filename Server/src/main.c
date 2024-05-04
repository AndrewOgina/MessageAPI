#include "../include/server.h"
#define PORT 8888


int main()
{
    int serverFD;
    int clientFD;
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
        close(serverFD);
        send(clientFD,"Hello",sizeof("Hello"),0);
    }
    close(clientFD);
}