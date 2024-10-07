#include "ringbuf.h"

void ringbuf8_init(struct ringbuf8 *rb, unsigned long size, uint8_t *buf)
{
    rb->size = size;
    rb->buf = buf;
    atomic_init(&rb->rptr, 0);
    atomic_init(&rb->wptr, 0);
}

int ringbuf8_read(struct ringbuf8 *rb, uint8_t *data)
{
    if (rb->rptr == rb->wptr) {
        return 1;
    }

    *data = rb->buf[rb->rptr];

    if (rb->rptr < rb->size - 1) {
        atomic_fetch_add(&rb->rptr, 1);
    } else {
        atomic_store(&rb->rptr, 0);
    }

    return 0;
}

int ringbuf8_write(struct ringbuf8 *rb, uint8_t data)
{
    if (rb->wptr == rb->rptr - 1 || (rb->wptr == rb->size - 1 && rb->rptr == 0)) {
        return 1;
    }

    rb->buf[rb->wptr] = data;

    if (rb->wptr < rb->size - 1) {
        atomic_fetch_add(&rb->wptr, 1);
    } else {
        atomic_store(&rb->wptr, 0);
    }

    return 0;
}

unsigned int ringbuf8_getfree(struct ringbuf8 *rb)
{
    if (rb->rptr <= rb->wptr) {
        return rb->wptr - rb->rptr;
    } else {
        return rb->size - rb->rptr + rb->wptr;
    }
}
