#include <debug.h>

#include <stdarg.h>
#include <stdio.h>

void debug_printf(const char *fmt, ...)
{
    va_list args;
    char buf[PRINTF_MAX_LEN];

    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    debug_write(buf);
}
