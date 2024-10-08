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
extern const struct chs floppy_5_25_dsdd_geom;  // 5 1/4 inch, 360 kB
extern const struct chs floppy_5_25_dshd_geom;  // 5 1/4 inch, 1.2 MB
extern const struct chs floppy_3_5_dsdd_geom;   // 3 1/2 inch, 720 kB
extern const struct chs floppy_3_5_dshd_geom;   // 3 1/2 inch, 1.44 MB
extern const struct chs floppy_3_5_dsed_geom;   // 3 1/2 inch, 2.88 MB

struct chs lba_to_chs(lba_t lba, const struct chs *geom);
lba_t chs_to_lba(struct chs chs, const struct chs *geom);

#endif // __DISK_H__
