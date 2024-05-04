#include "../include/server.h"
#define PORT 8888


int main(int argc,char* argv[])
{
    int serverFD;
    int clientFD;
    char* received;
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
        received = receiveTCP(clientFD);
        printf("Received: %s", received);

    }
    close(serverFD);
    close(clientFD);
}