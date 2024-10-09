#include "mpool.h"

#include <string.h>
#include <stdint.h>
#include <macros.h>

// bitmap memory pool

#define MSLOT_FREE      0
#define MSLOT_USING     1
#define MSLOT_USEEND    2

static void *mpool_bitmap_base;
static unsigned long mpool_bitmap_size;
static void *mpool_base;
static void *mpool_end;
static unsigned long mpool_size;
static unsigned long mpool_slots;

static int set_bitmap(unsigned long slot, int value)
{
    if (slot >= mpool_slots) {
        return -1;
    }

    uint8_t *bitmap = (uint8_t*)mpool_bitmap_base + (slot >> 2);
    *bitmap &= ~(3 << ((slot & 3) << 1));
    *bitmap |= (value & 3) << ((slot & 3) << 1);
    return 0;
}

static int get_bitmap(unsigned long slot)
{
    if (slot >= mpool_slots) {
        return -1;
    }

    uint8_t *bitmap = (uint8_t*)mpool_bitmap_base + (slot >> 2);
    return (*bitmap >> ((slot & 3) << 1)) & 3;
}

unsigned long mpool_init(void *base, unsigned long size)
{
    mpool_bitmap_base = base;
    mpool_slots = size / MSLOT_SIZE;
    mpool_bitmap_size = ((mpool_slots >> 2) + 15) & ~15;  // align to 16 bytes
    mpool_slots -= (mpool_bitmap_size + MSLOT_SIZE - 1) / MSLOT_SIZE;
    mpool_base = (uint8_t*)base + mpool_bitmap_size;
    mpool_size = mpool_slots * MSLOT_SIZE;
    mpool_end = (uint8_t*)mpool_base + mpool_size;
    memset(base, 0, size);
    return mpool_size;
}

void *mpool_alloc(unsigned long size)
{
    if (!size) return NULL;

    unsigned long required_slots = (size + (MSLOT_SIZE - 1)) / MSLOT_SIZE;
    void *slot_begin_ptr = NULL;
    unsigned long slot_begin;
    unsigned long free_slots = 0;
    
    // find contiguous free slots
    for (unsigned long i = 0; i < mpool_slots; i++) {
        if (get_bitmap(i) == MSLOT_FREE) {
            if (!free_slots) {
                slot_begin_ptr = (uint8_t*)mpool_base + i * MSLOT_SIZE;
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
            set_bitmap(i, MSLOT_USEEND);
        } else {
            set_bitmap(i, MSLOT_USING);
        }
    }

    return slot_begin_ptr;
}

void *mpool_realloc(void *ptr, unsigned long new_size)
{
    if (ptr < mpool_base || mpool_end < ptr) {
        return NULL;
    }

    unsigned long slot = (unsigned long)((uint8_t*)ptr - (uint8_t*)mpool_base) / MSLOT_SIZE;
    if (!get_bitmap(slot)) {  // realloc-ing unallocated pointer
        return NULL;
    }

    // count allocated slots
    unsigned long current_slot_count = 0;
    for (unsigned long i = slot; i < mpool_slots; i++) {
        current_slot_count++;
        if (get_bitmap(i) == MSLOT_USEEND) {
            break;
        }
    }

    unsigned long current_size = current_slot_count * MSLOT_SIZE;
    if (new_size <= current_size) {
        // new size is smaller than current size; no shrinking
        return ptr;
    }

    // free current slots temporarily
    mpool_free(ptr);

    // reallocate. this call SHOULD success
    void *new_ptr = mpool_alloc(new_size);

    // copy data if required
    if (new_ptr != ptr) {
        memcpy(new_ptr, ptr, MIN(new_size, current_size));
    }

    return new_ptr;
}

void mpool_free(void *ptr)
{
    if (ptr < mpool_base || mpool_end < ptr) {
        return;
    }

    unsigned long slot = (unsigned long)((uint8_t*)ptr - (uint8_t*)mpool_base) / MSLOT_SIZE;
    if (!get_bitmap(slot)) {  // double free
        return;
    }
    for (unsigned long i = slot; i < mpool_slots; i++) {
        int slot_status = get_bitmap(i);
        set_bitmap(i, MSLOT_FREE);
        if (slot_status == MSLOT_USEEND) {
            break;
        }
    }
}

unsigned long mpool_getfree(void)
{
    unsigned long free_slots = 0;
    for (unsigned long i = 0; i < mpool_slots; i++) {
        if (get_bitmap(i) == MSLOT_FREE) {
            free_slots++;
        }
    }
    return free_slots * MSLOT_SIZE;
}
