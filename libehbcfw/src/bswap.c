#include <bswap.h>
#include <asm/bswap.h>

#if !defined(__HAVE_ARCH_BSWAP16) && !defined(__HAVE_BUILTIN_BSWAP16)
uint16_t _bswap16(uint16_t val)
{
    return ((val & 0x00FF) << 8) | ((val & 0xFF00) >> 8);
}

#endif

#if !defined(__HAVE_ARCH_BSWAP32) && !defined(__HAVE_BUILTIN_BSWAP32)
uint32_t _bswap32(uint32_t val)
{
    return 
        ((val & 0x000000FF) << 24) | ((val & 0x0000FF00) << 8) |
        ((val & 0xFF000000) >> 24) | ((val & 0x00FF0000) >> 8);
}

#endif

#if !defined(__HAVE_ARCH_BSWAP64) && !defined(__HAVE_BUILTIN_BSWAP64)
uint64_t _bswap64(uint64_t val)
{
    return 
        ((val & 0x00000000000000FF) << 56) |
        ((val & 0x000000000000FF00) << 40) |
        ((val & 0x0000000000FF0000) << 24) |
        ((val & 0x00000000FF000000) << 8) |
        ((val & 0x000000FF00000000) >> 8) |
        ((val & 0x0000FF0000000000) >> 24) |
        ((val & 0x00FF000000000000) >> 40) |
        ((val & 0xFF00000000000000) >> 56);
}

#endif
