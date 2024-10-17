#include <string.h>

#include <stdint.h>

int strncmp(const char *p1, const char *p2, size_t maxlen)
{
    while (maxlen--) {
        if (*p1 != *p2) return (*(unsigned char*)p1 - *(unsigned char*)p2);
        if (!*p1++) break;
        if (!*p2++) break;
    }
    return 0;
}

int strcmp(const char *p1, const char *p2)
{
    return strncmp(p1, p2, SIZE_MAX);
}
