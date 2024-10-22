#include <time.h>

#include "libehbcfw/syscall.h"

time_t time(time_t *time)
{
    time_t ret = ehbcfw_rtc_get_time(9);
    *time = ret;
    return ret;
}
