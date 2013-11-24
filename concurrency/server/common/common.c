#include "common.h"
#include <sys/socket.h>
#include <sys/types.h>

int open_listenfd(int port)
{
    int listenfd, optval = 1;
    struct sockaddr_in server_addr;

    /* Create a socket descriptor */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    /* listenfd will be an end point for all requests to port
       on any IP address for this host */
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((unsigned short)port);
    if (bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        return -1;

    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, LISTENQ) < 0)
        return -1;
    return listenfd;
}
