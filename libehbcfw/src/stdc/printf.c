#include <stdio.h>

#include <libehbcfw/syscall.h>

int printf(const char *fmt, ...)
{
    va_list args;
    char buf[PRINTF_MAX_LEN];

    va_start(args, fmt);
    int ret = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    ehbcfw_video_write_ansi_tty(2, buf, sizeof(buf));
    return ret;
}
