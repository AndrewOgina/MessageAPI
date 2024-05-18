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

/**
 * @brief Sets the socket to nonblocking mode.
 * @param sock_fd - The socket file descriptor.
 * @return Void!
 */
void set_nonblocking(int sock_fd);

int check_error(int return_val, int error_val, char *error_message)
{
    if (return_val == error_val)
    {
        perror(error_message);
        exit(EXIT_FAILURE);
    }
    return return_val;
}

void set_nonblocking(int sock_fd)
{
    int flags;
    check_error((flags = fcntl(sock_fd, F_GETFL, 0)), SOCK_ERROR, "fcntl F_GETFL: Failed!");
    check_error((fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK)), SOCK_ERROR, "fcntl F_SETFL: Failed!");
}

void *receive_message(void *p_server_fd)
{
    char message_buffer[MAXLEN]; //>Holds incoming message.
    int size_received;           //>The size of the message received.
    int server_fd;               //>The server's file descriptor.
    server_fd = *(int *)p_server_fd;
    while (1)
    {
        size_received = recv(server_fd, message_buffer, MAXLEN - 1, 0);

        if (size_received < 0)
        {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
            {
                usleep(10000); // Sleeps for 10ms
                continue;
            }
            else
            {
                check_error(size_received, -1, "recv failed!");
            }
        }
        else if (size_received == 0)
        {
            printf("Server Closed connection!");
            break;
        }
        else
        {
    // Null Termination!
    message_buffer[size_received] = '\0';
    fprintf(stdout, "%s\n", message_buffer);
        }
    }
    pthread_exit(NULL);
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

    set_nonblocking(server_fd);

    memset(&client_address, 0, sizeof(client_address));
    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(port);
    address_size = sizeof(SA_IN);

    check_error((inet_pton(AF_INET, server_address, &client_address.sin_addr)), SOCK_ERROR, "inet_pton: Failed!");
    check_error((connect_status = connect(server_fd, (SA *)&client_address, address_size)), SOCK_ERROR, "connect: Failed to connect to server!");
    fprintf(stdout, "Connected to server successfully!\n");
    return server_fd;
}

void join_broadcast(int *server_fd)
{
    pthread_t new_thread; //>An instance of pthread;
    char message[MAXLEN]; //>Buffer for messages being sent
    char welcome_msg[40];
    char username[USERNAME_SIZE]; //>Buffer for the username.
    int size_received,message_size,size_sent;
    int username_size;
    int bytes_sent;

    // Getting welcome!
    check_error((size_received = recv(*server_fd, welcome_msg, MAXLEN - 1, 0)), SOCK_ERROR, "recv: Failed to get welcome note!");
    welcome_msg[size_received] = '\0';
    puts(welcome_msg);

    // sending the username
    printf("Username:");
    fgets(username, USERNAME_SIZE - 2, stdin);
    username_size = strnlen(username,USERNAME_SIZE);
    check_error((send(*server_fd, username, username_size, 0)), SOCK_ERROR, "send: failed to send username!");

    // Receiving messages!
    check_error((pthread_create(&new_thread, NULL, receive_message, &server_fd)), EAGAIN, "Pthreads failed!");

    while (1)
    {
        fgets(message, MAXLEN - 1, stdin);
        message_size = strnlen(message,MAXLEN);
        memmove(message+username_size,message,message_size+1);
        memcpy(message,username,username_size);
        size_sent = message_size + username_size +1;
        if((bytes_sent =send(*server_fd, message, size_sent, 0))<0)
        {
            if(errno != EWOULDBLOCK || errno != EAGAIN)
            {
                close(*server_fd);
                check_error(bytes_sent,-1,"Failed to send!");
            }
        }
        else
        {
            printf(">>sent<<");
        }
    }
    pthread_join(new_thread, NULL);
    close(*server_fd);;
}
