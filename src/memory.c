#include "memory.h"

void *memset(void *dest, int c, size_t count)
{
    for (char *dest_c = dest; count > 0; count--) *dest_c++ = c;
    return dest;
}

void *memset16(void *dest, int c, size_t count)
{
    for (uint16_t *dest_c = dest; count > 0; count--) *dest_c++ = c;
    return dest;
}

void *memcpy(void *dest, const void *src, size_t len)
{
    char *dest_c = dest;
    const char *src_c = src;
    while (len--) *dest_c++ = *src_c++;
    return dest;
}
