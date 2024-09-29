#ifndef PROTO1_MMIO_H__
#define PROTO1_MMIO_H__

#include <stddef.h>
#include <stdint.h>

uint8_t mmio_read_8(uint16_t port);
uint16_t mmio_read_16(uint16_t port);
void mmio_write_8(uint16_t port, uint8_t data);
void mmio_write_16(uint16_t port, uint16_t data);

#endif  // PROTO1_MMIO_H__
