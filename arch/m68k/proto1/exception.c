#include "exception.h"

#include <stdint.h>
#include <stddef.h>

#include "debug.h"
#include "proto1/scu.h"
#include "hw/floppy.h"
#include "hw/ps2kbms.h"
#include "hw/mc68681.h"

#define VOIDPTR(f) ((void*)(long)(f))
#define FUNC(f) ((void (*)(void))(long)(f))

void error_handler();
void trap0_handler();

__attribute__((interrupt_handler))
static void chk_instruction()
{

}

__attribute__((interrupt_handler))
static void misc_trap_instruction()
{

}

__attribute__((interrupt_handler))
static void trace()
{

}

extern struct device *const mc68681_device;
extern struct device *const floppy_device;
extern struct device *const ps2kbms_device;

static struct scu_regs *const scu = (void*)0xFF000000;

__attribute__((interrupt_handler))
static void irq1_handler(void)
{
    ps2kbms_irq_handler(ps2kbms_device);
    scu->isr_iar.iar[2];  // ack level 3
}

__attribute__((interrupt_handler))
static void irq6_handler(void)
{
    floppy_irq_handler(floppy_device);
    scu->isr_iar.iar[1];  // ack level 2
}

__attribute__((interrupt_handler))
static void irq8_handler(void)
{
    static int int_count = 0;
    debug_printf("%d\n", int_count++);
    scu->isr_iar.iar[5];  // ack level 5
}

__attribute__((interrupt_handler))
static void irq12_handler(void)
{
    ps2kbms_irq_handler(ps2kbms_device);
    scu->isr_iar.iar[2];  // ack level 3
}

__attribute__((interrupt_handler))
static void irq14_handler(void)
{
    debug_printf("irq14\n");
    scu->isr_iar.iar[0];  // ack level 1
}

__attribute__((interrupt_handler))
static void irq16_handler(void)
{
    mc68681_irq_handler(mc68681_device);
    scu->isr_iar.iar[1];  // ack level 2
}

struct exception_vector exception_vector = {
    .bus_error =                    VOIDPTR(error_handler),
    .addr_error =                   VOIDPTR(error_handler),

    .illegal_instr =                VOIDPTR(error_handler),
    .divzero =                      VOIDPTR(error_handler),
    .chk_instr =                    VOIDPTR(chk_instruction),
    .misc_trap_instr =              VOIDPTR(misc_trap_instruction),

    .privilege_violation =          VOIDPTR(error_handler),
    .trace =                        VOIDPTR(trace),
    .line_1010 =                    VOIDPTR(error_handler),
    .line_1111 =                    VOIDPTR(error_handler),

    .coproc_protocol_violation =    VOIDPTR(error_handler),
    .format_error =                 VOIDPTR(error_handler),
    .uninitialized_interrupt =      VOIDPTR(error_handler),

    .spurious_interrupt =           NULL,
    .autovector1 =                  NULL,
    .autovector2 =                  NULL,
    .autovector3 =                  NULL,
    .autovector4 =                  NULL,
    .autovector5 =                  NULL,
    .autovector6 =                  NULL,
    .autovector7 =                  NULL,

    .trap0 =                        VOIDPTR(trap0_handler),
    
    .fpcp_bra_set_unordered =       VOIDPTR(error_handler),
    .fpcp_inexact =                 VOIDPTR(error_handler),
    .fpcp_divzero =                 VOIDPTR(error_handler),
    .fpcp_underflow =               VOIDPTR(error_handler),
    .fpcp_operand_error =           VOIDPTR(error_handler),
    .fpcp_overflow =                VOIDPTR(error_handler),
    .fpcp_signaling_nan =           VOIDPTR(error_handler),
    .fpcp_unimpl_data_type =        VOIDPTR(error_handler),

    .mmu_config_error =             VOIDPTR(error_handler),
    .mmu_illegal_op_error =         VOIDPTR(error_handler),
    .mmu_access_violation =         VOIDPTR(error_handler),

    .userdef = {
        NULL,  // irq0
        VOIDPTR(irq1_handler),  // irq1
        NULL,  // irq2
        NULL,  // irq3
        NULL,  // irq4
        NULL,  // irq5
        VOIDPTR(irq6_handler),  // irq6
        NULL,  // irq7
        VOIDPTR(irq8_handler),  // irq8
        NULL,  // irq9
        NULL,  // irq10
        NULL,  // irq11
        VOIDPTR(irq12_handler),  // irq12
        NULL,  // irq13
        VOIDPTR(irq14_handler),  // irq14
        NULL,  // irq15
        VOIDPTR(irq16_handler),  // irq16
        NULL,  // irq17
        NULL,  // irq18
        NULL,  // irq19
        NULL,  // irq20
        NULL,  // irq21
        NULL,  // irq22
        NULL,  // irq23
    }
};
