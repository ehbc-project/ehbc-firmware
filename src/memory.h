#ifndef MEMORY_H__
#define MEMORY_H__

#include <stddef.h>
#include <stdint.h>

void *memset(void *dest, int c, size_t count);
void *memset16(void *dest, int c, size_t count);
void *memcpy(void *dest, const void *src, size_t len);

#endif  // MEMORY_H__
