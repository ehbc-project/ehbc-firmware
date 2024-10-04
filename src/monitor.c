#include "types.h"
#include "hw/mc68681.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

static int read_line(char *buf, int buflen)
{
    int rxbyte, len = 0;

    do {
        rxbyte = mc68681_rx_polled(0);
        if (rxbyte < 0) break;

        switch (rxbyte) {
            case '\r':
                printf("\r\n");
                goto finish;
            case '\b':
                if (len > 0) {
                    printf("\b \b");
                    *--buf = 0;
                    len--;
                }
                break;
            default:
                mc68681_tx_polled(0, rxbyte);
                *buf++ = rxbyte;
                len++;
        }
    } while (rxbyte && len < buflen);

finish:
    *buf = 0;
    return len;
}

static void hexdump(const void* p, size_t len) {
    const uint8_t* bp = p;

    for (int i = 0; i < len; i++) {
        if (!(i & 0xF)) {
            uint32_t addr = (uint32_t)p;
            addr += i;
            printf("\r\n%08lX ", addr);
        }

        printf("%02X ", *bp++);
    }
    printf("\r\n");
}

static int parse_line(char *buf, int buflen)
{
    // state 0: get start address
    // state 1: get address
    // state 2: get bytes
    // state 3: jump to address
    int state = 0;
    uint32_t start_addr = 0, addr = 0;
    for (int i = 0; i < buflen && buf[i]; i++) {
        // hexadecimal
        if (isdigit(buf[i])) {
            addr <<= 4;
            addr |= buf[i] - '0';
        } else if (isxdigit(buf[i])) {
            addr <<= 4;
            if (isupper(buf[i])) {
                addr |= buf[i] - 'A' + 10;
            } else {
                addr |= buf[i] - 'a' + 10;
            }
        } else {
            switch (buf[i]) {
                case '.':  // dump multiple
                    start_addr = addr;
                    state = 1;
                    break;
                case ':':  // write to memory
                    start_addr = addr;
                    state = 2;
                    break;
                case ' ':  // write to next memory
                    switch (state) {
                        case 0:
                            hexdump((void*)addr, 1);
                            break;
                        case 1:
                            if (addr < start_addr) {
                                printf("?ERROR\r\n");
                                break;
                            }
                            hexdump((void*)start_addr, addr - start_addr + 1);
                            state = 0;
                            break;
                        case 2:
                            *(uint8_t*)start_addr++ = addr;
                            break;
                        default:
                            break;
                    }
                    break;
                case 'R':  // run address
                    state = 3;
                    break;
                case '?':  // help
                    state = 4;
                    printf("<addr>               Dump a byte from given address\r\n");
                    printf("<addr>.<addr>        Dump multiple bytes from given address range\r\n");
                    printf("<addr>:<byte> ...    Write one or more byte to given address\r\n");
                    printf("R<addr>              Jump and run code from given address\r\n");
                    goto finish;
                default:
                    printf("?UNKNOWN CHAR\r\n");
                    goto finish;
            }
            addr = 0;
        }
    }

    if (buf[buflen - 1] != ' ') {
        switch (state) {
            case 0:
                hexdump((void*)addr, 1);
                break;
            case 1:
                if (addr < start_addr) {
                    printf("?ERROR\r\n");
                    break;
                }
                hexdump((void*)start_addr, addr - start_addr + 1);
                state = 0;
                break;
            case 2:
                *(uint8_t*)start_addr++ = addr;
                break;
            case 3:
                ((void (*)(void))addr)();
                break;
            default:
                printf("?UNKNOWN CHAR\r\n");
                break;
        }
    }

finish:
    return 0;
}

void run_monitor(void)
{
    for (;;) {
        char linebuf[128];
        printf("> ");
        parse_line(linebuf, read_line(linebuf, sizeof(linebuf)));
    }
}
