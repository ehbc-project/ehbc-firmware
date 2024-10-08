#ifndef HW_ATA_H__
#define HW_ATA_H__

#include <libehbcfw/disk.h>
#include <libehbcfw/device.h>

#include "types.h"

struct device_ata {
    uint16_t port_io;
    uint16_t port_ctrl;
};

int ata_identify_device(void *buf, int len);
int ata_read(void *buf, uint64_t lba, uint16_t count);
int ata_write(const void *buf, uint64_t lba, uint16_t count);

const char *ata_get_name(struct device *dev);
const char *ata_get_vendor(struct device *dev);

int ata_master_probe(struct device *dev);
int ata_slave_probe(struct device *dev);

int ata_master_reset(struct device *dev);
int ata_slave_reset(struct device *dev);

int ata_master_read_sectors_chs(struct device *dev, int head, int cyl, int sector, int count, void *buf);
int ata_slave_read_sectors_chs(struct device *dev, int head, int cyl, int sector, int count, void *buf);

int ata_master_write_sectors_chs(struct device *dev, int head, int cyl, int sector, int count, const void *buf);
int ata_slave_write_sectors_chs(struct device *dev, int head, int cyl, int sector, int count, const void *buf);

int ata_master_read_sectors_lba(struct device *dev, lba_t lba, int count, void *buf);
int ata_slave_read_sectors_lba(struct device *dev, lba_t lba, int count, void *buf);

int ata_master_write_sectors_lba(struct device *dev, lba_t lba, int count, const void *buf);
int ata_slave_write_sectors_lba(struct device *dev, lba_t lba, int count, const void *buf);

#endif  // HW_ATA_H__
