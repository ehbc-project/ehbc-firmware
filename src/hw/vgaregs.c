/*
  This code is based on SeaBIOS: https://github.com/coreboot/seabios
*/

#include "hw/vgaregs.h"

void vga_out_cr(int idx, uint8_t val, int mport)
{
    if (mport) {
        *(hwreg8_t*)VGA_CR_M_ADDR = idx;
        *(hwreg8_t*)VGA_CR_M_DATA = val;
    } else {
        *(hwreg8_t*)VGA_CR_C_ADDR = idx;
        *(hwreg8_t*)VGA_CR_C_DATA = val;
    }
}

uint8_t vga_in_cr(int idx, int mport)
{
    if (mport) {
        *(hwreg8_t*)VGA_CR_M_ADDR = idx;
        return *(hwreg8_t*)VGA_CR_M_DATA;
    } else {
        *(hwreg8_t*)VGA_CR_C_ADDR = idx;
        return *(hwreg8_t*)VGA_CR_C_DATA;
    }
}

void vga_out_gr(int idx, uint8_t val)
{
    *(hwreg8_t*)VGA_GR_ADDR = idx;
    *(hwreg8_t*)VGA_GR_DATA = val;
}

uint8_t vga_in_gr(int idx)
{
    *(hwreg8_t*)VGA_GR_ADDR = idx;
    return *(hwreg8_t*)VGA_GR_DATA;
}

void vga_out_sr(int idx, uint8_t val)
{
    *(hwreg8_t*)VGA_SR_ADDR = idx;
    *(hwreg8_t*)VGA_SR_DATA = val;
}

uint8_t vga_in_sr(int idx)
{
    *(hwreg8_t*)VGA_SR_ADDR = idx;
    return *(hwreg8_t*)VGA_SR_DATA;
}

void vga_out_ar(int idx, uint8_t val)
{
    *(hwreg8_t*)VGA_ISR1;
    uint8_t temp = *(hwreg8_t*)VGA_AR_AD;
    *(hwreg8_t*)VGA_AR_AD = idx;
    *(hwreg8_t*)VGA_AR_AD = val;
    *(hwreg8_t*)VGA_AR_AD = temp;
}

uint8_t vga_in_ar(int idx)
{
    *(hwreg8_t*)VGA_ISR1;
    uint8_t temp = *(hwreg8_t*)VGA_AR_AD;
    *(hwreg8_t*)VGA_AR_AD = idx;
    uint8_t val = *(hwreg8_t*)VGA_AR_DR;
    *(hwreg8_t*)VGA_ISR1;
    *(hwreg8_t*)VGA_AR_AD = temp;
    return *(hwreg8_t*)VGA_AR_DR;
}

void vga_out_misc(uint8_t val)
{
    *(hwreg8_t*)VGA_MISCW = val;
}

uint8_t vga_in_misc()
{
    return *(hwreg8_t*)VGA_MISCR;
}

void vga_dac_read(int startidx, uint8_t* data, int count)
{
    *(hwreg8_t*)VGA_DR_ADDRR = startidx;
    while (count) {
        *data++ = *(hwreg8_t*)VGA_DR_DATA;
        *data++ = *(hwreg8_t*)VGA_DR_DATA;
        *data++ = *(hwreg8_t*)VGA_DR_DATA;
        count--;
    }
}

void vga_dac_write(int startidx, const uint8_t* data, int count)
{
    *(hwreg8_t*)VGA_DR_ADDRW = startidx;
    while (count) {
        *(hwreg8_t*)VGA_DR_DATA = *data++;
        *(hwreg8_t*)VGA_DR_DATA = *data++;
        *(hwreg8_t*)VGA_DR_DATA = *data++;
        count--;
    }
}
