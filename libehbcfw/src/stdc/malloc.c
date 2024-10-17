#include <stdlib.h>

#include "mm.h"

void *malloc(size_t size)
{
    return mm_alloc(size);
}
