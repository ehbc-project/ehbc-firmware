#include <stdio.h>

#include "libehbcfw/syscall.h"

int printf(const char *fmt, ...)
{
    va_list args;
    char buf[PRINTF_MAX_LEN];

    va_start(args, fmt);
    int ret = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    for (int i = 0; i < sizeof(buf) && buf[i]; i++) {
        ehbcfw_aio_tx(0, buf[i]);
    }
    return ret;
}
