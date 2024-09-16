#include "types.h"
#include "hw/mc68681.h"

int read_line(char *buf, int buflen)
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

void hexdump(const void* p, size_t len) {
    const uint8_t* bp = p;

    for (int i = 0; i < len; i++) {
        if (!(i & 0xF)) {
            mc68681_tx(0, "\r\n");

            uint32_t addr = (uint32_t)p;
            addr += i;
            char buf[9];
            for (int i = 7; i >= 0; i--) {
                buf[i] = addr & 0xF;
                buf[i] += (buf[i] > 9) ? 'A' - 10 : '0';
                addr >>= 4;
            }
            buf[8] = 0;
            
            mc68681_tx(0, buf);
            mc68681_tx_polled(0, ' ');
        }

        char buf[3];
        buf[0] = (*bp >> 4) & 0xF;
        buf[0] += (buf[0] > 9) ? 'A' - 10 : '0';
        buf[1] = *bp & 0xF;
        buf[1] += (buf[1] > 9) ? 'A' - 10 : '0';
        buf[2] = 0;
        bp++;

        mc68681_tx(0, buf);
        mc68681_tx_polled(0, ' ');
    }
    mc68681_tx(0, "\r\n");
}

int parse_line(char *buf, int buflen)
{
    // state 0: get start address
    // state 1: get address
    // state 2: get bytes
    int state = 0;
    uint32_t start_addr = 0, addr = 0;
    for (int i = 0; i < buflen && buf[i]; i++) {
        // hexadecimal
        if ('0' <= buf[i] && buf[i] <= '9') {
            addr <<= 4;
            addr |= buf[i] - '0';
        } else if ('A' <= buf[i] && buf[i] <= 'F') {
            addr <<= 4;
            addr |= buf[i] - 'A' + 10;
        } else if ('a' <= buf[i] && buf[i] <= 'f') {
            addr <<= 4;
            addr |= buf[i] - 'A' + 10;
        } else {
            switch (buf[i]) {
                case '.':
                    start_addr = addr;
                    state = 1;
                    break;
                case ':':
                    start_addr = addr;
                    state = 2;
                    break;
                case ' ':
                    switch (state) {
                        case 0:
                            hexdump((void*)addr, 1);
                            addr = 0;
                            break;
                        case 1:
                            if (addr < start_addr) {
                                mc68681_tx(0, "?ERROR\r\n");
                                break;
                            }
                            hexdump((void*)start_addr, addr - start_addr + 1);
                            state = 0;
                            addr = 0;
                            break;
                        case 2:
                            *(uint8_t*)start_addr++ = addr;
                            addr = 0;
                            break;
                        default:
                            break;
                    }
                    break;
                case 'R':
                    ((void (*)(void))addr)();
                    break;
                default:
                    mc68681_tx(0, "?UNKNOWN CHAR\r\n");
                    goto finish;
                    break;
            }
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
