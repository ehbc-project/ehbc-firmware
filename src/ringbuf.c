#include "ringbuf.h"

void ringbuf8_init(struct ringbuf8 *rb, unsigned long size, uint8_t *buf)
{
    rb->size = size;
    rb->buf = buf;
    rb->external_lock = 0;
    spinlock_irq_init(&rb->lock);
}

void ringbuf8_lock(struct ringbuf8 *rb)
{
    spinlock_irq_lock(&rb->lock);
    rb->external_lock = 1;
}

void ringbuf8_unlock(struct ringbuf8 *rb)
{
    rb->external_lock = 0;
    spinlock_irq_lock(&rb->lock);
}

int ringbuf8_pop(struct ringbuf8 *rb, uint8_t *data)
{
    if (!rb->external_lock)
        spinlock_irq_lock(&rb->lock);
    if (rb->rptr == rb->wptr) {
        if (!rb->external_lock)
            spinlock_irq_unlock(&rb->lock);
        return 1;
    }

    *data = rb->buf[rb->rptr];

    if (rb->rptr < rb->size - 1) {
        rb->rptr++;
    } else {
        rb->rptr = 0;
    }

    if (!rb->external_lock)
        spinlock_irq_unlock(&rb->lock);
    return 0;
}

int ringbuf8_push(struct ringbuf8 *rb, uint8_t data)
{
    if (!rb->external_lock)
        spinlock_irq_lock(&rb->lock);
    if (rb->wptr == rb->rptr - 1 || (rb->wptr == rb->size - 1 && rb->rptr == 0)) {
        if (!rb->external_lock)
            spinlock_irq_unlock(&rb->lock);
        return 1;
    }

    rb->buf[rb->wptr] = data;

    if (rb->wptr < rb->size - 1) {
        rb->wptr++;
    } else {
        rb->wptr = 0;
    }

    if (!rb->external_lock)
        spinlock_irq_unlock(&rb->lock);
    return 0;
}

void ringbuf8_flush(struct ringbuf8 *rb)
{
    if (!rb->external_lock)
        spinlock_irq_lock(&rb->lock);

    rb->rptr = rb->wptr;

    if (!rb->external_lock)
        spinlock_irq_unlock(&rb->lock);
}

int ringbuf8_unpop(struct ringbuf8 *rb, uint8_t data)
{
    if (!rb->external_lock)
        spinlock_irq_lock(&rb->lock);
    if (rb->rptr == rb->wptr) {
        if (!rb->external_lock)
            spinlock_irq_unlock(&rb->lock);
        return 1;
    }

    if (rb->rptr > 0) {
        rb->rptr--;
    } else {
        rb->rptr = rb->size - 1;
    }

    rb->buf[rb->rptr] = data;

    if (!rb->external_lock)
        spinlock_irq_unlock(&rb->lock);
    return 0;
}

unsigned int ringbuf8_getfree(struct ringbuf8 *rb)
{
    unsigned int size;
    if (!rb->external_lock)
        spinlock_irq_lock(&rb->lock);

    if (rb->rptr <= rb->wptr) {
        size = rb->wptr - rb->rptr;
    } else {
        size = rb->size - rb->rptr + rb->wptr;
    }

    if (!rb->external_lock)
        spinlock_irq_unlock(&rb->lock);
    return size;
}
