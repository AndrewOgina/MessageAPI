#include "../include/client.h"
#include <pthread.h>
#define PORT 8080


int main(int argc,char* argv[])
{
    int serverFD;
    char message[MAXLEN];
    struct sockaddr_in serverAddr;

//@brief Checks whether the server IP address was provided at startup.
    if(argc != 2)
    {
        fprintf(stderr,"Usage: ./bin/client <Server Ip address e.g 127.0.0.1>");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout,"Server address: %s\n",argv[1]);
    serverFD = getSocket(AF_INET);
    serverAddr = get_serverAddr(PORT,argv[1]);
    connectTCP(serverFD,serverAddr);

    while(1)
    {
        sendTCP(serverFD,message);
    }
    close(serverFD);
    
    return 0;
}