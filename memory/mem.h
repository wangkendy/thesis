#ifndef __MEM_WXF__
#define __MEM_WXF__

#include <stddef.h>
#define MEM_SIZE 10*1024*1024   //10M

struct Slab {
    void *s_block;
    int s_object_size;
    int s_free_head;
    int *s_free;
};

void mem_init(size_t object_size);
void *mem_alloc();
void mem_free(void *ptr);

#endif //__MEM_WXF__
