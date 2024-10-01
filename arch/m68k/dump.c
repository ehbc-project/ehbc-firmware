#include <stdint.h>
#include <stddef.h>

#include "exception.h"

#include "debug.h"


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
    debug_printf("Exception (%s) Occured!\n\n", exception_name[frame.vector >> 2]);

    debug_printf("D0=%08lX D1=%08lX D2=%08lX D3=%08lX\n", regs.d[0], regs.d[1], regs.d[2], regs.d[3]);
    debug_printf("D5=%08lX D6=%08lX D7=%08lX D8=%08lX\n", regs.d[5], regs.d[6], regs.d[7], regs.d[8]);
    debug_printf("A0=%08lX A1=%08lX A2=%08lX A3=%08lX\n", regs.a[0], regs.a[1], regs.a[2], regs.a[3]);
    debug_printf("A4=%08lX A5=%08lX SP=%08lX\n", regs.a[4], regs.a[5], regs.sp);
    debug_printf("PC=%08lX SR=%04X\n\n", frame.pc, frame.sr);

    debug_printf("Exception Vector Frame: Format #%X\n", frame.format);

    switch (frame.format) {
        case 0x2:
            debug_printf("Instruction Address: %08lX\n", frame.fmt2h.instr_addr);
            break;
        case 0x9:
            debug_printf("Instruction Address: %08lX\n", frame.fmt9h.instr_addr);
            debug_printf("Internal Register #0: %04X\n", frame.fmt9h.internal[0]);
            debug_printf("Internal Register #1: %04X\n", frame.fmt9h.internal[1]);
            debug_printf("Internal Register #2: %04X\n", frame.fmt9h.internal[2]);
            debug_printf("Internal Register #3: %04X\n", frame.fmt9h.internal[3]);
            break;
        case 0xA:
        case 0xB:
        default:
            break;
    }

    debug_write("\nStack Trace:\n");
    void *sp = (void*)regs.sp;
    void *pc = (void*)frame.pc;
    for (int i = 0; pc != NULL; i++) {
        debug_printf("Stack Frame #%d: PC=%p SP=%p\n", i, pc, sp);
        void *ptr = (void*)((uint32_t)sp & 0xFFFFFFF0);
        for (int i = 0; i < 4; i++) {
            debug_printf("%08lX: ", (uint32_t)ptr);
            for (int j = 0; j < 16; j++) {
                debug_printf("%02X ", *(uint8_t*)ptr);
                ptr = (uint8_t*)ptr + 1;
            }
            debug_write("\r\n");
        }
        sp = (void*)*((uint32_t*)sp);
        pc = (void*)*((uint32_t*)sp + 1);
    }

    debug_write("\nCode:\n");
    pc = (void*)(frame.pc & 0xFFFFFFF0);
    for (int i = 0; i < 4; i++) {
        debug_printf("%08lX: ", (uint32_t)pc);
        for (int j = 0; j < 16; j++) {
            debug_printf("%02X ", *(uint8_t*)pc);
            pc = (uint8_t*)pc + 1;
        }
        debug_write("\r\n");
    }

    for (;;) {}
}
