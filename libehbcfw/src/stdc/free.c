#include <stdlib.h>

#include "mm.h"

void free(void *ptr)
{
    mm_free(ptr);
}
