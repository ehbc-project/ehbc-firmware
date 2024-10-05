#include <stdint.h>

#include "proto1/mmio.h"

__attribute__((constructor))
static void init_system(void)
{
    mmio_write_8(0x00, 0x07);  // set clock to 33 MHz
    mmio_write_8(0x10, 0x09);  // irq0= disabled    irq1= disabled
    mmio_write_8(0x11, 0x09);  // irq2= disabled    irq3= disabled
    mmio_write_8(0x12, 0x99);  // irq4= disabled    irq5= disabled
    mmio_write_8(0x13, 0x99);  // irq6= disabled    irq7= disabled
    mmio_write_8(0x14, 0x99);  // irq8= disabled    irq9= disabled
    mmio_write_8(0x15, 0x99);  // irq10=disabled    irq11=disabled
    mmio_write_8(0x16, 0x90);  // irq12=disabled    irq13=disabled
    mmio_write_8(0x17, 0x99);  // irq14=disabled    irq15=disabled
    mmio_write_8(0x18, 0xA9);  // irq16=level2      irq17=disabled
    mmio_write_8(0x19, 0x99);  // irq18=disabled    irq19=disabled
    mmio_write_8(0x1A, 0x00);  // irq20=disabled    irq21=disabled
    mmio_write_8(0x1B, 0x00);  // irq22=disabled    irq23=disabled
}
