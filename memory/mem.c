#include "mem.h"
#include <stdlib.h>
#include <stdio.h>

static struct Slab slab;

void mem_init(size_t object_size)
{
    static int flag = 0;
    int i, object_count = 0;
    if (flag == 0) {
        flag = 1;
        slab.s_block = malloc(MEM_SIZE);
        slab.s_object_size = object_size;
        slab.s_free_head = 0;
        object_count = MEM_SIZE / object_size;
        slab.s_free = (int*) malloc(object_count * sizeof(int));
        if (slab.s_block == NULL || slab.s_free == NULL) {
            fprintf(stderr, "mem_init() failed.");
            exit(1);
        }
        for (i = 0; i < object_count; i++) {
            slab.s_free[i] = i + 1;
        }
        slab.s_free[--i] = -1;
    }
}

void *mem_alloc()
{
    int free_head;
    if (slab.s_free_head != -1) {
        free_head = slab.s_free_head;
        slab.s_free_head = slab.s_free[free_head];
        return slab.s_block + free_head * slab.s_object_size;
    }
    return NULL;
}

void mem_free(void *ptr){
    int object_index;
    if (((ptr - slab.s_block) % slab.s_object_size) != 0) {
        fprintf(stderr, "mem_free(), freeing unknown pointer");
        exit(2);
    }
    object_index = (ptr - slab.s_block) / slab.s_object_size;
    slab.s_free[object_index] = slab.s_free_head;
    slab.s_free_head = object_index;
}
