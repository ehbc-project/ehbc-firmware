#include <time.h>

static const int mon_days[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

// daylight saving time is ignored
time_t mktime(struct tm *tm)
{
    time_t ret = 0;

    int sec = tm->tm_sec;
    int min = tm->tm_min;
    int hour = tm->tm_hour;
    int mday = tm->tm_mday - 1;
    int mon = tm->tm_mon;
    int year = tm->tm_year + 1900;

    // assume that the year is after 1970, should be fixed
    for (int i = 1970; i < year; i++) {
        // check if leap year
        if ((!(i & 3) && (i % 100)) || !(i % 400)) {
            ret += 366 * 24 * 60 * 60;
        } else {
            ret += 365 * 24 * 60 * 60;
        }
    }

    for (int i = 0; i < mon; i++) {
        ret += mon_days[i] * 24 * 60 * 60;
        if (i == 1 && ((!(year & 3) && (year % 100)) || !(year % 400))) {
            ret += 24 * 60 * 60;
        }
    }

    ret += mday * 24 * 60 * 60;
    ret += hour * 60 * 60;
    ret += min * 60;
    ret += sec;

    return ret;
}
