#include "io.h"

#include <stdio.h>

void debug_write(const char *str)
{
    for (; *str; str++)
        io_write_8(0xE9, *str);
}

void debug_printf(const char *fmt, ...)
{
    va_list args;
    char buf[PRINTF_MAX_LEN];

    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    debug_write(buf);
}
