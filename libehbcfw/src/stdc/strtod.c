#include <stdlib.h>

#include <math.h>
#include <errno.h>
#include <limits.h>

double strtod(const char *str, char **endptr)
{
    double ret;
    int ival = 0, dval = 0, ddigit = 0, exp = 0;
    int sign = 0, expsign = 0;  // 0: positive 1: negative
    int hex = 0;  // 0: decimal 1: hexadecimal

    while (*str && *str == ' ') {  // trim leading spaces
        str++;
    }

    if (*str == '+') {
        sign = 0;
        str++;
    } else if (*str == '-') {
        sign = 1;
        str++;
    }

    if (*str == '0') {
        if (str[1] != 'x' && str[1] != 'X') {
            errno = EINVAL;
            return 0;
        }
        hex = 1;
        str += 2;
    }

    while (*str) {
        int digit = 0;
        if (*str == '.' || *str == 'e' || *str == 'E') {
            break;
        } else if ('0' <= *str && *str <= '9') {
            digit = *str - '0';
        } else {
            errno = EINVAL;
            break;
        }

        ival *= 10;
        ival += digit;
        str++;
    }

    if (*str == '.') {
        str++;
        while (*str) {
            int digit = 0;
            if (*str == 'e' || *str == 'E') {
                break;
            } else if ('0' <= *str && *str <= '9') {
                digit = *str - '0';
            } else {
                errno = EINVAL;
                break;
            }

            dval *= 10;
            dval += digit;
            str++;
            ddigit++;
        }
    }

    if (*str == 'E' || *str == 'e') {
        str++;
        if (*str == '+') {
            expsign = 0;
            str++;
        } else if (*str == '-') {
            expsign = 1;
            str++;
        }

        while (*str) {
            int digit = 0;
            if ('0' <= *str && *str <= '9') {
                digit = *str - '0';
            } else {
                errno = EINVAL;
                break;
            }

            exp *= 10;
            exp += digit;
            str++;
        }
    }

    if (expsign) {
        exp = -exp;
    }

    // workaround: MAME does not support fetox.x instruction
    ret = ival;
    // ret = ((double)ival + ((double)dval * pow(10, -ddigit))) * pow(10, exp);

    if (sign) {
        ret = -ret;
    }

    if (endptr) {
        *endptr = (char*)str;
    }

    return ret;
}
