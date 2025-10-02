#include "sh.h"

char *program_name = NULL;

void *xmalloc(size_t sz)
{
    void *p;

    p = calloc(1, sz);
    if (!p)
        exit(3);
    return p;
}

void *xrealloc(void *ptr, size_t sz)
{
    void *p;

    if (!ptr) 
        return xmalloc(sz);
    p = realloc(ptr, sz);
    if (!p)
        exit(3);
    return p;
}