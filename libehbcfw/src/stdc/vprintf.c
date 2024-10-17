#include <stdio.h>

#include <libehbcfw/syscall.h>

int vprintf(const char *fmt, va_list args)
{
    char buf[PRINTF_MAX_LEN];
    int ret = vsnprintf(buf, sizeof(buf), fmt, args);

    ehbcfw_video_write_ansi_tty(2, buf, sizeof(buf));
    return ret;
}
