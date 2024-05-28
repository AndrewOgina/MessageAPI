#include "../include/client.h"
#define PORT 8088

int main()
{   
    char* nl;
    char local_ip[] = "127.0.0.1";
    char username[USERNAME_SIZE];
    printf("Username:");
    fgets(username,USERNAME_SIZE,stdin);

    //Removing the new line.
    nl = strchr(username,'\n');
    *nl = '\0';
    int server_fd = connect_to_server(PORT,local_ip,username);
    join_broadcast(server_fd,username);
}