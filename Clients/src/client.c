#include "../include/client.h"
/**
 * @brief Sets the socket passed to non-blocking mode.
 * @param sock_fd - The file descriptor.
 * @return void!
 */
void set_nonblocking(int sock_fd);

/**
 * @brief Checks for errors based on return value.
 * @param return_val - The value returned by the function call.
 * @param error_val- Expected value that the function returns incase of an error.
 * @param error_message - The error message.
 * @return The return value of the function.
 */
int check_error(int return_val, int error_val, char *error_message);

/**
 * @brief Sends the message to the server.
 * @param server_fd - The server's file descriptor.
 * @param username - The username of the client.
 * @return void
 */
void send_message(int server_fd, char *username);

int connect_to_server(int port, char *ip_address, char *username)
{
    int server_fd;           //> The server's file descriptor.
    int buffer_len = 50;     //> The approximate length of the welcome message.
    SA_IN server_address;    //> Server's address information.
    int addrlen;             //> Size of server_address.
    int bytes_received;      //> Bytes received.
    char buffer[buffer_len]; //> Welcome message!

    check_error((server_fd = socket(AF_INET, SOCK_STREAM, 0)), SOCK_ERROR, "Socket: failed to create socket!");
    fprintf(stdout, "Socket created!\n");
    set_nonblocking(server_fd);

    // Filling the server address information.
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    check_error((inet_pton(AF_INET, ip_address, &server_address.sin_addr)), SOCK_ERROR, "inet_pton: Failed to convert!");
    addrlen = sizeof(server_address);

    while (1)
    {
        if ((connect(server_fd, (SA *)&server_address, addrlen)) == -1)
        {
            if (errno == EINPROGRESS)
            {
                usleep(100000);
                continue;
            }
            else
            {
                perror("connect: failed to connect!\n");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            break;
        }
    }
    fprintf(stdout, "Connection established!\n");

    check_error((bytes_received = recv(server_fd, buffer, buffer_len - 1, 0)), SOCK_ERROR, "recv: failed to get welcome message!");
    printf("%s\n", buffer);
    usleep(100000);
    check_error((send(server_fd, username, strlen(username), 0)), SOCK_ERROR, "send: failed to send username!");
    usleep(100000);
    check_error((bytes_received = recv(server_fd, buffer, buffer_len - 1, 0)), SOCK_ERROR, "recv: failed to get welcome message!");
    return server_fd;
}

void join_broadcast(int server_fd, char *username)
{
    char message_received[MAXLEN]; //> Received messages.
    int bytes_received;            //> Bytes received.
    fd_set read_fds;               //> Set of file descriptors.
    int max_fd;                    //> The maximum number of file descriptors.

    set_nonblocking(STDIN_FILENO);
    set_nonblocking(server_fd);

    while (1)
    {
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(server_fd, &read_fds);

        max_fd = (STDIN_FILENO > server_fd) ? STDIN_FILENO : server_fd;

        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0 && errno != EINTR)
        {
            perror("select error");
            exit(EXIT_FAILURE);
        }

        // Check for incoming message from the server
        if (FD_ISSET(server_fd, &read_fds))
        {
            bytes_received = recv(server_fd, message_received, MAXLEN - 1, 0);
            if (bytes_received == -1)
            {
                if (errno != EWOULDBLOCK && errno != EAGAIN)
                {
                    perror("recv: failed to receive message!");
                    break;
                }
            }
            else if (bytes_received > 0)
            {
                message_received[bytes_received] = '\0';
                fprintf(stdout, "\033[1A%s\n", message_received);
            }
        }

        // Check for user input from stdin
        if (FD_ISSET(STDIN_FILENO, &read_fds))
        {
            send_message(server_fd, username);
        }
    }

    close(server_fd);
}

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
    int flags = fcntl(sock_fd, F_GETFL, 0);
    check_error(flags, -1, "fcntl: F_GETFL");
    check_error(fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK), -1, "fcntl: F_SETFL");
}

void send_message(int server_fd, char *username)
{
    int bytes_read;              //> Bytes read from stdin.
    int bytes_sent;              //> Bytes sent over the server socket.
    int username_length;         //> The length of the client's username.
    char message_buffer[MAXLEN]; //> The buffer in which the message will be stored.

    username_length = strlen(username);
    snprintf(message_buffer, MAXLEN, "\033[1;33m%s\033[0m: ", username);
    username_length = strlen(message_buffer);

    bytes_read = read(STDIN_FILENO, message_buffer + username_length, MAXLEN - username_length - 1);
    if (bytes_read == -1)
    {
        if (errno != EWOULDBLOCK && errno != EAGAIN)
        {
            perror("read: failed to read from stdin!");
            exit(EXIT_FAILURE);
        }
    }
    else if (bytes_read > 0)
    {
        message_buffer[username_length + bytes_read] = '\0';
        bytes_sent = send(server_fd, message_buffer, username_length + bytes_read, 0);
        check_error(bytes_sent, SOCK_ERROR, "send: failed to send message!");
    }
}
