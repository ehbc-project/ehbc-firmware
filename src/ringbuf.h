#ifndef __RINGBUF_H__
#define __RINGBUF_H__

#include <stdint.h>

#include "spinlock.h"

// parrtially atomic ring buffer
struct ringbuf8 {
    unsigned long size;
    unsigned long rptr, wptr;
    uint8_t *buf;
    struct spinlock_irq lock;
};

void ringbuf8_init(struct ringbuf8 *rb, unsigned long size, uint8_t *buf);

int ringbuf8_read(struct ringbuf8 *rb, uint8_t *data);
int ringbuf8_write(struct ringbuf8 *rb, uint8_t data);
void ringbuf8_flush(struct ringbuf8 *rb);

unsigned int ringbuf8_getfree(struct ringbuf8 *rb);

#endif // __RINGBUF_H__
