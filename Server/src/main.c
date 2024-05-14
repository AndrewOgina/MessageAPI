#include "../include/server.h"
#define PORT 8088
#define BACKLOG 10

int main(int argc,char* argv[])
{
    int server_fd; //> The server's file descriptor.
    server_fd = setup_server(PORT,SOCK_STREAM,BACKLOG);
    handle_connections(server_fd); 
}