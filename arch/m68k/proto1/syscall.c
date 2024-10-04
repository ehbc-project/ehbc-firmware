#include <stdint.h>
#include <stddef.h>

#include "hw/mc68681.h"

struct syscall_args {
    uint32_t d[3];
    void *a[2];
};

static long syscall_handler_aio(const struct syscall_args *args)
{
    switch (args->d[0] & 0xFFFF) {
        case 1:
            return mc68681_tx_polled((args->d[1] >> 8) & 0xFF, args->d[1] & 0xFF);
        default:
            break;
    }
    return 0;
}

long syscall_handler(struct syscall_args args) {
    switch ((args.d[0] >> 16) & 0xFFFF) {
        case 0:
            break;
        case 1:
            return syscall_handler_aio(&args);
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        default:
            break;
    }
    return 0;
}
