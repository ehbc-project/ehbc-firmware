#include <assert.h>
#include <stdio.h>
#include <macros.h>

#include "types.h"
#include "hw/vga.h"
#include "hw/ps2kbms.h"
#include "hw/mc68681.h"
#include "hw/rtc.h"
#include "io.h"

#define FDC_SRA     0x3F0
#define FDC_SRB     0x3F1
#define FDC_DOR     0x3F2
#define FDC_TDR     0x3F3
#define FDC_MSR     0x3F4
#define FDC_DRSR    0x3F4
#define FDC_FIFO    0x3F5
#define FDC_DIR     0x3F7
#define FDC_CCR     0x3F7

#define CMD_SPECIFY 0x03
#define CMD_WRITE   0x05
#define CMD_READ    0x06
#define CMD_RECALIB 0x07
#define CMD_SENSE   0x08
#define CMD_SEEK    0x09



void main(void)
{
    if (!mc68681_init() &&
        !mc68681_set_param(0,
            AIO_BAUD_9600, AIO_BAUD_9600, 8, AIO_PM_NONE, AIO_SB_1)) {
        mc68681_enable(0, 1, 1);
        mc68681_tx(0, "DUART OK\r\n");
    }

    if (!vga_init()) {
        mc68681_tx(0, "VGA OK\r\n");
    }

    if (!ps2kbms_init()) {
        mc68681_tx(0, "PS2KBD OK\r\n");
    }

    if (!rtc_init()) {
        mc68681_tx(0, "RTC OK\r\n");
    }

    const struct video_mode_info *mode = vga_get_mode_info(0x03);
    if (vga_set_mode(0x03)) {
        mc68681_tx(0, "Video mode not found\r\n");
        return;
    }
    
    vga_set_cursor_shape(0x0A0F);
    volatile uint8_t* fbuf_base = (uint8_t*)mode->buf_base;
    for (int i = 0; i < mode->width * mode->height; i++) {
        fbuf_base[(i << 1) + 1] = 0x07;
        vga_set_cursor_pos(i + 1);
    }

    const char str[] = "KB OK";

    for (int i = 11; i < 11 + sizeof(str); i++) {
        fbuf_base[i << 1] = str[i - 11];
    }

    volatile void* memtest_ptr = (void*)0xFFFC;
    for (;;) {
        *(volatile uint32_t*)memtest_ptr = (uint32_t)memtest_ptr;

        if (*(volatile uint32_t*)memtest_ptr != (uint32_t)memtest_ptr) {
            break;
        }

        uint32_t val = ((uint32_t)memtest_ptr + 4) >> 10;
        for (int i = 9; val > 0; i--) {
            fbuf_base[i << 1] = val % 10 + '0';
            val /= 10;
        }

        memtest_ptr = (void*)((uint32_t)memtest_ptr + 0x10000);
    }

    extern void run_monitor(void);
    run_monitor();
}
