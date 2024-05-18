#include "../include/client.h"
#include <pthread.h>
#define PORT 8088

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr,"Provide the server address please!\n");
        exit(EXIT_FAILURE);
    }
    int server_fd = connect_to_server(PORT, argv[1]);

    join_broadcast(&server_fd);
    
}