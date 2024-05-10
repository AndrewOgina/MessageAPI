#include "../include/server.h"

/**
 * @brief Error handling.
 * @param returnVal - The value returned by a function.
 * @param errVal - The value returned by unsuccessful function call.
 * @param errorMsg - The message printed incase of an error.
*/
int errHandle(int returnVal,int errVal,char* errorMsg)
{
    if(returnVal == errVal)
    {
        perror(errorMsg);
        exit(EXIT_FAILURE);
    }
    return returnVal;
}


int setup_server(int port,int sockType,int backlog)
{
    int serverFD;
    SA_IN serverAddr;

    // 1. Creating the socket.

    errHandle((serverFD = socket(AF_INET,sockType,0)),SOCK_ERROR,"Socket: Server side!\n");
    fprintf(stdout,"Socket created!\n");
    
    // 2. Filling the serverAddr struct.

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // 3. Binding socket to address and port.

    errHandle((bind(serverFD,(SA*)&serverAddr,sizeof(serverAddr))),SOCK_ERROR,"Bind: Server side!\n");
    fprintf(stdout,"Socket binded to an address!\n");

    // 4. Listen for clients.

    errHandle((listen(serverFD,backlog)),SOCK_ERROR,"Listen: Server side!\n");
    fprintf(stdout,"Listening...\n");

    // 5. Returning the server's file descriptor.
    return serverFD;
}

int acceptConnections(int serverFD)
{
    // Variable declarations!

    socklen_t clientAddrSize = sizeof(SA_IN);
    SA_IN clientAddr;
    int clientFD;
    char clientIP[INET_ADDRSTRLEN];

    // Accepting new connections!

    errHandle((clientFD = accept(serverFD,(SA*)&clientAddr,&clientAddrSize)),SOCK_ERROR,"Accept: Server side!");
    fprintf(stdout,"Accepting connections...\n");

    // Getting the Ip address 
    inet_ntop(AF_INET,&clientAddr,clientIP,sizeof(clientIP));
    fprintf(stdout,"Accepted connection from: %s...\n",clientIP);
    
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
    int msgSize = strlen(message);
    printf("Message size: %i",msgSize);
    if((send(clientFD,message,msgSize,0))==-1)
    {
        perror("send: server side");
        close(clientFD);
        exit(EXIT_FAILURE);
    }
}


void receive_and_send(int clientFD,char* msgBuffer)
{
    char* message = receiveTCP(clientFD,msgBuffer);
    printf("message:::%s",message);
    sendTCP(clientFD,message);
}
