#include "../common/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

void sigchld_handler(int sig)
{
    /* Since SIGCHLD signals are blocked while the SIGCHLD handler is executing,
       and since UNIX signals are not queued, the SIGCHLD handler must be
       prepared to reap multiple zombie children. 
    */
    while (waitpid(-1, 0, WNOHANG) > 0)
        ;
    return;
}
int main(int argc, char **argv)
{
    int listenfd, connfd, port;
    socklen_t client_len = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[1]);

    signal(SIGCHLD, sigchld_handler);
    listenfd = open_listenfd(port);
    while (1) {
        connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_len);
        if (fork() == 0) {
            close(listenfd);
            echo(connfd);
            close(connfd);
            exit(0);
        }
        close(connfd);  /* Parent closes connected socket (important!) */
    }
    return 0;
}
