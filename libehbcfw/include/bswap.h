#ifndef ___BSWAP_H__
#define ___BSWAP_H__

#include <stdint.h>

#include <libehbcfw/asm/processor.h>

uint16_t bswap16(uint16_t val);
uint32_t bswap32(uint32_t val);
uint64_t bswap64(uint64_t val);

#if defined(__PROCESSOR_BIG_ENDIAN)
#define CONV_BE_16(v) (v)
#define CONV_BE_32(v) (v)
#define CONV_BE_64(v) (v)

#define CONV_LE_16(v) bswap16(v)
#define CONV_LE_32(v) bswap32(v)
#define CONV_LE_64(v) bswap64(v)

#elif defined(__PROCESSOR_LITTLE_ENDIAN)
#define CONV_BE_16(v) bswap16(v)
#define CONV_BE_32(v) bswap32(v)
#define CONV_BE_64(v) bswap64(v)

#define CONV_LE_16(v) (v)
#define CONV_LE_32(v) (v)
#define CONV_LE_64(v) (v)

#else
#error Processor endianness is unknown

#endif

#endif // ___BSWAP_H__
