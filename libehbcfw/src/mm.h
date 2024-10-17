#ifndef __MM_H__
#define __MM_H__

#include <stdint.h>
#include <assert.h>
#include <stddef.h>

#define BLOCK_SIZE      (4 * 1024)
#define CHUNK_MINSIZE   (BLOCK_SIZE / 16)  // 256 bytes

#define BF_CONT    0x02    // allocation is continued
#define BF_DIV     0x01

/*
Chunk Flags Truth Table
    DIV FULL    State
    0   0       Free
    0   1       Full
    1   0       Partial (S)
    1   1       Full (S)
*/

#define CF_LDIV     0x20
#define CF_LFULL    0x10
#define CF_RDIV     0x02
#define CF_RFULL    0x01

struct block_descriptor {
    uint8_t block_flags;
    uint8_t chunk_flags_l1;
    uint8_t chunk_flags_l2[2];  // [2n]: left, [2n+1]: right
    uint8_t chunk_flags_l3[4];
    uint8_t chunk_flags_l4[8];
} __attribute__((aligned(16)));

static_assert(sizeof(struct block_descriptor) == 16, "Invalid block descriptor struct size");

int mm_init(void *heap_start, unsigned long heap_size);
void *mm_alloc(unsigned long size);
unsigned long mm_free(void *ptr);  // returns allocation size, 0 if error
void *mm_realloc(void *ptr, unsigned long newsize);

#endif // __MM_H__
