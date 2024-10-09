#ifndef __DISK_H__
#define __DISK_H__

#include <stdint.h>

typedef int64_t lba_t;

struct chs {
    int cylinder;
    int head;
    int sector;
};

extern const struct chs ata5_geom;              // ATA-5 virtual geometry

struct chs lba_to_chs(lba_t lba, const struct chs *geom);
lba_t chs_to_lba(struct chs chs, const struct chs *geom);

#endif // __DISK_H__
