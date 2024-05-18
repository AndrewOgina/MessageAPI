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
    int server_fd;          // The server's file descriptor.
    int connect_status;     // The return value of connect
    socklen_t address_size; // Holds the size of sockaddr_in
    SA_IN client_address;   // Holds client's address information.

    // Creating the socket
    check_error((server_fd = socket(AF_INET, SOCK_STREAM, 0)), SOCK_ERROR, "client: failed to create socket!");
    fprintf(stdout, "Socket created successfully!\n");

    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(port);
    address_size = sizeof(SA_IN);

    check_error((inet_pton(AF_INET, server_address, &client_address.sin_addr)), SOCK_ERROR, "inet_pton: Failed!");
    check_error((connect_status = connect(server_fd, (SA*)&client_address, address_size)), SOCK_ERROR, "connect: Failed to connect to server!");
    fprintf(stdout, "Connected to server successfully!\n");
    return server_fd;
}

void join_broadcast(int *server_fd)
{
    pthread_t new_thread; //>An instance of pthread;
    char message[MAXLEN]; //>Buffer for messages being sent
    char welcome_msg[40];
    char username[USERNAME_SIZE]; //>Buffer for the username.
    int size_received;

    // Getting welcome!
    check_error((size_received = recv(*server_fd, welcome_msg, MAXLEN - 1, 0)), SOCK_ERROR, "recv: Failed to get welcome note!");
    welcome_msg[size_received] = '\0';
    puts(welcome_msg);
    // sending the username
    printf("Username:");
    fgets(username, USERNAME_SIZE - 2, stdin);
    check_error((send(*server_fd, username, strnlen(username,USERNAME_SIZE), 0)), SOCK_ERROR, "send: failed to send username!");

    // Receiving messages!
    while (1)
    {
        pthread_create(&new_thread, NULL, receive_message, &server_fd);
        fgets(message, MAXLEN - 1, stdin);
        check_error((send(*server_fd,message,strnlen(message,MAXLEN),0)),SOCK_ERROR,"send: Failed to send messages!");
    }
}
