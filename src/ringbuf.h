#ifndef __RINGBUF_H__
#define __RINGBUF_H__

#include <stdint.h>

struct ringbuf8 {
    unsigned long size;
    unsigned long rptr, wptr;
    uint8_t *buf;
};

void ringbuf8_init(struct ringbuf8 *rb, unsigned long size, uint8_t *buf);

int ringbuf8_read(struct ringbuf8 *rb, uint8_t *data);
int ringbuf8_write(struct ringbuf8 *rb, uint8_t data);

#endif // __RINGBUF_H__
