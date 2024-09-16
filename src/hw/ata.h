#ifndef HW_ATA_H__
#define HW_ATA_H__

#include "types.h"

struct chs {
    uint16_t head;
    uint16_t cylinder;
    uint16_t sector;
};

int ata_init(void);
int ata_identify_device(void *buf, int len);
int ata_read(void *buf, uint64_t lba, uint16_t count);
int ata_write(const void *buf, uint64_t lba, uint16_t count);


#endif  // HW_ATA_H__
