#ifndef HW_FLOPPY_H__
#define HW_FLOPPY_H__

#include "types.h"

struct chs {
    uint16_t head;
    uint16_t cylinder;
    uint16_t sector;
};

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

int floppy_init(int id, int type);

#endif  // HW_FLOPPY_H__
