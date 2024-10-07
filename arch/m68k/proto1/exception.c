#include "exception.h"

#include <stdint.h>
#include <stddef.h>

#include "hw/mc68681.h"

#define FUNC(f) ((void*)(long)(f))

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

__attribute__((interrupt_handler))
static void interrupt_autovector()
{
    mc68681_irq_handler(mc68681_device);
}

struct exception_vector exception_vector = {
    .bus_error =                    FUNC(error_handler),
    .addr_error =                   FUNC(error_handler),

    .illegal_instr =                FUNC(error_handler),
    .divzero =                      FUNC(error_handler),
    .chk_instr =                    FUNC(chk_instruction),
    .misc_trap_instr =              FUNC(misc_trap_instruction),

    .privilege_violation =          FUNC(error_handler),
    .trace =                        FUNC(trace),
    .line_1010 =                    FUNC(error_handler),
    .line_1111 =                    FUNC(error_handler),

    .coproc_protocol_violation =    FUNC(error_handler),
    .format_error =                 FUNC(error_handler),
    .uninitialized_interrupt =      FUNC(error_handler),

    .spurious_interrupt =           FUNC(error_handler),
    .autovector1 =                  FUNC(interrupt_autovector),
    .autovector2 =                  FUNC(interrupt_autovector),
    .autovector3 =                  FUNC(interrupt_autovector),
    .autovector4 =                  FUNC(interrupt_autovector),
    .autovector5 =                  FUNC(interrupt_autovector),
    .autovector6 =                  FUNC(interrupt_autovector),
    .autovector7 =                  FUNC(interrupt_autovector),

    .trap0 =                        FUNC(trap0_handler),
    
    .fpcp_bra_set_unordered =       FUNC(error_handler),
    .fpcp_inexact =                 FUNC(error_handler),
    .fpcp_divzero =                 FUNC(error_handler),
    .fpcp_underflow =               FUNC(error_handler),
    .fpcp_operand_error =           FUNC(error_handler),
    .fpcp_overflow =                FUNC(error_handler),
    .fpcp_signaling_nan =           FUNC(error_handler),
    .fpcp_unimpl_data_type =        FUNC(error_handler),

    .mmu_config_error =             FUNC(error_handler),
    .mmu_illegal_op_error =         FUNC(error_handler),
    .mmu_access_violation =         FUNC(error_handler),
};
