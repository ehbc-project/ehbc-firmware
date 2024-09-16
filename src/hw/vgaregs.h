#ifndef HW_VGAREGS_H__
#define HW_VGAREGS_H__

#include "types.h"

#define VGA_CR_M_ADDR       0xFE0003B4
#define VGA_CR_M_DATA       0xFE0003B5
#define VGA_FCR             0xFE0003BA
#define VGA_AR_AD           0xFE0003C0
#define VGA_AR_DR           0xFE0003C1
#define VGA_ISR0            0xFE0003C2
#define VGA_MISCW           0xFE0003C2
#define VGA_SR_ADDR         0xFE0003C4
#define VGA_SR_DATA         0xFE0003C5
#define VGA_PALMASK         0xFE0003C6
#define VGA_DR_STATE        0xFE0003C7
#define VGA_DR_ADDRR        0xFE0003C7
#define VGA_DR_ADDRW        0xFE0003C8
#define VGA_DR_DATA         0xFE0003C9
#define VGA_MISCR           0xFE0003CC
#define VGA_GR_ADDR         0xFE0003CE
#define VGA_GR_DATA         0xFE0003CF
#define VGA_CR_C_ADDR       0xFE0003D4
#define VGA_CR_C_DATA       0xFE0003D5
#define VGA_ISR1            0xFE0003DA

void vga_out_cr(int idx, uint8_t val, int mport);
uint8_t vga_in_cr(int idx, int mport);
void vga_out_gr(int idx, uint8_t val);
uint8_t vga_in_gr(int idx);
void vga_out_sr(int idx, uint8_t val);
uint8_t vga_in_sr(int idx);
void vga_out_ar(int idx, uint8_t val);
uint8_t vga_in_ar(int idx);
void vga_out_misc(uint8_t val);
uint8_t vga_in_misc();
void vga_dac_read(int startidx, uint8_t* data, int count);
void vga_dac_write(int startidx, const uint8_t* data, int count);

#endif  // HW_VGAREGS_H__
