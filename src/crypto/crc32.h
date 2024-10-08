#ifndef __CRC32_H__
#define __CRC32_H__

#include <stdint.h>

uint32_t crc32(const void *buf, unsigned long len, uint32_t crc);

#endif // __CRC32_H__
