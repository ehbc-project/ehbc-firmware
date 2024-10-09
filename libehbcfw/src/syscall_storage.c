#include "libehbcfw/syscall.h"

int ehbcfw_storage_read_sectors_chs(int id, struct chs chs, int count, void *buf)
{
    uint32_t d2 =
        ((chs.cylinder & 0x3FF) << 6) |
        ((chs.head & 0xFF) << 16) |
        (chs.sector & 0x3F);
    return __syscall3(4, 3, (id & 0xFF) | ((count & 0xFF) << 8), d2, (long)buf);
}

int ehbcfw_storage_read_sectors_lba(int id, lba_t lba, int count, void *buf)
{
    return __syscall3(4, 6, (id & 0xFF) | ((count & 0xFF) << 8) | ((lba >> 32) & 0xFFFF), lba & 0xFFFFFFFF, (long)buf);
}
