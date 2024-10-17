#include <stdlib.h>

#include "mm.h"

void *realloc(void *ptr, size_t newsize)
{
    return mm_realloc(ptr, newsize);
}
