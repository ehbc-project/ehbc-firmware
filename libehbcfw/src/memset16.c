#include "string.h"

#include <stdint.h>

void *memset16(void *dest, int c, size_t count)
{
    for (uint16_t *dest_c = dest; count > 0; count--) *dest_c++ = c;
    return dest;
}
