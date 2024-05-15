#include "../include/server.h"

/**
 * @brief Checks if a function encountered an error.
 * @param return_val - The value returned by a function.
 * @param error_val - The value returned by unsuccessful function call.
 * @param error_message - The message printed incase of an error.
 * @return The value returned upon successful function call.
 */
int check_error(int return_val, int error_val, char *error_message);

/**
 * @brief Receives message from clients and broadcasts it to everyone!
 * @param sender_fd The sender's file descriptor.
 * @param epoll_fd -The epoll file descriptor.
 * @param clients - An array of Client structs.
 * @return void!
 */
void receive_and_broadcast(int sender_fd, int epoll_fd, struct Client *clients);

/**
 * @brief Retrieves the session username of connected clients.
 * @param client_fd - The client's file descriptor.
 * @param username_buffer - Buffer storing the username.
 * @return void!
 */
void get_username(int client_fd, char *username_buffer);

int check_error(int return_val, int error_val, char *error_message)
{
    if (return_val == error_val)
    {
        perror(error_message);
        exit(EXIT_FAILURE);
    }
    return return_val;
}

void get_username(int client_fd, char *username_buffer)
{
    char welcome_message[] = "\033[1;31mWELCOME TO OTP.\033[0m\n"; //> The welcome message!
    char login_feedback[] = "Login Successful!\n";                 //> Login  message feedback!
    int username_size;                                             //> The username size.

    // Sending welcome note!
    check_error((send(client_fd, welcome_message, strlen(welcome_message), 0)), SOCK_ERROR, "Send: Welcome msg: Server side\n");

    // Receiving the username!
    check_error((username_size = recv(client_fd, username_buffer, USERNAME_SIZE - 1, 0)), SOCK_ERROR, "Receive: Username: Server side!");
    username_buffer[username_size] = '\0';
    fprintf(stdout, "Username: \033[1m%s\033[0m\n", username_buffer);

    // Confirmatory message!
    check_error((send(client_fd, login_feedback, strlen(login_feedback), 0)), SOCK_ERROR, "Send: successful login message failed to be sent!");
}

int setup_server(int port, int sock_type, int backlog)
{
    int server_fd;     //> The server's file descriptor!
    SA_IN server_addr; //> The struct containing the address information of the server!

    // 1. Creating the socket.

    check_error((server_fd = socket(AF_INET, sock_type, 0)), SOCK_ERROR, "Socket: Server side!\n");
    fprintf(stdout, "Socket created!\n");

    // 2. Filling the server_addr struct.

    server_addr.sin_family = AF_INET; //> Ipv4
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // 3. Binding socket to address and port.

    check_error((bind(server_fd, (SA *)&server_addr, sizeof(server_addr))), SOCK_ERROR, "Bind: Server side!\n");
    fprintf(stdout, "Socket binded to an address!\n");

    // 4. Listen for clients.

    check_error((listen(server_fd, backlog)), SOCK_ERROR, "Listen: Server side!\n");
    fprintf(stdout, "Listening...\n");

    // 5. Returning the server's file descriptor.
    return server_fd;
}

void receive_and_broadcast(int sender_fd, int epoll_fd, struct Client *clients)
{
    char message[MAXLEN]; //> Buffer to store the message.
    ssize_t bytes_received;
    bytes_received = recv(sender_fd, message, MAXLEN - 1, 0);

    if (bytes_received <= 0)
    {
        fprintf(stdout, "Client disconnected!\n");
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sender_fd, NULL);

        close(sender_fd);
        for (int j = 0; j < MAX_CLIENTS; j++)
        {
            if (clients[j].client_fd == sender_fd)
            {
                clients[j].state = false;
                break;
            }
        }
    }
    else
    {

        message[bytes_received] = '\0';
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if ((clients[i].state == ONLINE) && (clients[i].client_fd != sender_fd))
            {
                check_error((send(clients[i].client_fd, message, strlen(message), 0)), SOCK_ERROR, "send: broadcasting!");
            }
        }
    }
}

void handle_connections(int server_fd)
{

    struct epoll_event event;            //> Struct holds new epoll events.
    Client clients[MAX_CLIENTS];         //> Array to store clients.
    socklen_t addr_size = sizeof(SA_IN); //> Client address size.
    SA_IN client_addr;                   //> Struct holding client address information.
    int client_fd, epoll_fd;             //> File descriptor.
    int num_events;                      //> The number of fds available returned by epoll_wait.

    // Creating an epoll instance
    check_error((epoll_fd = epoll_create1(0)), SOCK_ERROR, "epoll_create1:server side!");

    // Adding server_fd to epoll
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    check_error((epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event)), SOCK_ERROR, "epoll_ctl: server side!");

    // Set all clients to false
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        clients[i].state = false;
    }

    // Infinite loop for accepting clients and sending and receiving messages!
    while (1)
    {
        struct epoll_event events[MAX_CLIENTS];
        check_error((num_events = epoll_wait(epoll_fd, events, MAX_CLIENTS, -1)), SOCK_ERROR, "epoll_wait: Server side!"); // -1 meaning indefinite timeout.

        for (int i = 0; i < num_events; i++)
        {
            if (events[i].data.fd == server_fd)
            {
                if ((client_fd = accept(server_fd, (SA *)&client_addr, &addr_size)) == SOCK_ERROR)
                {
                    perror("accept: Server side!");
                    continue;
                }

                // Add connected clients to epoll
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = client_fd;

                if ((epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event)) == SOCK_ERROR)
                {
                    perror("epoll_ctl: Adding clients: Server side");
                    continue;
                }

                int clientNo; // Number of connected clients

                for (clientNo = 0; clientNo < MAX_CLIENTS; clientNo++)
                {
                    if (clients[clientNo].state == false)
                    {
                        char username[USERNAME_SIZE + 15];
                        clients[clientNo].client_fd = client_fd;
                        clients[clientNo].state = true;
                        fprintf(stdout, "New client connected!\n");
                        get_username(clients[clientNo].client_fd, username);
                        strncpy(clients[clientNo].username, username, USERNAME_SIZE);
                        break;
                    }
                }

                if (clientNo == MAX_CLIENTS)
                {
                    fprintf(stdout, "Maximum number of clients attained!\n");
                    close(client_fd);
                }
            }
            else
            {
                int sender_fd = events[i].data.fd; //> The sender's file descriptor.
                receive_and_broadcast(sender_fd, epoll_fd, clients);
            }
        }
    }
}

// Note I am giving username prefixing responsibilities to the client.
