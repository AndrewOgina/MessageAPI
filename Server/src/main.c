#include "../include/server.h"
#define PORT 8088
#define BACKLOG 10

int main(int argc,char* argv[])
{
    int serverFD;
    Clients client;
    serverFD = setup_server(PORT,SOCK_STREAM,BACKLOG);
    client = acceptConnections(serverFD); 
    handleMessaging(client);
}