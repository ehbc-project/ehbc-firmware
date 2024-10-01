#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>

void *memset(void *dest, int c, size_t count);
void *memset16(void *dest, int c, size_t count);
void *memcpy(void *dest, const void *src, size_t len);

size_t strnlen(const char *str, size_t maxlen);
size_t strlen(const char *str);

#endif // __STRING_H__
