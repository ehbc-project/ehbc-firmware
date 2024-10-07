#include "libehbcfw/syscall.h"

int ehbcfw_storage_read_sectors_lba(int id, long long lba, int count, void *buf)
{
    return __syscall1(4, 6, (id & 0xFF));
}
