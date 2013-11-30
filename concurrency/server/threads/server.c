#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <pthread.h>
#include "../common/common.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

void *service(void *vargp);

int main(int argc, char **argv)
{
    int listenfd, *connfdp, port;
    socklen_t clientlen = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;
    pthread_t tid;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[1]);
    
    listenfd = open_listenfd(port);
    while (1) {
        connfdp = malloc(sizeof(int));
        *connfdp = accept(listenfd, (struct sockaddr*)&client_addr, &clientlen);
        pthread_create(&tid, NULL, service, connfdp);
    }
}

/* Thread routine */
void *service(void *vargp)
{
    int connfd = *((int*)vargp);
    pthread_detach(pthread_self());
    free(vargp);
    echo(connfd);
    close(connfd);
    return NULL;
}
