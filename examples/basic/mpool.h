#ifndef __MPOOL_H__
#define __MPOOL_H__

#define MSLOT_SIZE      0x100  // 256 bytes per slot

unsigned long mpool_init(void *base, unsigned long size);
void *mpool_alloc(unsigned long size);
void *mpool_realloc(void *ptr, unsigned long new_size);
void mpool_free(void *ptr);
unsigned long mpool_getfree(void);

#endif // __MPOOL_H__
