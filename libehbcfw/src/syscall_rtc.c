#include "libehbcfw/syscall.h"

time_t ehbcfw_rtc_get_time(int id)
{
    time_t time;
    __syscall2(5, 0, id, (long)&time);
    return time;
}
