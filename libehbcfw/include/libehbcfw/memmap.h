#ifndef __LIBEHBCFW_MEMMAP_H__
#define __LIBEHBCFW_MEMMAP_H__

#include <stdint.h>

#define MEM_UNMAPPED    0x00000001
#define MEM_OCCUPIED    0x00000002
#define MEM_MMIO        0x00000004
#define MEM_READONLY    0x00000008

struct memmap {
    struct memmap *next;
    void *start, *end;
    uint32_t flags;
};

#endif // __LIBEHBCFW_MEMMAP_H__
