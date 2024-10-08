#ifndef __TIME_H__
#define __TIME_H__

#include <stdint.h>
#include <stddef.h>

typedef int64_t time_t;

struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};

time_t mktime(struct tm *time);

size_t strftime(char *str, size_t maxsize, const char *fmt, const struct tm *tm);

#endif // __TIME_H__
