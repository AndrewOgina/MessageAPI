#include "../include/client.h"

/**
 * @brief Checks if a function returned an error and handles it accordingly.
 * @param return_val - The value returned by the function.
 * @param error_val - The value the function will return if an error occurs.
 * @param error_message - The message displayed incase an error occurs.
 * @return - The value returned by the function
 * @warning - The program exits if an error occurs!
 */
int check_error(int return_val, int error_val, char *error_message);

/**
 * @brief A thread compatible function for receiving Messages.
 * @param p_server_fd  A void pointer to the server's file descriptor.
 * @return - A void pointer - NULL
 */
void *receive_message(void *p_server_fd);

int check_error(int return_val, int error_val, char *error_message)
{
    if (return_val == error_val)
    {
        perror(error_message);
        exit(EXIT_FAILURE);
    }
    return return_val;
}

void *receive_message(void *p_server_fd)
{
    char message_buffer[MAXLEN]; //>Holds incoming message.
    int size_received;           //>The size of the message received.
    int server_fd;               //>The server's file descriptor.
    server_fd = *(int *)p_server_fd;
    check_error((size_received = recv(server_fd, message_buffer, MAXLEN - 1, 0)), SOCK_ERROR, "recv: Failed to receive message!");

    // Null Termination!
    message_buffer[size_received] = '\0';
    fprintf(stdout, "%s\n", message_buffer);
    return NULL;
}

int connect_to_server(int port, char *server_address)
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
    fgets(message, sizeof(message), stdin);
    fprintf(stdout, "Sending:%s\n", message);

    if ((send(serverFD, message, strlen(message), 0)) == -1)
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
