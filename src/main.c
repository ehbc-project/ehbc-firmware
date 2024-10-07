#include <assert.h>
#include <stdio.h>
#include <macros.h>

#include "device.h"
#include "types.h"
#include "hw/vga.h"
#include "hw/ps2kbms.h"
#include "hw/mc68681.h"
#include "hw/rtc.h"
#include "io.h"

extern struct device *const mc68681_device;

void main(void)
{
    struct ehbcfw_aio_param aio_param = {
        .baud = AIO_BAUD_9600,
        .bpc = 8,
        .parity_mode = AIO_PM_NONE,
        .stop_bits = AIO_SB_1,
    };

    if (!mc68681_cha_set_param(mc68681_device, aio_param)) {
        printf("DUART OK\r\n");
    }

    if (!ps2kbms_init()) {
        printf("PS2KBC OK\r\n");
    }

    struct device *vga = find_device(2);
    const struct video_mode_info *mode = vga_get_mode_info(vga, 0x03);
    if (ehbcfw_video_set_mode(2, 0x03)) {
        printf("Video mode not found\r\n");
        return;
    }
    
    ehbcfw_video_set_cursor_shape(2, 0x0A0F);
    volatile uint8_t* fbuf_base = (uint8_t*)mode->buf_base;
    for (int i = 0; i < mode->width * mode->height; i++) {
        fbuf_base[(i << 1) + 1] = 0x07;
    }

    char str[15];

    volatile void* memtest_ptr = (void*)0xFFFC;
    for (;;) {
        *(volatile uint32_t*)memtest_ptr = (uint32_t)memtest_ptr;

        if (*(volatile uint32_t*)memtest_ptr != (uint32_t)memtest_ptr) {
            break;
        }

        printf("%8lu kB OK\r", ((uint32_t)memtest_ptr + 4) >> 10);

        memtest_ptr = (void*)((uint32_t)memtest_ptr + 0x10000);
    }
    printf("\r\n");

    struct device *dev = get_device_list_head();
    while (dev) {
        if (dev->flags & DF_AVAILABLE) {
            printf("Device #%d: %s %s\r\n", dev->id, dev->get_vendor(dev), dev->get_name(dev));
        }
        dev = dev->next;
    }

    extern void run_monitor(void);
    run_monitor();
}
