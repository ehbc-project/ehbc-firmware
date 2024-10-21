#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <libehbcfw/syscall.h>

#include "types.h"
#include "exec.h"

void hexdump(const void *ptr, int count)
{
    const uint8_t *bp = ptr;

    while (count > 0) {
        printf("%p", bp);
        for (int i = 0; i < 16 && count > 0; i++) {
            printf(" %02X", *bp++);
            count--;
        }
        printf("\n");
    }
}

static int parse_line(char *buf, int buflen)
{
    long addr, end_addr, lba, count, devid;
    uint8_t *diskbuf;

    // trim leading whitespaces
    while (*buf == ' ') { buf++; }

    switch (*buf) {
        case 'R':  // run code at address
        case 'r':
            buf++;
            addr = strtol(buf, &buf, 16);
            printf("Program terminated with code %d\n", exec((void*)addr));
            return 0;
        case 'L':  // load from disk to memory
        case 'l':
            buf++;
            devid = strtol(buf, &buf, 10);
            while (*buf == ' ') { buf++; }
            if (!isdigit(*buf)) break;
            lba = strtol(buf, &buf, 10);
            while (*buf == ' ') { buf++; }
            if (!isdigit(*buf)) break;
            count = strtol(buf, &buf, 10);
            while (*buf == ' ') { buf++; }
            if (!isxdigit(*buf)) break;
            addr = strtol(buf, &buf, 16);

            diskbuf = (void*)addr;

            for (int i = 0; i < count; i++) {
                ehbcfw_storage_read_sectors_lba(devid, lba + i, 1, diskbuf);
                diskbuf += 512;
            }

            printf("Loaded LBA #%lu-#%lu of device #%lu to %08lX\n", lba, lba + count - 1, devid, addr);
            return 0;
        case '?':  // help
            printf("<addr>                      Dump a byte from given address\n");
            printf("<addr>.<addr>               Dump multiple bytes from given address range\n");
            printf("<addr>:<byte> ...           Write one or more byte to given address\n");
            printf("R<addr>                     Jump and run code from given address\n");
            printf("L<dev> <lba> <count> <addr> Load sectors from disk\n");
            return 0;
            break;
        default:
            if (isxdigit(*buf)) {
                addr = strtol(buf, &buf, 16);
                if (*buf == '.') {  // dump multiple bytes
                    buf++;
                    end_addr = strtol(buf, &buf, 16);

                    hexdump((void*)addr, end_addr - addr + 1);
                } else if (*buf == ':') {  // set bytes
                    buf++;

                    while (isxdigit(*buf)) {
                        *(uint8_t*)addr++ = strtol(buf, &buf, 16);
                        while (*buf == ' ') { buf++; }
                    }
                } else {  // dump single byte
                    hexdump((void*)addr, 1);
                }
                return 0;
            }
    }

    printf("UNEXPECTED CHAR '%c'\n", *buf);
    return 1;
}

void run_monitor(void)
{
    for (;;) {
        char linebuf[128];
        printf("> ");
        fgets(linebuf, sizeof(linebuf), stdin);
        parse_line(linebuf, strnlen(linebuf, sizeof(linebuf)) - 1);
    }
}
