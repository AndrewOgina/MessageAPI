#include "../include/client.h"
#include <pthread.h>
#define PORT 8088

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(stderr,"Provide the: 1.Server address!\n             2.Your username!\n");
        exit(EXIT_FAILURE);
    }
    int server_fd = connect_to_server(PORT, argv[1]);

    join_broadcast(&server_fd);
    
}