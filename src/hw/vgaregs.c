/*
  This code is based on SeaBIOS: https://github.com/coreboot/seabios
*/

#include "hw/vgaregs.h"

#include <asm/io.h>

void vga_out_cr(int idx, uint8_t val, int mport)
{
    if (mport) {
        io_write_8(VGA_CR_M_ADDR, idx);
        io_write_8(VGA_CR_M_DATA, val);
    } else {
        io_write_8(VGA_CR_C_ADDR, idx);
        io_write_8(VGA_CR_C_DATA, val);
    }
}

uint8_t vga_in_cr(int idx, int mport)
{
    if (mport) {
        io_write_8(VGA_CR_M_ADDR, idx);
        return io_read_8(VGA_CR_M_DATA);
    } else {
        io_write_8(VGA_CR_C_ADDR, idx);
        return io_read_8(VGA_CR_C_DATA);
    }
}

void vga_out_gr(int idx, uint8_t val)
{
    io_write_8(VGA_GR_ADDR, idx);
    io_write_8(VGA_GR_DATA, val);
}

uint8_t vga_in_gr(int idx)
{
    io_write_8(VGA_GR_ADDR, idx);
    return io_read_8(VGA_GR_DATA);
}

void vga_out_sr(int idx, uint8_t val)
{
    io_write_8(VGA_SR_ADDR, idx);
    io_write_8(VGA_SR_DATA, val);
}

uint8_t vga_in_sr(int idx)
{
    io_write_8(VGA_SR_ADDR, idx);
    return io_read_8(VGA_SR_DATA);
}

void vga_out_ar(int idx, uint8_t val)
{
    io_read_8(VGA_ISR1);
    uint8_t temp = io_read_8(VGA_AR_AD);
    io_write_8(VGA_AR_AD, idx);
    io_write_8(VGA_AR_AD, val);
    io_write_8(VGA_AR_AD, temp);
}

uint8_t vga_in_ar(int idx)
{
    io_read_8(VGA_ISR1);
    uint8_t temp = io_read_8(VGA_AR_AD);
    io_write_8(VGA_AR_AD, idx);
    uint8_t val = io_read_8(VGA_AR_DR);
    io_read_8(VGA_ISR1);
    io_write_8(VGA_AR_AD, temp);
    return val;
}

void vga_out_misc(uint8_t val)
{
    io_write_8(VGA_MISCW, val);
}

uint8_t vga_in_misc()
{
    return io_read_8(VGA_MISCR);
}

void vga_dac_read(int startidx, uint8_t* data, int count)
{
    io_write_8(VGA_DR_ADDRR, startidx);
    while (count) {
        *data++ = io_read_8(VGA_DR_DATA);
        *data++ = io_read_8(VGA_DR_DATA);
        *data++ = io_read_8(VGA_DR_DATA);
        count--;
    }
}

void vga_dac_write(int startidx, const uint8_t* data, int count)
{
    io_write_8(VGA_DR_ADDRW, startidx);
    while (count) {
        io_write_8(VGA_DR_DATA, *data++);
        io_write_8(VGA_DR_DATA, *data++);
        io_write_8(VGA_DR_DATA, *data++);
        count--;
    }
}
