#ifndef __RINGBUF_H__
#define __RINGBUF_H__

#include <stdint.h>

#include "spinlock.h"

struct ringbuf8 {
    unsigned long size;
    unsigned long rptr, wptr;
    uint8_t *buf;
    int external_lock;
    struct spinlock_irq lock;
};

void ringbuf8_init(struct ringbuf8 *rb, unsigned long size, uint8_t *buf);

void ringbuf8_lock(struct ringbuf8 *rb);
void ringbuf8_unlock(struct ringbuf8 *rb);

int ringbuf8_pop(struct ringbuf8 *rb, uint8_t *data);
int ringbuf8_push(struct ringbuf8 *rb, uint8_t data);
void ringbuf8_flush(struct ringbuf8 *rb);
int ringbuf8_unpop(struct ringbuf8 *rb, uint8_t data);

unsigned int ringbuf8_getfree(struct ringbuf8 *rb);

#endif // __RINGBUF_H__
