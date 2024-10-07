#ifndef __RINGBUF_H__
#define __RINGBUF_H__

#include <stdint.h>
#include <stdatomic.h>

// parrtially atomic ring buffer
struct ringbuf8 {
    unsigned long size;
    atomic_uint rptr, wptr;
    atomic_flag mutex;
    uint8_t *buf;
};

void ringbuf8_init(struct ringbuf8 *rb, unsigned long size, uint8_t *buf);

int ringbuf8_read(struct ringbuf8 *rb, uint8_t *data);
int ringbuf8_write(struct ringbuf8 *rb, uint8_t data);

unsigned int ringbuf8_getfree(struct ringbuf8 *rb);

#endif // __RINGBUF_H__
