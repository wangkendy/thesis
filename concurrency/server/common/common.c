#include "common.h"
#include <stdio.h>
#include <strings.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int open_listenfd(int port)
{
    int listenfd, optval = 1;
    struct sockaddr_in server_addr;

    /* Create a socket descriptor */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    /* Eliminates "Address already in use" error from bind */
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                (const void*)&optval, sizeof(int)) < 0)
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

int echo(int connfd)
{
    unsigned char buf[BUF_SIZE];
    int nbyte, i;
    for (;;) {
        nbyte = read(connfd, buf, BUF_SIZE);
        if (nbyte == 0) {
            printf("*** Disconnected.\n");
            break;
        }
        write(connfd, buf, nbyte);
        /*
        for (i = 0; i < nbyte; i++)
            printf("%c", buf[i]);
        fflush(stdout);
        */
    }
    return 0;
}
