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
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    unsigned int size = 0;
    //pthread_attr_getstacksize(&attr,&size);
    //printf("stack size :%d\n",size);
    size = 1024 * 1024;
    //pthread_attr_setstacksize(&attr,size);
    pthread_attr_getstacksize(&attr,&size);
    printf("stack size :%d\n",size);
    return 0;
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
