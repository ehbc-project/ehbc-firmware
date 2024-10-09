#ifndef HW_FLOPPY_H__
#define HW_FLOPPY_H__

#include <libehbcfw/disk.h>
#include <libehbcfw/device.h>

#include "types.h"

enum floppy_size {
    FSZ_525 = 0, FSZ_35
};

enum floppy_data_rate {
    FDR_500K = 0, FDR_300K, FDR_250K, FDR_1M
};

struct floppy_info {
    struct chs chs;
    enum floppy_size size;
    enum floppy_data_rate data_rate;
};

struct device_fdc {
    uint16_t port;
    volatile int irq_done;
    uint8_t dor;
    uint8_t recalib_status;
    uint8_t state[4];
    uint8_t track[4];
};

void floppy_irq_handler(struct device *dev);

const char *floppy_get_name(struct device *dev);
const char *floppy_get_vendor(struct device *dev);

int floppy_drive0_probe(struct device *dev);
int floppy_drive1_probe(struct device *dev);
int floppy_drive2_probe(struct device *dev);
int floppy_drive3_probe(struct device *dev);

int floppy_drive0_reset(struct device *dev);
int floppy_drive1_reset(struct device *dev);
int floppy_drive2_reset(struct device *dev);
int floppy_drive3_reset(struct device *dev);

int floppy_drive0_read_sectors_chs(struct device *dev, struct chs chs, int count, void *buf);
int floppy_drive0_read_sectors_lba(struct device *dev, lba_t lba, int count, void *buf);

#endif  // HW_FLOPPY_H__
