#ifndef __ASM_GENERIC_IO_H__
#define __ASM_GENERIC_IO_H__

#include <stddef.h>
#include <stdint.h>

uint8_t io_read_8(uint16_t port);
uint16_t io_read_16(uint16_t port);
void io_write_8(uint16_t port, uint8_t data);
void io_write_16(uint16_t port, uint16_t data);

#endif  // __ASM_GENERIC_IO_H__
