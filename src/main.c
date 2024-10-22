#include <assert.h>
#include <stdio.h>
#include <macros.h>
#include <time.h>

#include <asm/io.h>

#include "types.h"
#include "device.h"
#include "memmap.h"
#include "hw/vga.h"
#include "hw/ps2kbms.h"
#include "hw/mc68681.h"
#include "hw/floppy.h"
#include "hw/rtc.h"
#include "fs/fat.h"
#include "cfgutil.h"
#include "splash.h"
#include "crypto/crc32.h"

extern struct device *const mc68681_device;
extern struct device *const floppy_device;

void main(void)
{
    struct ehbcfw_aio_param aio_param = {
        .baud = AIO_BAUD_9600,
        .bpc = 8,
        .parity_mode = AIO_PM_NONE,
        .stop_bits = AIO_SB_1,
    };

    // init video
    struct device *vga = find_device(2);
    const struct video_mode_info *mode = vga_get_mode_info(vga, 0x03);
    if (ehbcfw_video_set_mode(2, 0x03)) {
        printf("Video mode not found\r\n");
        return;
    }

    show_splash();

    for (int i = 0; i < 1048576 * 5; i++) {}

    cfgutil();
    
    ehbcfw_video_set_cursor_shape(2, 0x0C0F);
    volatile uint8_t* fbuf_base = (uint8_t*)mode->buf_base;
    for (int i = 0; i < mode->width * mode->height; i++) {
        fbuf_base[(i << 1) + 1] = 0x07;
    }

    // init aio channel
    if (!mc68681_cha_set_param(mc68681_device, aio_param)) {
        printf("DUART OK\r\n");
    }

    // print current time
    printf("Current UNIX timestamp: %lld\r\n", ehbcfw_rtc_get_time(9));

    // test memory
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

    // list device
    struct device *dev = get_device_list_head();
    while (dev) {
        if (dev->flags & DF_AVAILABLE) {
            printf("Device #%d: %s %s\r\n", dev->id, dev->get_vendor(dev), dev->get_name(dev));
        }
        dev = dev->next;
    }

    // print memory map
    struct memmap *map = get_memory_map_head();
    printf("+----------+----------+------------+-------+\r\n");
    printf("| start    | end      | size       | flags |\r\n");
    printf("+----------+----------+------------+-------+\r\n");
    while (map) {
        printf("| %p | %p | %7lu kB | %c%c%c%c  |\r\n", map->start, map->end,
            ((unsigned long)map->end - (unsigned long)map->start + 1) >> 10,
            (map->flags & MEM_UNMAPPED) ? 'U' : ' ',
            (map->flags & MEM_OCCUPIED) ? 'O' : ' ',
            (map->flags & MEM_MMIO) ? 'I' : ' ',
            (map->flags & MEM_READONLY) ? 'R' : ' ');
        map = map->next;
    }
    printf("+----------+----------+------------+-------+\r\n");
    
    // mount floppy
    struct fat_filesystem fs;
    if (fat_mount(&fs, 7)) {
        printf("Failed to mount FAT\r\n");
    }

    static const char *fat_type[] = {
        "UNKNOWN", "FAT12", "FAT16", "FAT32"
    };

    printf("Filesystem: %s\r\n", fat_type[fs.fat_type]);
    printf("Label: %11s\r\n", fs.volume_label);

    // iterate and list directory
    struct fat_dir dir;
    struct fat_dir_iter iter;
    if (!fat_rootdir_open(&fs, &dir) && !fat_dir_iter_start(&dir, &iter)) {
        while (!fat_dir_iter_next(&iter)) {
            printf("- Entry: %s\r\n", iter.filename);
        }
    }

    // load basic interpreter to 0x20000
    struct fat_file file;
    if (!fat_file_open(&dir, &file, "BOOT.BIN")) {
        fat_file_seek(&file, 0, SEEK_END);
        long file_size = fat_file_tell(&file);
        fat_file_seek(&file, 0, SEEK_SET);

        printf("BOOT.BIN Found. (%ld bytes)\r\n", file_size);

        uint8_t *ptr = (void*)0x10000;

        for (int i = 0; i < 50; i++) {
            printf("Loading... %3d%% [", i * 2);
            for (int j = 0; j < 50; j++) {
                printf(j < i ? "#" : " ");
            }
            printf("]\r");
            fat_file_read(&file, ptr, file_size / 50, 1);
            ptr += file_size / 50;
        }
        printf("Loading... 100%% [");
        for (int j = 0; j < 50; j++) {
            printf("#");
        }
        printf("]\r");
        fat_file_read(&file, ptr, file_size % (file_size / 50), 1);
        printf("\n");

        printf("Loaded to address 0x10000.\r\n");

        // verify checksum
        printf("crc32: %08lX\n", crc32((void*)0x10000, file_size, 0));
    }

    ehbcfw_aio_flush_rx(0);

    // run machine code monitor
    extern void run_monitor(void);
    run_monitor();
}
