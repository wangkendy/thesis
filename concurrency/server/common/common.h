#ifndef __COMMON_WANG__
#define __COMMON_WANG__
#define LISTENQ 10
#define BUF_SIZE 8192
int open_listenfd(int port);
int echo(int connfd);
#endif // __COMMON_WANG__
