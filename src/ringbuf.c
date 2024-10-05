#include "ringbuf.h"

void ringbuf8_init(struct ringbuf8 *rb, unsigned long size, uint8_t *buf)
{
    rb->size = size;
    rb->buf = buf;
    rb->rptr = 0;
    rb->wptr = 0;
}

int ringbuf8_read(struct ringbuf8 *rb, uint8_t *data)
{
    if (rb->rptr == rb->wptr) {
        return 1;
    }

    *data = rb->buf[rb->rptr];

    rb->rptr++;
    if (rb->rptr >= rb->size) {
        rb->rptr = 0;
    }

    return 0;
}

int ringbuf8_write(struct ringbuf8 *rb, uint8_t data)
{
    if (rb->wptr == rb->rptr - 1 || (rb->wptr == rb->size - 1 && rb->rptr == 0)) {
        return 1;
    }

    rb->buf[rb->wptr] = data;

    rb->wptr++;
    if (rb->wptr >= rb->size) {
        rb->wptr = 0;
    }

    return 0;
}
