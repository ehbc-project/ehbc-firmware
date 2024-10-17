#include "mm.h"

#include <string.h>

#define LOG(...)  // eat debug output

typedef unsigned long block_t;

static struct block_descriptor *pdt_list;
static block_t pdt_count;
static void *alloc_start;

int mm_init(void *heap_start, unsigned long heap_size)
{
    pdt_count = heap_size / BLOCK_SIZE;
    pdt_list = heap_start;
    unsigned long pdt_size = pdt_count * sizeof(struct block_descriptor);
    alloc_start = (void*)((unsigned long)heap_start + pdt_size);

    // align alloc_start to BLOCK_SIZE
    alloc_start = (void*)((((unsigned long)alloc_start + BLOCK_SIZE - 1) / BLOCK_SIZE) * BLOCK_SIZE);

    // init pdt to zero
    memset(pdt_list, 0, pdt_size);
    return 0;
}

static ptrdiff_t do_mm_alloc_chunk(block_t block, int level, int current, int idx)
{
    uint8_t flags, *chunk_flags_list, *child_flags_list;
    ptrdiff_t ret;

    switch (current) {
        case 1:
            chunk_flags_list = &pdt_list[block].chunk_flags_l1;
            child_flags_list = pdt_list[block].chunk_flags_l2;
            break;
        case 2:
            chunk_flags_list = pdt_list[block].chunk_flags_l2;
            child_flags_list = pdt_list[block].chunk_flags_l3;
            break;
        case 3:
            chunk_flags_list = pdt_list[block].chunk_flags_l3;
            child_flags_list = pdt_list[block].chunk_flags_l4;
            break;
        case 4:
            chunk_flags_list = pdt_list[block].chunk_flags_l4;
            child_flags_list = NULL;
            break;
        default:
            return -1;
    }

    flags = chunk_flags_list[idx];

    if (current == level) {
        // we should return a result and not recurse in here
        if (!(flags & (CF_LDIV | CF_LFULL))) {
            // allocate left chunk
            flags |= CF_LFULL;
            ret = 0;
            chunk_flags_list[idx] = flags;
            LOG("L%d: idx=%d flags=%02X ", current, idx << 1, flags);
            return ret;
        }

        flags = chunk_flags_list[idx >> 1];
        if (!(flags & (CF_RDIV | CF_RFULL))) {
            // allocate right chunk
            flags |= CF_RFULL;
            ret = (CHUNK_MINSIZE << (4 - current));
            chunk_flags_list[idx] = flags;
            LOG("L%d: idx=%d flags=%02X ", current, (idx << 1) + 1, flags);
            return ret;
        }
    } else {
        // we have to go more
        if (!(flags & CF_LFULL)) {
            // allocate left chunk
            ret = do_mm_alloc_chunk(block, level, current + 1, idx << 1);

            if (ret >= 0) {  // if success
                flags |= CF_LDIV;
                // check if the current chunk is full
                if ((child_flags_list[idx << 1] & (CF_LFULL | CF_RFULL)) == (CF_LFULL | CF_RFULL)) {
                    flags |= CF_LFULL;
                }
                chunk_flags_list[idx] = flags;
                LOG("L%d: idx=%d flags=%02X ", current, idx << 1, flags);
                return ret;
            }
        }

        if (!(flags & CF_RFULL)) {
            // allocate right chunk
            ret = do_mm_alloc_chunk(block, level, current + 1, (idx << 1) + 1);

            if (ret >= 0) {  // if success
                // check if the current chunk is full
                flags |= CF_RDIV;
                if ((child_flags_list[(idx << 1) + 1] & (CF_LFULL | CF_RFULL)) == (CF_LFULL | CF_RFULL)) {
                    flags |= CF_RFULL;
                }
                ret += (CHUNK_MINSIZE << (4 - current));
                chunk_flags_list[idx] = flags;
                LOG("L%d: idx=%d flags=%02X ", current, (idx << 1) + 1, flags);
                return ret;
            }
        }
    }
    return -1;
}

static ptrdiff_t mm_alloc_chunk(block_t block, int level)
{
    uint8_t l1flags = pdt_list[block].chunk_flags_l1;
    // avoid full chunks
    if ((l1flags & CF_LFULL) && (l1flags & CF_RFULL)) {
        return -1;
    }

    ptrdiff_t ret = do_mm_alloc_chunk(block, level, 1, 0);
    if (ret >= 0) {
        pdt_list[block].block_flags |= BF_DIV;
    }
    return ret;
}

void *mm_alloc(unsigned long size)
{
    LOG("alloc: size=%lu ", size);
    if (size > BLOCK_SIZE * pdt_count) {  // too big!
        LOG("failed\n");
        return NULL;
    } else if (size > BLOCK_SIZE / 2) {  // allocate without slicing block
        block_t req = (size + BLOCK_SIZE - 1) / BLOCK_SIZE, contig = 0, start = 0;

        // find contiguous free blocks
        for (block_t i = 0; i < pdt_count && contig < req; i++) {
            if (!(pdt_list[i].chunk_flags_l1 & (CF_LFULL | CF_RFULL)) && !(pdt_list[i].block_flags & BF_DIV)) {
                if (!contig) {
                    start = i;
                }
                contig++;
            } else {
                contig = 0;
            }
        }

        LOG("blockunit start=%lu count=%lu ", start, req);

        if (contig < req) {
            LOG("failed\n");
            return NULL;
        }
        LOG("ptr=%p\n", (uint8_t*)alloc_start + start * BLOCK_SIZE);

        // set flags
        for (block_t i = 0; i < req; i++) {
            pdt_list[start + i].block_flags = BF_CONT;
            pdt_list[start + i].chunk_flags_l1 = CF_LFULL | CF_RFULL;
        }
        // terminate allocation by clearing BF_CONT
        pdt_list[start + req - 1].block_flags = 0;

        return (uint8_t*)alloc_start + start * BLOCK_SIZE;
    } else {
        int chunk_level;
        ptrdiff_t chunk_offs;

        if (size > BLOCK_SIZE / 4) {  // allocate a level 1 chunk
            chunk_level = 1;
        } else if (size > BLOCK_SIZE / 8) {  // allocate a level 2 chunk
            chunk_level = 2;
        } else if (size > BLOCK_SIZE / 16) {  // allocate a level 3 chunk
            chunk_level = 3;
        } else {  // allocate a level 4 chunk
            chunk_level = 4;
        }

        LOG("chunk\n");

        for (block_t block = 0; block < pdt_count; block++) {
            // skip if the block is full
            if ((pdt_list[block].chunk_flags_l1 & (CF_LFULL | CF_RFULL)) == (CF_LFULL | CF_RFULL)) {
                continue;
            }

            // try allocation
            LOG("\tblock=%lu ", block);
            chunk_offs = mm_alloc_chunk(block, chunk_level);
            if (chunk_offs >= 0) {
                LOG("allocated ptr=%p\n", (uint8_t*)alloc_start + block * BLOCK_SIZE + chunk_offs);
                return (uint8_t*)alloc_start + block * BLOCK_SIZE + chunk_offs;
            }
            LOG("\n");
        }

        // allocation failed
        LOG("failed\n");
        return NULL;
    }
}

// returns freed chunk level, negative if error
static int do_mm_free_chunk(block_t block, ptrdiff_t chunk_offs, int level)
{
    uint8_t flags, *chunk_flags_list, *child_flags_list;
    int ret = level;

    switch (level) {
        case 1:
            chunk_flags_list = &pdt_list[block].chunk_flags_l1;
            child_flags_list = pdt_list[block].chunk_flags_l2;
            break;
        case 2:
            chunk_flags_list = pdt_list[block].chunk_flags_l2;
            child_flags_list = pdt_list[block].chunk_flags_l3;
            break;
        case 3:
            chunk_flags_list = pdt_list[block].chunk_flags_l3;
            child_flags_list = pdt_list[block].chunk_flags_l4;
            break;
        case 4:
            chunk_flags_list = pdt_list[block].chunk_flags_l4;
            child_flags_list = NULL;
            break;
        default:
            return -1;
    }

    int chunk_idx = chunk_offs / (CHUNK_MINSIZE << (4 - level));
    flags = chunk_flags_list[chunk_idx >> 1];

    if (chunk_idx & 1) {
        if (flags & CF_RDIV) {
            ret = do_mm_free_chunk(block, chunk_offs, level + 1);
        }
        flags &= ~CF_RFULL;
    } else {
        if (flags & CF_LDIV) {
            ret = do_mm_free_chunk(block, chunk_offs, level + 1);
        }
        flags &= ~CF_LFULL;
    }

    if (child_flags_list) {
        // cleanup current level when the child level is free
        if (!(child_flags_list[chunk_idx] & (CF_LDIV | CF_LFULL | CF_RDIV | CF_RFULL))) {
            flags &= (chunk_idx & 1) ? ~(CF_RDIV | CF_RFULL) : ~(CF_LDIV | CF_LFULL);
        }
    }
    chunk_flags_list[chunk_idx >> 1] = flags;
    LOG("L%d: idx=%d flags=%02X ", level, chunk_idx, flags);

    return ret;
}

// returns freed chunk level, negative if error
static int mm_free_chunk(block_t block, ptrdiff_t chunk_offs)
{
    int ret = do_mm_free_chunk(block, chunk_offs, 1);
    if (!(pdt_list[block].chunk_flags_l1 & (CF_LDIV | CF_LFULL | CF_RDIV | CF_RFULL))) {
        pdt_list[block].block_flags &= ~BF_DIV;
    }
    return ret;
}

unsigned long mm_free(void *ptr)
{
    if ((unsigned long)ptr < (unsigned long)alloc_start ||
        (unsigned long)alloc_start + pdt_count * BLOCK_SIZE <= (unsigned long)ptr) {
        return 0;
    }

    ptrdiff_t ptr_offs = (ptrdiff_t)ptr - (ptrdiff_t)alloc_start;
    block_t block = (unsigned long)ptr_offs / BLOCK_SIZE;
    ptrdiff_t chunk_offs = ptr_offs - block * BLOCK_SIZE;
    int l4idx = chunk_offs / (BLOCK_SIZE << 1);
    int l3idx = l4idx >> 1;
    int l2idx = l3idx >> 1;
    unsigned long ret = 0;

    LOG("free: ptr=%p ", ptr);
    while (pdt_list[block].block_flags & BF_CONT) {  // multiple blocks
        LOG("blockunit block=%lu", block);
        memset(&pdt_list[block], 0, sizeof(struct block_descriptor));  // clear block
        block++;
        ret += BLOCK_SIZE;
    }

    if (pdt_list[block].block_flags & BF_DIV) {  // free chunk
        LOG("chunk\n\tblock=%lu ", block);
        int chunk_level = mm_free_chunk(block, chunk_offs);
        LOG("\n");
        ret += CHUNK_MINSIZE << (4 - chunk_level);
    } else {  // whole block
        LOG("blockunit block=%lu\n", block);
        memset(&pdt_list[block], 0, sizeof(struct block_descriptor));  // clear block
        ret += BLOCK_SIZE;
    }
    return ret;
}

void *mm_realloc(void *ptr, unsigned long newsize)
{
    int orig_size = mm_free(ptr);
    if (!orig_size) {
        return NULL;
    }

    void *newptr = mm_alloc(newsize);

    if (ptr != newptr) {
        memcpy(newptr, ptr, orig_size);
    }

    return newptr;
}
