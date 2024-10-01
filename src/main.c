#include <assert.h>
#include <stdio.h>

#include "types.h"
#include "macros.h"
#include "hw/vga.h"
#include "hw/ps2kbms.h"
#include "hw/mc68681.h"
#include "hw/rtc.h"

void main(void)
{
    if (!mc68681_init() && !mc68681_set_param(0, 9600, 9600, 8, PM_NONE, SB_1)) {
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

    for (int i = 0; i < 1048576; i++) {}

    // start video mode tests
    const int video_modes[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x6A
    };
    for (int i = 15; i < ARRAY_SIZE(video_modes); i++) {
        mode = vga_get_mode_info(video_modes[i]);
        if (vga_set_mode(video_modes[i])) {
            mc68681_tx(0, "Video mode not found\r\n");
            return;
        }

        char buf[16];
        snprintf(buf, sizeof(buf), "Video mode %02Xh\r\n", video_modes[i]);
        mc68681_tx(0, buf);

        volatile uint8_t* fbuf_baseb = (uint8_t*)mode->buf_base;
        volatile uint16_t* fbuf_basew = (uint16_t*)mode->buf_base;

        const int pixels = mode->width * mode->height;
        switch (mode->mmodel) {
            case MM_TEXT:
                vga_set_cursor_shape(0x0A0F);
                for (int i = 0; i < pixels; i++) {
                    fbuf_base[i << 1] = i;
                    fbuf_base[(i << 1) + 1] = i;
                }
                break;
            case MM_CGA:
                if (mode->bpp == 1) {
                    vga_select_plane(0);
                    for (int i = 0; i < pixels >> 4; i++) {
                        int temp = (i / (pixels >> 5)) ? 0xFFFF : 0x0000;
                        fbuf_basew[i] = temp;
                        fbuf_basew[(pixels >> 4) + i] = temp;
                    }
                } else {
                    for (int i = 0; i < pixels >> 3; i++) {
                        uint16_t pix = (i / (pixels >> 5)) & 3;
                        pix |= pix << 2;
                        pix |= pix << 4;
                        pix |= pix << 8;
                        vga_select_plane(0);
                        fbuf_baseb[i] = pix;
                        fbuf_baseb[0x2000 + i] = pix;
                        vga_select_plane(1);
                        fbuf_baseb[i] = pix;
                        fbuf_baseb[0x2000 + i] = pix;
                    }
                }
                break;
            case MM_PLANAR: {
                for (int j = 0; j < mode->bpp; j++) {
                    vga_select_plane(j);
                    const int divisor = pixels >> (5 + j);
                    for (int i = 0; i < pixels >> 4; i++) {
                        int temp = i / divisor;
                        fbuf_basew[i] = (temp & 1) ? 0xFFFF : 0x0000;
                    }
                }
                break;
            }
            case MM_PACKED: {
                vga_select_plane(0);
                for (int i = 0; i < pixels; i++) {
                    int temp = i / (pixels >> 8);
                    fbuf_baseb[i] = temp & 0xFF;
                }
                break;
            }
            default:
                break;
        }
        
        for (volatile int i = 0; i < 1048576; i++) {}
    }

    extern void run_monitor(void);
    run_monitor();
}
