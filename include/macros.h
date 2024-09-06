#ifndef EHBCFW_MACROS_H__
#define EHBCFW_MACROS_H__

#include <stdint.h>

#include <ehbcfw/byteswap.h>

#ifdef BYTE_ORDER_BIG_ENDIAN
#define htole16(v) byteswap16(v)
#define htobe16(v) (v)
#define htole32(v) byteswap32(v)
#define htobe32(v) (v)
#define htole64(v) byteswap64(v)
#define htobe64(v) (v)

#else
#define htole16(v) (v)
#define htobe16(v) byteswap16(v)
#define htole32(v) (v)
#define htobe32(v) byteswap32(v)
#define htole64(v) (v)
#define htobe64(v) byteswap64(v)

#endif

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define TEST_FLAG(val, mask) (((val) & mask) == mask)

#endif
