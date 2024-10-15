#include <libehbcfw/mpool.h>

#include <string.h>
#include <stdint.h>
#include <macros.h>

// bitmap memory pool
#define MSLOT_FREE      0
#define MSLOT_USING     1
#define MSLOT_USEEND    2

static int set_bitmap(struct mpool *pool, unsigned long slot, int value)
{
    if (slot >= pool->slots) {
        return -1;
    }

    uint8_t *bitmap = (uint8_t*)pool->bitmap + (slot >> 2);
    *bitmap &= ~(3 << ((slot & 3) << 1));
    *bitmap |= (value & 3) << ((slot & 3) << 1);
    return 0;
}

static int get_bitmap(struct mpool *pool, unsigned long slot)
{
    if (slot >= pool->slots) {
        return -1;
    }

    uint8_t *bitmap = (uint8_t*)pool->bitmap + (slot >> 2);
    return (*bitmap >> ((slot & 3) << 1)) & 3;
}

unsigned long mpool_init(struct mpool *pool, void *base, unsigned long size)
{
    pool->bitmap = base;
    pool->slots = size / MPOOL_SLOT_SIZE;
    pool->bitmap_size = ((pool->slots >> 2) + 15) & ~15;  // align to 16 bytes
    pool->slots -= (pool->bitmap_size + MPOOL_SLOT_SIZE - 1) / MPOOL_SLOT_SIZE;
    pool->data = (uint8_t*)base + pool->bitmap_size;
    pool->data_size = pool->slots * MPOOL_SLOT_SIZE;
    pool->end = (uint8_t*)pool->data + pool->data_size - 1;
    memset(base, 0, size);
    return pool->data_size;
}

void *mpool_alloc(struct mpool *pool, unsigned long size)
{
    if (!size) return NULL;

    unsigned long required_slots = (size + (MPOOL_SLOT_SIZE - 1)) / MPOOL_SLOT_SIZE;
    void *slot_begin_ptr = NULL;
    unsigned long slot_begin;
    unsigned long free_slots = 0;
    
    // find contiguous free slots
    for (unsigned long i = 0; i < pool->slots; i++) {
        if (get_bitmap(pool, i) == MSLOT_FREE) {
            if (!free_slots) {
                slot_begin_ptr = (uint8_t*)pool->data + i * MPOOL_SLOT_SIZE;
                slot_begin = i;
            }
            free_slots++;
        } else {
            free_slots = 0;
            slot_begin_ptr = NULL;
        }

        if (free_slots == required_slots) {
            break;
        }
    }

    // no available free slots
    if (!slot_begin_ptr) {
        return NULL;
    }

    // set bitmap
    for (unsigned long i = slot_begin; i < slot_begin + free_slots; i++) {
        if (i == slot_begin + free_slots - 1) {
            set_bitmap(pool, i, MSLOT_USEEND);
        } else {
            set_bitmap(pool, i, MSLOT_USING);
        }
    }

    return slot_begin_ptr;
}

void *mpool_realloc(struct mpool *pool, void *ptr, unsigned long new_size)
{
    if (ptr < pool->data || pool->end < ptr) {
        return NULL;
    }

    unsigned long slot = (unsigned long)((uint8_t*)ptr - (uint8_t*)pool->data) / MPOOL_SLOT_SIZE;
    if (!get_bitmap(pool, slot)) {  // realloc-ing unallocated pointer
        return NULL;
    }

    // count allocated slots
    unsigned long current_slot_count = 0;
    for (unsigned long i = slot; i < pool->slots; i++) {
        current_slot_count++;
        if (get_bitmap(pool, i) == MSLOT_USEEND) {
            break;
        }
    }

    unsigned long current_size = current_slot_count * MPOOL_SLOT_SIZE;
    if (new_size <= current_size) {
        // new size is smaller than current size; no shrinking
        return ptr;
    }

    // free current slots temporarily
    mpool_free(pool, ptr);

    // reallocate. this call SHOULD success
    void *new_ptr = mpool_alloc(pool, new_size);

    // copy data if required
    if (new_ptr != ptr) {
        memcpy(new_ptr, ptr, MIN(new_size, current_size));
    }

    return new_ptr;
}

void mpool_free(struct mpool *pool, void *ptr)
{
    if (ptr < pool->data || pool->end < ptr) {
        return;
    }

    unsigned long slot = (unsigned long)((uint8_t*)ptr - (uint8_t*)pool->data) / MPOOL_SLOT_SIZE;
    if (!get_bitmap(pool, slot)) {  // double free
        return;
    }
    for (unsigned long i = slot; i < pool->slots; i++) {
        int slot_status = get_bitmap(pool, i);
        set_bitmap(pool, i, MSLOT_FREE);
        if (slot_status == MSLOT_USEEND) {
            break;
        }
    }
}

unsigned long mpool_getfree(struct mpool *pool)
{
    unsigned long free_slots = 0;
    for (unsigned long i = 0; i < pool->slots; i++) {
        if (get_bitmap(pool, i) == MSLOT_FREE) {
            free_slots++;
        }
    }
    return free_slots * MPOOL_SLOT_SIZE;
}
