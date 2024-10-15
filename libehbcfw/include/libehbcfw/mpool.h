#ifndef __LIBEHBCFW_MPOOL_H__
#define __LIBEHBCFW_MPOOL_H__

#define MPOOL_SLOT_SIZE     0x100  // 256 bytes per slot

struct mpool {
    void *bitmap, *data, *end;
    unsigned long bitmap_size, data_size, slots;
};

unsigned long mpool_init(struct mpool *pool, void *base, unsigned long size);
void *mpool_alloc(struct mpool *pool, unsigned long size);
void *mpool_realloc(struct mpool *pool, void *ptr, unsigned long new_size);
void mpool_free(struct mpool *pool, void *ptr);
unsigned long mpool_getfree(struct mpool *pool);

#endif // __LIBEHBCFW_MPOOL_H__
