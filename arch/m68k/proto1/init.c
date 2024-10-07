#include <stdint.h>

#include <macros.h>

#include "device.h"
#include "ringbuf.h"
#include "hw/mc68681.h"
#include "hw/ata.h"
#include "hw/vga.h"
#include "hw/ps2kbms.h"
#include "hw/rtc.h"
#include "proto1/mmio.h"

static struct ringbuf8 mc68681_cha_rxbuf, mc68681_cha_txbuf;
static uint8_t mc68681_cha_rxbuf_data[128], mc68681_cha_txbuf_data[128];

static struct ringbuf8 mc68681_chb_rxbuf, mc68681_chb_txbuf;
static uint8_t mc68681_chb_rxbuf_data[128], mc68681_chb_txbuf_data[128];

static struct device_mc68681 mc68681 = {
    .mmio_base = (void*)0xFF000200,
    .cha_rxbuf = &mc68681_cha_rxbuf,
    .cha_txbuf = &mc68681_cha_txbuf,
    .chb_rxbuf = &mc68681_chb_rxbuf,
    .chb_txbuf = &mc68681_chb_txbuf,
};

static void init_mc68681_buf(void)
{
    ringbuf8_init(&mc68681_cha_rxbuf, sizeof(mc68681_cha_rxbuf_data), mc68681_cha_rxbuf_data);
    ringbuf8_init(&mc68681_cha_txbuf, sizeof(mc68681_cha_txbuf_data), mc68681_cha_txbuf_data);
    ringbuf8_init(&mc68681_chb_rxbuf, sizeof(mc68681_chb_rxbuf_data), mc68681_chb_rxbuf_data);
    ringbuf8_init(&mc68681_chb_txbuf, sizeof(mc68681_chb_txbuf_data), mc68681_chb_txbuf_data);
}

static struct device_vga vga;

struct device builtin_devices[] = {
    {   // mc68681 channel A
        .class = DC_AIO,
        .flags = DF_AVAILABLE,
        .get_name = mc68681_get_name,
        .get_vendor = mc68681_get_vendor,
        .reset = mc68681_cha_reset,
        .aio_ops = {
            .set_param = mc68681_cha_set_param,
            .wait_send = mc68681_cha_write_byte,
            .send = mc68681_cha_write_byte,
            .wait_recv = mc68681_cha_read_byte,
            .recv = mc68681_cha_read_byte,
        },
        .param = &mc68681,
    },
    {   // mc68681 channel B
        .class = DC_AIO,
        .flags = DF_AVAILABLE,
        .get_name = mc68681_get_name,
        .get_vendor = mc68681_get_vendor,
        .reset = mc68681_chb_reset,
        .aio_ops = {
            .set_param = mc68681_chb_set_param,
            .wait_send = mc68681_chb_write_byte,
            .send = mc68681_chb_write_byte,
            .wait_recv = mc68681_chb_read_byte,
            .recv = mc68681_chb_read_byte,
        },
        .param = &mc68681,
    },
    {
        .class = DC_VIDEO,
        .get_name = vga_get_name,
        .get_vendor = vga_get_vendor,
        .probe = vga_probe,
        .reset = vga_reset,
        .video_ops = {
            .set_mode = vga_set_mode,
            .get_mode = vga_get_mode,
            .set_cursor_shape = vga_set_cursor_shape,
            .get_cursor_shape = vga_get_cursor_shape,
            .set_cursor_pos = vga_set_cursor_pos,
            .get_cursor_pos = vga_get_cursor_pos,
            .scroll_area = vga_scroll_area,
            .read_char_attr = vga_read_char_attr,
            .write_char_attr = vga_write_char_attr,
            .write_string = vga_write_string,
            .write_string_with_attr = vga_write_string_with_attr,
            .write_ansi_tty = vga_write_ansi_tty,
        },
        .param = &vga
    },
    {
        .class = DC_RTC,
        .flags = DF_AVAILABLE,
        .get_name = rtc_get_name,
        .get_vendor = rtc_get_vendor,
        .reset = rtc_reset,
        .rtc_ops = {
            .get_time = rtc_get_time,
            .set_time = rtc_set_time,
            .get_alarm = rtc_get_alarm,
            .set_alarm = rtc_set_alarm,
            .read_nvsram = rtc_read_nvsram,
            .write_nvsram = rtc_write_nvsram,
        },
    }
    /*
    {
        .class = DC_STORAGE,
        .storage_ops = {
            .get_status = ata_get_status,
            .read_sectors_chs = ata_read_sectors_chs,
            .write_sectors_chs = ata_write_sectors_chs,
        },
    },
    {
        .class = DC_STORAGE,
        .storage_ops = {
            .get_status = ata_get_status,
            .read_sectors_chs = ata_read_sectors_chs,
            .write_sectors_chs = ata_write_sectors_chs,
        },
    },
    {
        .class = DC_KEYBOARD,
        .keyboard_ops = {
            .wait_get_stroke = ps2_keyboard_wait_get_stroke,
            .get_stroke = ps2_keyboard_get_stroke,
            .get_flags = ps2_keyboard_get_flags,
            .flush_buffer = ps2_keyboard_flush_buffer,
        },
    },
    {
        .class = DC_MOUSE,
        .mouse_ops = {
            .set_param = ps2_mouse_set_param,
            .get_status = ps2_mouse_get_status,
            .get_movement = ps2_mouse_get_movement,
        },
    },
    */
};

struct device *const mc68681_device = &builtin_devices[0];


static void init_devices(void)
{
    init_mc68681_buf();

    for (int i = 0; i < ARRAY_SIZE(builtin_devices); i++) {
        add_device(&builtin_devices[i]);
    }
}  

__attribute__((constructor))
static void init_system(void)
{
    mmio_write_8(0x00, 0x07);  // set clock to 33 MHz
    mmio_write_8(0x10, 0x00);  // irq0= disabled    irq1= disabled
    mmio_write_8(0x11, 0x00);  // irq2= disabled    irq3= disabled
    mmio_write_8(0x12, 0x00);  // irq4= disabled    irq5= disabled
    mmio_write_8(0x13, 0x00);  // irq6= disabled    irq7= disabled
    mmio_write_8(0x14, 0x00);  // irq8= disabled    irq9= disabled
    mmio_write_8(0x15, 0x00);  // irq10=disabled    irq11=disabled
    mmio_write_8(0x16, 0x00);  // irq12=disabled    irq13=disabled
    mmio_write_8(0x17, 0x00);  // irq14=disabled    irq15=disabled
    mmio_write_8(0x18, 0xA0);  // irq16=level2      irq17=disabled
    mmio_write_8(0x19, 0x00);  // irq18=disabled    irq19=disabled
    mmio_write_8(0x1A, 0x00);  // irq20=disabled    irq21=disabled
    mmio_write_8(0x1B, 0x00);  // irq22=disabled    irq23=disabled

    init_devices();
}
