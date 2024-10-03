#include <stdint.h>
#include <stddef.h>

#include "hw/mc68681.h"

struct syscall_args {
    uint32_t d[3];
    void *a[2];
};

void syscall_handler(struct syscall_args args) {
    if (args.d[0] == 0x00000002) {
        mc68681_tx_polled((args.d[1] >> 8) & 0xFF, args.d[1] & 0xFF);
    }
    return;
}
