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
                mc68681_tx(0, "\r\n");
                goto finish;
                break;
            case '\b':
                if (len > 0) {
                    mc68681_tx_polled(0, '\b');
                    mc68681_tx_polled(0, ' ');
                    mc68681_tx_polled(0, '\b');
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

    char buf[12];

    for (int i = 0; i < len; i++) {
        if (!(i & 0xF)) {
            uint32_t addr = (uint32_t)p;
            addr += i;
            snprintf(buf, sizeof(buf), "\r\n%08lX ", addr);
            mc68681_tx(0, buf);
        }

        snprintf(buf, sizeof(buf), "%02X ", *bp++);
        mc68681_tx(0, buf);
    }
    mc68681_tx(0, "\r\n");
}

static int parse_line(char *buf, int buflen)
{
    // state 0: get start address
    // state 1: get address
    // state 2: get bytes
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
                                mc68681_tx(0, "?ERROR\r\n");
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
                    ((void (*)(void))addr)();
                    break;
                case '?':  // help
                    state = 4;
                    mc68681_tx(0, "<addr>               Dump a byte from given address\r\n");
                    mc68681_tx(0, "<addr>.<addr>        Dump multiple bytes from given address range\r\n");
                    mc68681_tx(0, "<addr>:<byte> ...    Write one or more byte to given address\r\n");
                    mc68681_tx(0, "R<addr>              Jump and run code from given address\r\n");
                    goto finish;
                default:
                    mc68681_tx(0, "?UNKNOWN CHAR\r\n");
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
                    mc68681_tx(0, "?ERROR\r\n");
                    break;
                }
                hexdump((void*)start_addr, addr - start_addr + 1);
                state = 0;
                break;
            case 2:
                *(uint8_t*)start_addr++ = addr;
                break;
            default:
                mc68681_tx(0, "?UNKNOWN CHAR\r\n");
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
        mc68681_tx(0, "> ");
        parse_line(linebuf, read_line(linebuf, sizeof(linebuf)));
    }
}
