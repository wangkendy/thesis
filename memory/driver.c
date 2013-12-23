#include "mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//#define OBJECT_SIZE 800
#define TEST_COUNT 100

int main(int argc, char *argv[])
{
    int i;
    int OBJECT_SIZE = 0;
    void *ptr_arr[TEST_COUNT] = {NULL};
    struct timeval tv;
    double sec1, sec2;
    double sec3, sec4;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <object_isze>\n", argv[0]);
        exit(1);
    }
    OBJECT_SIZE = atoi(argv[1]);

    mem_init(OBJECT_SIZE);
   
    printf("hello world");

    gettimeofday(&tv, NULL);
    sec1 = tv.tv_sec + tv.tv_usec / 1000000.0;
    //printf("start time:%f\n", sec1);
    for (i = 0; i < TEST_COUNT; i++) {
        ptr_arr[i] = mem_alloc();
        //printf("ptr_arr[%d]:%p\n", i, ptr_arr[i]);
    }

    for (i = 0; i < TEST_COUNT; i++) {
        mem_free(ptr_arr[i]);
    }
    gettimeofday(&tv, NULL);
    sec2 = tv.tv_sec + tv.tv_usec / 1000000.0;
    //printf("end time:%f\n", sec2);
    printf("time elapse:%f\n", sec2 - sec1);

    gettimeofday(&tv, NULL);
    sec3 = tv.tv_sec + tv.tv_usec / 1000000.0;
    //printf("start time:%f\n", sec3);
    for (i = 0; i < TEST_COUNT; i++) {
        //printf("i=%d\n", i);
        ptr_arr[i] = malloc(OBJECT_SIZE);
        if (!ptr_arr[i]) {
            printf("malloc failed\n");
            exit(1);
        }
    }
    
    for (i = 0; i < TEST_COUNT; i++) {
        free(ptr_arr[i]);
    }
    gettimeofday(&tv, NULL);
    sec4 = tv.tv_sec + tv.tv_usec / 1000000.0;
    //printf("end time:%f\n", sec4);
    printf("time elapse:%f\n", sec4 - sec3);
    return 0;
}
