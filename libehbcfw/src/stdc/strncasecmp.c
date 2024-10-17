#include "string.h"

#include <stdint.h>
#include <ctype.h>

int strncasecmp(const char *p1, const char *p2, size_t maxlen)
{
    while (maxlen--) {
        char c1 = toupper(*p1), c2 = toupper(*p2);
        if (c1 != c2) return ((unsigned char)c1 - (unsigned char)c2);
        if (!*p1++) break;
        if (!*p2++) break;
    }
    return 0;
}

int strcasecmp(const char *p1, const char *p2)
{
    return strncasecmp(p1, p2, SIZE_MAX);
}
