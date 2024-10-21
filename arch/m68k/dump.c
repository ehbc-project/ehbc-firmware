#include <stdint.h>
#include <stddef.h>

#include "exception.h"

#include "debug.h"

struct reg_dump {
    uint32_t d[8];
    void *a[8];
};

static const char* exception_name[] = {
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
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Spurious Interrupt",
    "Level 1 Interrupt Autovector",
    "Level 2 Interrupt Autovector",
    "Level 3 Interrupt Autovector",
    "Level 4 Interrupt Autovector",
    "Level 5 Interrupt Autovector",
    "Level 6 Interrupt Autovector",
    "Level 7 Interrupt Autovector",
    "TRAP #0 Instruction",
    "TRAP #1 Instruction",
    "TRAP #2 Instruction",
    "TRAP #3 Instruction",
    "TRAP #4 Instruction",
    "TRAP #5 Instruction",
    "TRAP #6 Instruction",
    "TRAP #7 Instruction",
    "TRAP #8 Instruction",
    "TRAP #9 Instruction",
    "TRAP #9 Instruction",
    "TRAP #10 Instruction",
    "TRAP #11 Instruction",
    "TRAP #12 Instruction",
    "TRAP #13 Instruction",
    "TRAP #14 Instruction",
    "TRAP #15 Instruction",
    "FPCP Branch or Set on Unordered Condition",
    "FPCP Inexact Result",
    "FPCP Divide by Zero",
    "FPCP Underflow",
    "FPCP Operand Error",
    "FPCP Overflow",
    "FPCP Signaling NaN",
    NULL,
    "MMU Configuration Error",
    NULL,
    NULL,
};

__attribute__((noreturn))
void system_dump(struct reg_dump regs, struct stack_frame frame)
{
    debug_printf("Exception #%u (%s) Occured!\n\n", frame.vector >> 2, exception_name[frame.vector >> 2]);

    debug_printf("D0=%08lX D1=%08lX D2=%08lX D3=%08lX\n", regs.d[0], regs.d[1], regs.d[2], regs.d[3]);
    debug_printf("D5=%08lX D6=%08lX D7=%08lX D8=%08lX\n", regs.d[5], regs.d[6], regs.d[7], regs.d[8]);
    debug_printf("A0=%p A1=%p A2=%p A3=%p\n", regs.a[0], regs.a[1], regs.a[2], regs.a[3]);
    debug_printf("A4=%p A5=%p A6=%p SP=%p\n", regs.a[4], regs.a[5], regs.a[6], regs.a[7]);
    debug_printf("PC=%p SR=%04X\n\n", frame.pc, frame.sr);

    debug_printf("Exception Vector Frame: Format #%X\n", frame.format);

    switch (frame.format) {
        case 0x2:
            debug_printf("Instruction Address: %p\n", frame.fmt2h.instr_addr);
            break;
        case 0x9:
            debug_printf("Instruction Address: %p\n", frame.fmt9h.instr_addr);
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
    void *sp = regs.a[7];
    void *pc = frame.pc;
    for (int i = 0; pc && sp; i++) {
        debug_printf("Stack Frame #%d: PC=%p SP=%p\n", i, pc, sp);
        void *ptr = (void*)((uint32_t)sp & 0xFFFFFFF0);
        for (int i = 0; i < 4; i++) {
            debug_printf("%p: ", ptr);
            for (int j = 0; j < 16; j++) {
                debug_printf("%02X ", *(uint8_t*)ptr);
                ptr = (uint8_t*)ptr + 1;
            }
            debug_write("\r\n");
        }
        sp = *((void**)sp);
        pc = *((void**)sp + 1);
    }

    debug_write("\nCode:\n");
    pc = (void*)((uint32_t)frame.pc & 0xFFFFFFF0);
    for (int i = 0; i < 4; i++) {
        debug_printf("%p: ", pc);
        for (int j = 0; j < 16; j++) {
            debug_printf("%02X ", *(uint8_t*)pc);
            pc = (uint8_t*)pc + 1;
        }
        debug_write("\r\n");
    }

    for (;;) {}
}
