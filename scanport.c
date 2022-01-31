#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "errno.h"

void getHost(struct sockaddr_in *my_addr,char* hostname,struct hostent *host);

void scanPort(int socket_fd,struct sockaddr_in my_addr);


int main(int argc, char *argv[])
{
    struct sockaddr_in my_addr;
    struct hostent *host = NULL;
    int socket_fd = 0;
    char *hostname = argv[1];
   
    memset(&my_addr,0,sizeof(my_addr));
    my_addr.sin_family = AF_INET;

    getHost(&my_addr,hostname,host);

    scanPort(socket_fd,my_addr);

    return 0;
}


void getHost(struct sockaddr_in *my_addr,char* hostname,struct hostent *host)
{
    if(isdigit(hostname[0]))
    {
        my_addr->sin_addr.s_addr = inet_addr(hostname);
    }

    else if ((host = gethostbyname(hostname)) != 0)
    {
        memcpy(&my_addr->sin_addr, host->h_addr_list[0], host->h_length);
    }

    else
    {
        printf("Error resolving hostname");
        exit(EXIT_FAILURE);
    }
}

void scanPort(int socket_fd,struct sockaddr_in my_addr)
{
    printf("Scanning ports...\n");

    for (int port = 1; port <= 65535; port++)
    {
        socket_fd = socket(AF_INET, SOCK_STREAM,0);
        if(socket_fd < 0)
        {
            perror("Socket: ");
            exit(EXIT_FAILURE);
        }

        my_addr.sin_port = htons(port);

        int status = connect(socket_fd , (struct sockaddr*)&my_addr , sizeof my_addr);

        if(status == -1)
        {
            //printf("%s %-5d %s\r" , hostname , i, strerror(errno));
            close(socket_fd);
        }

        else
        {
            printf("Port %d is open...\n", port);
        }

        close(socket_fd);
    }
}
