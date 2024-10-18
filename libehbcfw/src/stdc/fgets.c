#include <stdio.h>

#include <libehbcfw/syscall.h>

char *fgets(char *str, int num, FILE *stream)
{
    int len = 0;
    char rxbyte;
    char *buf = str;

    do {
        if (ehbcfw_aio_rx(0, &rxbyte)) break;

        switch (rxbyte) {
            case '\n':
                if (len > 0 && buf[-1] == '\r') {
                    break;
                }
            case '\r':
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
                printf("%c", rxbyte);
                *buf++ = rxbyte;
                len++;
        }
    } while (rxbyte && len < num - 1);

finish:
    *buf = 0;
    return str;
}
