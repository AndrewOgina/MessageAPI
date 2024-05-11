#include "../include/server.h"

/**
 * @brief Error handling.
 * @param returnVal - The value returned by a function.
 * @param errVal - The value returned by unsuccessful function call.
 * @param errorMsg - The message printed incase of an error.
*/
int errHandle(int returnVal, int errVal, char *errorMsg)
{
    if (returnVal == errVal)
    {
        perror(errorMsg);
        exit(EXIT_FAILURE);
    }
    return returnVal;
}

int setup_server(int port, int sockType, int backlog)
{
    int serverFD;
    SA_IN serverAddr;

    // 1. Creating the socket.

    errHandle((serverFD = socket(AF_INET, sockType, 0)), SOCK_ERROR, "Socket: Server side!\n");
    fprintf(stdout, "Socket created!\n");
    
    // 2. Filling the serverAddr struct.

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // 3. Binding socket to address and port.

    errHandle((bind(serverFD, (SA *)&serverAddr, sizeof(serverAddr))), SOCK_ERROR, "Bind: Server side!\n");
    fprintf(stdout, "Socket binded to an address!\n");

    // 4. Listen for clients.

    errHandle((listen(serverFD, backlog)), SOCK_ERROR, "Listen: Server side!\n");
    fprintf(stdout, "Listening...\n");

    // 5. Returning the server's file descriptor.
    return serverFD;
}

/**
 * @brief Requests the session username of connected clients.
 * @param clientFD - The client's file descriptor.
 * @return A formatted username.
 */
void handleLogin(int clientFD, char *userName)
{
    int usernameSize;
    // Prefixing some  messages!

    char welcomeMsg[25] = "WELCOME TO OTP.\n"; // Array should be large enough for bolding  and underlining tags!

    // Bolding and underlining!
    char* formatted_welcomeMsg = format_text(welcomeMsg,BOLD_UNDERLINE);

    char loginMsg[32] = "You are logged in as:";
    char* formatted_loginMsg =  format_text(loginMsg,BOLD);

    // Requesting session username!

    errHandle((send(clientFD, formatted_welcomeMsg, strlen(formatted_welcomeMsg), 0)), SOCK_ERROR, "Send: Welcome msg: Server side\n");
    free(formatted_welcomeMsg);
    // Receiving the username!

    errHandle((usernameSize = recv(clientFD, userName, MAX_USERNAME_SIZE - 1, 0)), SOCK_ERROR, "Receive: Username: Server side!");
    // Null terminating the username!

    // A little bit of formatting.
    char* formatted_userName = format_text(userName,BOLD_RED);

    fprintf(stdout, "Username: %s\n", formatted_userName);

    // Confirmatory message!
    errHandle((send(clientFD, formatted_loginMsg, strlen(formatted_loginMsg), 0)), SOCK_ERROR, "Send: successful login message failed to be sent!");
    free(formatted_loginMsg);
    errHandle((send(clientFD, formatted_userName, strlen(formatted_userName), 0)), SOCK_ERROR, "Send: resending username failed to be sent!");
    free(formatted_userName);
}

struct Clients acceptConnections(int serverFD)
{
    // Variable declarations!
    Clients clients;
    socklen_t clientAddrSize = sizeof(SA_IN);
    SA_IN clientAddr;
    int clientFD;
    char clientIP[INET_ADDRSTRLEN];
    char userName[MAX_USERNAME_SIZE + 10];

    // Accepting new connections!

    errHandle((clientFD = accept(serverFD, (SA *)&clientAddr, &clientAddrSize)), SOCK_ERROR, "Accept: Server side!");
    fprintf(stdout, "Accepting connections...\n");

    // Getting the Ip address 
    inet_ntop(AF_INET, &clientAddr, clientIP, sizeof(clientIP));
    fprintf(stdout, "Accepted connection from: %s...\n", clientIP);

    handleLogin(clientFD, userName);

    clients.clientFD = clientFD;
    strcpy(clients.userName, userName);
    // Returning the client's file descriptor.
    return clients;
}

void handleMessaging(struct Clients clients)
{
    char message[MAXLEN];
    char* formatted_userName = format_text(clients.userName,BOLD_RED);
    strcpy(message, formatted_userName);
    strcat(message, "Hello World!");
    char *newLine = strchr(message, '\n');
    *newLine = ':';
    send(clients.clientFD, message, strlen(message), 0);
    free(formatted_userName);
}

// TODO Use the client struct to store connected client info
// TODO Try to use threads / select to avoid non blocking code.
