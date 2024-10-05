#include <stdint.h>
#include <stddef.h>

#include "hw/mc68681.h"

struct syscall_args {
    uint32_t d[5];
};

static long syscall_handler_aio(const struct syscall_args *args)
{
    switch (args->d[0] & 0xFFFF) {
        case 1:
            return mc68681_tx_polled((args->d[1] >> 8) & 0xFF, args->d[1] & 0xFF);
        case 2:
            mc68681_write_byte((args->d[1] >> 8) & 0xFF, args->d[1] & 0xFF);
            return 0;
        case 6:
            return mc68681_txn((args->d[1] >> 24) & 0xFF, (const char*)args->d[2], args->d[1] & 0xFFFFFF);
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
