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


void handleLogin(int clientFD)
{
    int maxUsernameSize = 26;
    char usernameBuffer[maxUsernameSize];
    int usernameSize;
    // Prefixing some  messages! 

    char welcomeMsg[] = "Welcome to OTM!\n\n";
    char usernameReq[] = "Please enter your session username:";
    char successMsg[] = "\nYou have now successfully logged in as:";

    // Requesting session username!

    errHandle((send(clientFD,welcomeMsg,strlen(welcomeMsg),0)),SOCK_ERROR,"Send: Welcome msg: Server side\n");
    errHandle((send(clientFD,usernameReq,strlen(usernameReq),0)),SOCK_ERROR,"Send: User name request: server side!");

    // Receiving the username!

    errHandle((usernameSize = recv(clientFD,usernameBuffer,maxUsernameSize-1,0)),SOCK_ERROR,"Receive: Username: Server side!");

    // Null terminating the username!

    usernameBuffer[usernameSize] = '\0';
    fprintf(stdout,"Username: %s\n",usernameBuffer);

    // Confirmatory message!
    errHandle((send(clientFD,successMsg,strlen(successMsg),0)),SOCK_ERROR,"Send: successfull login message!");
    errHandle((send(clientFD,usernameBuffer,strlen(usernameBuffer),0)),SOCK_ERROR,"Send: resending username!");
}
