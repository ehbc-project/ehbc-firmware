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
        .get_name = ps2kbms_get_name,
        .get_vendor = ps2kbms_get_vendor,
        .probe = ps2kbms_probe,
        .reset = ps2kbms_reset,
        .keyboard_ops = { NULL },
        .param = &ps2kbms
    },
    {
        .class = DC_MOUSE,
        .get_name = ps2kbms_get_name,
        .get_vendor = ps2kbms_get_vendor,
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

static void init_devices(void)
{
    init_mc68681_buf();

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

__attribute__((packed))
struct mc68440_channel_regs {
    hwreg8_t csr, cer;
    hwreg16_t reserved0;
    hwreg8_t dcr, ocr;
    hwreg8_t scr, ccr;
    hwreg16_t reserved1;
    hwreg16_t mtcr;
    hwreg32_t mar;
    hwreg32_t reserved2;
    hwreg32_t dar;
    hwreg16_t reserved3;
    hwreg16_t btcr;
    hwreg32_t bar;
    hwreg32_t reserved4;
    hwreg8_t reserved5, nivr;
    hwreg8_t reserved6, eivr;
    hwreg8_t reserved7, mfcr;
    hwreg16_t reserved8;
    hwreg8_t reserved9, cpr;
    hwreg16_t reserved10;
    hwreg8_t reserved11, dfcr;
    hwreg32_t reserved12;
    hwreg16_t reserved13;
    hwreg8_t reserved14, bfcr;
    hwreg32_t reserved15;
    hwreg8_t reserved16, gcr;
};

__attribute__((packed))
struct mc68440_regs {
    struct mc68440_channel_regs channel[4];
};

int dmac_setup_floppy_write(const void *buf, int count)
{
    struct mc68440_regs *dmac0 = (void*)0xFF000300;
    dmac0->channel[0].mar = (uint32_t)buf;
    dmac0->channel[0].bar = (uint32_t)buf;
    dmac0->channel[0].ocr &= ~0x80;
    dmac0->channel[0].mtcr = count;
    dmac0->channel[0].btcr = count;
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
    return 0;
}

__attribute__((constructor))
static void init_system(void)
{
    mmio_write_8(0x00, 0x07);  // set clock to 33 MHz
    mmio_write_8(0x10, 0x00);  // irq0= disabled    irq1= disabled
    mmio_write_8(0x11, 0x00);  // irq2= disabled    irq3= disabled
    mmio_write_8(0x12, 0x00);  // irq4= disabled    irq5= disabled
    mmio_write_8(0x13, 0xA0);  // irq6= level2      irq7= disabled
    mmio_write_8(0x14, 0x00);  // irq8= disabled    irq9= disabled
    mmio_write_8(0x15, 0x00);  // irq10=disabled    irq11=disabled
    mmio_write_8(0x16, 0x00);  // irq12=disabled    irq13=disabled
    mmio_write_8(0x17, 0x00);  // irq14=disabled    irq15=disabled
    mmio_write_8(0x18, 0xA0);  // irq16=level2      irq17=disabled
    mmio_write_8(0x19, 0x00);  // irq18=disabled    irq19=disabled
    mmio_write_8(0x1A, 0x00);  // irq20=disabled    irq21=disabled
    mmio_write_8(0x1B, 0x00);  // irq22=disabled    irq23=disabled

    init_memmap();

    init_devices();

    // init dma controllers
    struct mc68440_regs *dmac0 = (void*)0xFF000300;
    // dmac0ch0: floppy dma
    dmac0->channel[0].dcr = 0x00;       // burst, explicit m68k, 8-bit, pcl status
    dmac0->channel[0].ocr = 0x82;       // d2m, byte, no chain, external
    dmac0->channel[0].scr = 0x04;       // memory count up, device no count
    dmac0->channel[0].cpr = 0x00;       // channel priority 0
    dmac0->channel[0].mtcr = 0x01FF;    // 512 B memory transfer
    dmac0->channel[0].btcr = 0x01FF;    // 512 B base transfer
    dmac0->channel[0].dfcr = 0x05;      // device fc = 5
    dmac0->channel[0].dar = 0xFE0003f4; // device address = 0xFE0003f4
    dmac0->channel[0].mfcr = 0x05;      // memory fc = 5
    dmac0->channel[0].mar = 0x00010000; // memory address = 0x00010000
    dmac0->channel[0].bfcr = 0x05;      // base fc = 5
    dmac0->channel[0].bar = 0x00010000; // base address = 0xFE0003f4

    dmac0->channel[3].gcr = 0x00;       // 16 clocks, 50%

    struct mc68440_regs *dmac1 = (void*)0xFF000400;
    dmac1->channel[0].dcr = 0x08;       // burst, explicit, 16-bit, pcl status
    dmac1->channel[0].ocr = 0x90;       // d2m, word, no chain, internal, limited rate
    dmac1->channel[0].scr = 0x04;       // memory count up, device no count
    dmac1->channel[0].cpr = 0x00;       // channel priority 0
    dmac1->channel[0].mtcr = 0xFFFF;    // 64 kB memory transfer
    dmac1->channel[0].btcr = 0xFFFF;    // 64 kB base transfer
    dmac1->channel[0].dfcr = 0x05;      // device fc = 5
    dmac1->channel[0].dar = 0xFD000004; // device address = 0x00000004
    dmac1->channel[0].mfcr = 0x05;      // memory fc = 5
    dmac1->channel[0].mar = 0x00010000; // memory address = 0x00010000
    dmac1->channel[0].bfcr = 0x05;      // base fc = 5
    dmac1->channel[0].bar = 0x00010000; // base address = 0x00010000

    dmac1->channel[3].gcr = 0x00;       // 16 clocks, 50%
}
