#include <stdint.h>
#include <macros.h>

#include "device.h"
#include "memmap.h"
#include "ringbuf.h"
#include "hw/mc68681.h"
#include "hw/ata.h"
#include "hw/vga.h"
#include "hw/floppy.h"
#include "hw/ps2kbms.h"
#include "hw/rtc.h"
#include "proto1/mc68440.h"
#include "proto1/scu.h"

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

static struct ringbuf8 ps2kbbuf, ps2msbuf;
static uint8_t ps2kbbuf_data[128], ps2msbuf_data[128];

static struct device_ps2kbms ps2kbms = {
    .kbbuf = &ps2kbbuf,
    .msbuf = &ps2msbuf,
};

static void init_ps2kbms_buf(void)
{
    ringbuf8_init(&ps2kbbuf, sizeof(ps2kbbuf_data), ps2kbbuf_data);
    ringbuf8_init(&ps2msbuf, sizeof(ps2msbuf_data), ps2msbuf_data);
}

static struct device_vga vga;

static struct device_ata ata = {
    .port_io = 0x01F0,
    .port_ctrl = 0x03F4,
};

static struct device_fdc fdc = {
    .port = 0x03F0,
};

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
            .flush_tx = mc68681_cha_flushtx,
            .flush_rx = mc68681_cha_flushrx,
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
            .flush_tx = mc68681_chb_flushtx,
            .flush_rx = mc68681_chb_flushrx,
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
        .class = DC_KEYBOARD,
        .get_name = ps2kb_get_name,
        .get_vendor = ps2kb_get_vendor,
        .probe = ps2kbms_probe,
        .reset = ps2kbms_reset,
        .keyboard_ops = {
            .get_char = ps2kb_get_key_char,
            .get_stroke = ps2kb_get_keystroke,
        },
        .param = &ps2kbms
    },
    {
        .class = DC_MOUSE,
        .get_name = ps2ms_get_name,
        .get_vendor = ps2ms_get_vendor,
        .probe = ps2kbms_probe,
        .reset = ps2kbms_reset,
        .mouse_ops = { NULL },
        .param = &ps2kbms
    },
    {
        .class = DC_STORAGE,
        .get_name = ata_get_name,
        .get_vendor = ata_get_vendor,
        .probe = ata_master_probe,
        .reset = ata_master_reset,
        .storage_ops = { NULL, },
        .param = &ata,
    },
    {
        .class = DC_STORAGE,
        .get_name = ata_get_name,
        .get_vendor = ata_get_vendor,
        .probe = ata_slave_probe,
        .reset = ata_slave_reset,
        .storage_ops = { NULL, },
        .param = &ata,
    },
    {
        .class = DC_STORAGE,
        .get_name = floppy_get_name,
        .get_vendor = floppy_get_vendor,
        .probe = floppy_drive0_probe,
        .reset = floppy_drive0_reset,
        .storage_ops = {
            .read_sectors_chs = floppy_drive0_read_sectors_chs,
            .read_sectors_lba = floppy_drive0_read_sectors_lba,
        },
        .param = &fdc,
    },
    {
        .class = DC_STORAGE,
        .get_name = floppy_get_name,
        .get_vendor = floppy_get_vendor,
        .probe = floppy_drive1_probe,
        .reset = floppy_drive1_reset,
        .storage_ops = { NULL, },
        .param = &fdc,
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
    },
};

struct device *const mc68681_device = &builtin_devices[0];
struct device *const floppy_device = &builtin_devices[7];
struct device *const ps2kbms_device = &builtin_devices[3];

static void init_devices(void)
{
    init_mc68681_buf();
    init_ps2kbms_buf();

    for (int i = 0; i < ARRAY_SIZE(builtin_devices); i++) {
        add_device(&builtin_devices[i]);
    }
}  

struct memmap memmap[] = {
    {
        .start = (void*)0x00000000,
        .end = (void*)0x0000FFFF,
        .flags = MEM_OCCUPIED,
    },
    {
        .start = (void*)0x00010000,
        .end = (void*)0x000FFFFF,
        .flags = 0,
    },
    {
        .start = (void*)0x00100000,
        .end = (void*)0xFCFFFFFF,
        .flags = MEM_UNMAPPED,
    },
    {
        .start = (void*)0xFD000000,
        .end = (void*)0xFDFFFFFF,
        .flags = MEM_OCCUPIED | MEM_READONLY,
    },
    {
        .start = (void*)0xFE000000,
        .end = (void*)0xFEFFFFFF,
        .flags = MEM_MMIO,
    },
    {
        .start = (void*)0xFF000000,
        .end = (void*)0xFFFFFFFF,
        .flags = MEM_MMIO,
    },
};

static void init_memmap(void)
{
    for (int i = 0; i < ARRAY_SIZE(memmap) - 1; i++) {
        memmap[i].next = &memmap[i + 1];
    }
    set_memory_map_head(memmap);
}

int dmac_setup_floppy_write(const void *buf, int count)
{
    struct mc68440_regs *dmac0 = (void*)0xFF000300;
    dmac0->channel[0].mar = (uint32_t)buf;
    dmac0->channel[0].bar = (uint32_t)buf;
    dmac0->channel[0].ocr &= ~0x80;
    dmac0->channel[0].mtcr = count;
    dmac0->channel[0].btcr = count;
    dmac0->channel[0].ccr |= 0x80;
    return 0;
}

int dmac_setup_floppy_read(void *buf, int count)
{
    struct mc68440_regs *dmac0 = (void*)0xFF000300;
    dmac0->channel[0].mar = (uint32_t)buf;
    dmac0->channel[0].bar = (uint32_t)buf;
    dmac0->channel[0].ocr |= 0x80;
    dmac0->channel[0].mtcr = count;
    dmac0->channel[0].btcr = count;
    dmac0->channel[0].ccr = 0x80;
    return 0;
}

int dmac_wait_floppy_transfer(void)
{
    struct mc68440_regs *dmac0 = (void*)0xFF000300;
    while (!(dmac0->channel[0].csr & 0x80)) {}
    dmac0->channel[0].ccr = 0x30;

    return 0;
}

static void init_scu(void)
{
    struct scu_regs *scu = (void*)0xFF000000;
    scu->ccr = 0x07;  // set clock to 33 MHz
    scu->icr[0] = 0x0B;  // irq0= disabled    irq1= level3
    scu->icr[1] = 0x00;  // irq2= disabled    irq3= disabled
    scu->icr[2] = 0x00;  // irq4= disabled    irq5= disabled
    scu->icr[3] = 0xA0;  // irq6= level2      irq7= disabled
    scu->icr[4] = 0xD0;  // irq8= level5      irq9= disabled
    scu->icr[5] = 0x00;  // irq10=disabled    irq11=disabled
    scu->icr[6] = 0xB0;  // irq12=level3      irq13=disabled
    scu->icr[7] = 0x90;  // irq14=level1      irq15=disabled
    scu->icr[8] = 0xA0;  // irq16=level2      irq17=disabled
    scu->icr[9] = 0x00;  // irq18=disabled    irq19=disabled
    scu->icr[10] = 0x00;  // irq20=disabled   irq21=disabled
    scu->icr[11] = 0x00;  // irq22=disabled   irq23=disabled
}

static void init_dmac(void)
{
    struct mc68440_regs *dmac0 = (void*)0xFF000300;
    // dmac0ch0: floppy dma
    dmac0->channel[0].dcr = 0x00;       // burst, explicit m68k, 8-bit, pcl status
    dmac0->channel[0].ocr = 0x82;       // d2m, byte, no chain, external
    dmac0->channel[0].scr = 0x04;       // memory count up, device no count
    dmac0->channel[0].cpr = 0x00;       // channel priority 0
    dmac0->channel[0].mtcr = 0x01FF;    // 512 B memory transfer
    dmac0->channel[0].dfcr = 0x05;      // device fc = 5
    dmac0->channel[0].dar = 0xFE0003F4; // device address = 0xFE0003F4
    dmac0->channel[0].mfcr = 0x05;      // memory fc = 5
    dmac0->channel[0].mar = 0x00010000; // memory address = 0x00010000
    dmac0->channel[0].bfcr = 0x05;      // base fc = 5
    dmac0->channel[0].bar = 0x00010000; // base address = memory address

    // dmac0ch1: ide dma
    dmac0->channel[1].dcr = 0x04;       // burst, explicit m68k, 16-bit, pcl status
    dmac0->channel[1].ocr = 0x92;       // d2m, word, no chain, external
    dmac0->channel[1].scr = 0x04;       // memory count up, device no count
    dmac0->channel[1].cpr = 0x00;       // channel priority 0
    dmac0->channel[1].mtcr = 0x01FF;    // 512 B memory transfer
    dmac0->channel[1].dfcr = 0x05;      // device fc = 5
    dmac0->channel[1].dar = 0xFE0001F0; // device address = 0xFE0001F0
    dmac0->channel[1].mfcr = 0x05;      // memory fc = 5
    dmac0->channel[1].mar = 0x00010000; // memory address = 0x00010000
    dmac0->channel[1].bfcr = 0x05;      // base fc = 5
    dmac0->channel[1].bar = 0x00010000; // base address = memory address

    dmac0->channel[3].gcr = 0x00;       // 16 clocks, 50%

    struct mc68440_regs *dmac1 = (void*)0xFF000400;
    dmac1->channel[3].gcr = 0x00;       // 16 clocks, 50%
}

__attribute__((constructor))
static void init_system(void)
{
    init_scu();
    init_dmac();
    init_memmap();
    init_devices();
}
