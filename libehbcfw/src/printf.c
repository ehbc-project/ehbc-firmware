#include <stdio.h>

#include <stdint.h>

extern void do_system_call(uint32_t d0, uint32_t d1);

int printf(const char *fmt, ...)
{
    va_list args;
    char buf[PRINTF_MAX_LEN];

    va_start(args, fmt);
    int ret = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    for (int i = 0; i < sizeof(buf) && buf[i]; i++) {
        do_system_call(2, buf[i]);
    }
    return ret;
}
