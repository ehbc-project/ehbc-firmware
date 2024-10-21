#include <stdio.h>

#include <libehbcfw/syscall.h>

char *fgets(char *str, int num, FILE *stream)
{
    int len = 0;
    int rxdata;
    char *buf = str;

    while ((rxdata = ehbcfw_kbd_get_char(3)) > 0 && len < num) {
        switch (rxdata) {
            case '\r':  // ignore carriage return
                break;
            case '\n':
                printf("\r\n");
                *buf++ = '\n';
                len++;
                goto finish;
            case '\b':
                if (len > 0) {
                    printf("\b \b");
                    *--buf = 0;
                    len--;
                }
                break;
            default:
                printf("%c", rxdata);  // echo input
                *buf++ = rxdata;
                len++;
        }
    };

finish:
    *buf = 0;
    return str;
}
