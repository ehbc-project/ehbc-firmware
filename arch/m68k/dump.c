#include <stdint.h>
#include <stddef.h>

#include "exception.h"

#include "debug.h"

char xdigit(int h) {
    if (h < 10) return h + '0';
    else return h + 'A' - 10;
}

char* hexstr(uint32_t val, char* str, int len) {
    for (int i = len; i >= 0; i--) {
        str[i - 1] = xdigit(val & 0xF);
        val >>= 4;
    }
    return str;
}

__attribute__((packed))
struct reg_dump {
    uint32_t sp;
    uint32_t d[8];
    uint32_t a[6];
};

const char* exception_name[] = {
    NULL, NULL,
    "Bus Error",
    "Address Error",
    "Illegal Instruction",
    "Divide by Zero",
    "CHK Instruction",
    "TRAP Instrudtion",
    "Privilege Violation",
    "Trace",
    "Line A",
    "Line F",
    NULL,
    "Coprocessor Protocol Violation",
    "Format Error",
    "Uninitialized Interrupt",
};

void handle_error(struct reg_dump regs, struct stack_frame frame)
{
    char str[16];

    debug_write("Exception (");
    debug_write(exception_name[frame.vector >> 2]);
    debug_write(") Occured!");

    debug_write("\r\n\r\nD0: ");
    debug_write(hexstr(regs.d[0], str, 8));

    debug_write(" D1: ");
    debug_write(hexstr(regs.d[1], str, 8));

    debug_write(" D2: ");
    debug_write(hexstr(regs.d[2], str, 8));

    debug_write(" D3: ");
    debug_write(hexstr(regs.d[3], str, 8));

    debug_write("\r\nD4: ");
    debug_write(hexstr(regs.d[4], str, 8));

    debug_write(" D5: ");
    debug_write(hexstr(regs.d[5], str, 8));

    debug_write(" D6: ");
    debug_write(hexstr(regs.d[6], str, 8));
    
    debug_write(" D7: ");
    debug_write(hexstr(regs.d[7], str, 8));

    debug_write("\r\nA0: ");
    debug_write(hexstr(regs.a[0], str, 8));

    debug_write(" A1: ");
    debug_write(hexstr(regs.a[1], str, 8));

    debug_write(" A2: ");
    debug_write(hexstr(regs.a[2], str, 8));
    
    debug_write(" A3: ");
    debug_write(hexstr(regs.a[3], str, 8));

    debug_write("\r\nA4: ");
    debug_write(hexstr(regs.a[4], str, 8));
    
    debug_write(" A5: ");
    debug_write(hexstr(regs.a[5], str, 8));
    
    if (frame.sr & 0x2000) {
        if (frame.sr & 0x1000) {
            debug_write(" MSP: ");
        } else {
            debug_write(" ISP: ");
        }
    } else {
        debug_write(" USP: ");
    }
    debug_write(hexstr(regs.sp, str, 8));

    debug_write("\r\nPC: ");
    debug_write(hexstr(frame.pc, str, 8));
    
    str[4] = 0;
    debug_write(" SR: ");
    debug_write(hexstr(frame.sr, str, 4));

    debug_write("\r\n\r\nException Vector Frame: Format #");
    str[0] = xdigit(frame.format);
    str[1] = 0;
    debug_write(str);
    switch (frame.format) {
        case 0x2:
            debug_write("\r\nInstruction Address: ");
            debug_write(hexstr(frame.fmt2h.instr_addr, str, 8));
            break;
        case 0x9:
            debug_write("\r\nInstruction Address: ");
            debug_write(hexstr(frame.fmt2h.instr_addr, str, 8));
            debug_write("\r\nInternal Register #0: ");
            debug_write(hexstr(frame.fmt9h.internal[0], str, 4));
            debug_write("\r\nInternal Register #1: ");
            debug_write(hexstr(frame.fmt9h.internal[1], str, 4));
            debug_write("\r\nInternal Register #2: ");
            debug_write(hexstr(frame.fmt9h.internal[2], str, 4));
            debug_write("\r\nInternal Register #3: ");
            debug_write(hexstr(frame.fmt9h.internal[3], str, 4));
            break;
        case 0xA:
        case 0xB:
        default:
            break;
    }

    debug_write("\r\n\r\nStack Trace:");
    void* sp = (void*)regs.sp;
    void* pc = (void*)frame.pc;
    for (int i = 0; pc != NULL; i++) {
        debug_write("\r\nStack Frame #");
        str[0] = i + '0';
        str[1] = 0;
        debug_write(str);
        debug_write(" (");
        debug_write(hexstr((uint32_t)pc, str, 8));
        debug_write("): ");
        debug_write(hexstr((uint32_t)sp, str, 8));
        sp = (void*)*((uint32_t*)sp);
        pc = (void*)*((uint32_t*)sp + 1);
    }

    debug_write("\r\n\r\nCode:");
    pc = (void*)(frame.pc & 0xFFFFFFF0);
    for (int i = 0; i < 4; i++) {
        debug_write("\r\n");
        debug_write(hexstr((uint32_t)pc, str, 8));
        debug_write(": ");
        str[2] = 0;
        for (int j = 0; j < 16; j++) {
            debug_write(hexstr(*(uint8_t*)pc, str, 2));
            debug_write(" ");
            pc = (uint8_t*)pc + 1;
        }
    }
    debug_write("\r\n");

    for (;;) {}
}
