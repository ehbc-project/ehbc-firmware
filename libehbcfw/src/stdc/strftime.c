#include <time.h>

#include <macros.h>

static const char *weekday_name[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

static const char *month_name[] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

static int process_placeholder(char *str, size_t maxsize, char type, const struct tm *tm)
{
    int char_count = 0;

    // I'm too lazy to think...
    switch (type) {
        case 'a': {
            const char *wdstr = weekday_name[tm->tm_wday];
            while (maxsize > 0 && char_count < 3) {
                *str++ = *wdstr++;
                maxsize--;
                char_count++;
            }
        } break;
        case 'A': {
            const char *wdstr = weekday_name[tm->tm_wday];
            while (maxsize > 0) {
                *str++ = *wdstr++;
                maxsize--;
                char_count++;
            }
        } break;
        case 'h':
            break;
        case 'b': {
            const char *monstr = month_name[tm->tm_mon];
            while (maxsize > 0 && char_count < 3) {
                *str++ = *monstr++;
                maxsize--;
                char_count++;
            }
        } break;
        case 'B': {
            const char *monstr = month_name[tm->tm_mon];
            while (maxsize > 0) {
                *str++ = *monstr++;
                maxsize--;
                char_count++;
            }
        } break;
        case 'c':
            break;
        case 'C': {
            int year = (tm->tm_year + 1900) / 100;

            if (maxsize > 0) {
                *str++ = year / 10 + '0';
                maxsize--;
                char_count++;
            }

            if (maxsize > 0) {
                *str++ = year % 10 + '0';
                maxsize--;
                char_count++;
            }
        } break;
        case 'd': {
            int mday = tm->tm_mday;

            if (maxsize > 0) {
                *str++ = mday / 10 + '0';
                maxsize--;
                char_count++;
            }

            if (maxsize > 0) {
                *str++ = mday % 10 + '0';
                maxsize--;
                char_count++;
            }
        } break;
        case 'D':
            break;
        case 'e': {
            int mday = tm->tm_mday;

            if (maxsize > 0) {
                *str++ = (mday / 10) ? (mday / 10 + '0') : ' ';
                maxsize--;
                char_count++;
            }

            if (maxsize > 0) {
                *str++ = mday % 10 + '0';
                maxsize--;
                char_count++;
            }
        } break;
        case 'F':
        case 'g':
        case 'G':
            break;
        case 'H': {
            int hour = tm->tm_hour;

            if (maxsize > 0) {
                *str++ = hour / 10 + '0';
                maxsize--;
                char_count++;
            }

            if (maxsize > 0) {
                *str++ = hour % 10 + '0';
                maxsize--;
                char_count++;
            }
        } break;
        case 'I': {
            int hour = tm->tm_hour % 12;
            if (!hour) hour = 12;

            if (maxsize > 0) {
                *str++ = hour / 10 + '0';
                maxsize--;
                char_count++;
            }

            if (maxsize > 0) {
                *str++ = hour % 10 + '0';
                maxsize--;
                char_count++;
            }
        } break;
        case 'j': {
            int yday = tm->tm_yday;

            if (maxsize > 0) {
                *str++ = yday / 100 + '0';
                maxsize--;
                char_count++;
            }

            if (maxsize > 0) {
                *str++ = yday / 10 + '0';
                maxsize--;
                char_count++;
            }

            if (maxsize > 0) {
                *str++ = yday % 10 + '0';
                maxsize--;
                char_count++;
            }
        } break;
        case 'm': {
            int mon = tm->tm_mon;

            if (maxsize > 0) {
                *str++ = mon / 10 + '0';
                maxsize--;
                char_count++;
            }

            if (maxsize > 0) {
                *str++ = mon % 10 + '0';
                maxsize--;
                char_count++;
            }
        } break;
        case 'M': {
            int min = tm->tm_min;

            if (maxsize > 0) {
                *str++ = min / 10 + '0';
                maxsize--;
                char_count++;
            }

            if (maxsize > 0) {
                *str++ = min % 10 + '0';
                maxsize--;
                char_count++;
            }
        } break;
        case 'p': {
            if (maxsize > 0) {
                *str++ = (((tm->tm_hour + 11) / 12) & 1) ? 'A' : 'P';
                maxsize--;
                char_count++;
            }

            if (maxsize > 0) {
                *str++ = 'M';
                maxsize--;
                char_count++;
            }
        } break;
        case 'r':
        case 'R':
            break;
        case 'S': {
            int sec = tm->tm_sec;

            if (maxsize > 0) {
                *str++ = sec / 10 + '0';
                maxsize--;
                char_count++;
            }

            if (maxsize > 0) {
                *str++ = sec % 10 + '0';
                maxsize--;
                char_count++;
            }
        } break;
        case 'T':
            break;
        case 'u': {
            int wday = tm->tm_wday;
            if (!wday) wday = 7;

            if (maxsize > 0) {
                *str++ = wday + '0';
                maxsize--;
                char_count++;
            }
        } break;
        case 'U':
        case 'V':
        case 'w':
        case 'W':
        case 'x':
        case 'X':
            break;
        case 'Y': {
            int year = tm->tm_year / 100;

            if (maxsize > 0) {
                *str++ = year / 10 + '0';
                maxsize--;
                char_count++;
            }

            if (maxsize > 0) {
                *str++ = year % 10 + '0';
                maxsize--;
                char_count++;
            }
        }
        case 'y': {
            int year = tm->tm_year;

            if (maxsize > 0) {
                *str++ = year / 10 + '0';
                maxsize--;
                char_count++;
            }

            if (maxsize > 0) {
                *str++ = year % 10 + '0';
                maxsize--;
                char_count++;
            }
        } break;
        case 'z':
        case 'Z':
            break;
        case 't':
            *str++ = '\n';
            char_count = 1;
            break;
        case 'n':
            *str++ = '\n';
            char_count = 1;
            break;
        case '%':
            *str++ = '%';
            char_count = 1;
            break;
        default:
            return 0;
    }

    return char_count;
}

size_t strftime(char *str, size_t maxsize, const char *fmt, const struct tm *tm)
{
    size_t char_cnt = 0;

    while (*fmt && maxsize > 0) {
        switch (*fmt) {
            case '%':
                if (fmt[1]) {
                    int ret = process_placeholder(str, maxsize, fmt[1], tm);
                    char_cnt += ret;
                    maxsize -= ret;
                    str += ret;
                    fmt++;
                }
                break;
            default:
                if (maxsize > 0) {
                    *str++ = *fmt;
                    char_cnt++;
                    maxsize--;
                }
                break;
        }

        fmt++;
    }

    return char_cnt;
}
