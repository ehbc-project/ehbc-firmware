#include <stddef.h>


__attribute__((interrupt_handler))
static void bus_error()
{

}

__attribute__((interrupt_handler))
static void address_error()
{

}

__attribute__((interrupt_handler))
static void divide_by_zero()
{

}

__attribute__((interrupt_handler))
static void chk_instruction()
{

}

__attribute__((interrupt_handler))
static void trace()
{

}

__attribute__((interrupt_handler))
static void coproc_protocol_error()
{

}

__attribute__((interrupt_handler))
static void spurious_interrupt()
{

}

__attribute__((interrupt_handler))
static void interrupt_autovector()
{

}

__attribute__((interrupt_handler))
static void trap_instruction()
{

}

__attribute__((interrupt_handler))
static void fpu_exception()
{

}

__attribute__((interrupt_handler))
static void mmu_exception()
{

}

__attribute__((interrupt_handler))
static void other_exception()
{

}

void (*exception_vector[256])() = {
    NULL,
    NULL,
    bus_error,
    address_error,
    other_exception,
    divide_by_zero,
    chk_instruction,
    trap_instruction,
    other_exception,
    trace,
    NULL,
    other_exception,
    other_exception,
    coproc_protocol_error,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    spurious_interrupt,
    interrupt_autovector,
    interrupt_autovector,
    interrupt_autovector,
    interrupt_autovector,
    interrupt_autovector,
    interrupt_autovector,
    interrupt_autovector,
    trap_instruction,
    trap_instruction,
    trap_instruction,
    trap_instruction,
    trap_instruction,
    trap_instruction,
    trap_instruction,
    trap_instruction,
    trap_instruction,
    trap_instruction,
    trap_instruction,
    trap_instruction,
    trap_instruction,
    trap_instruction,
    trap_instruction,
    trap_instruction,
    fpu_exception,
    fpu_exception,
    fpu_exception,
    fpu_exception,
    fpu_exception,
    fpu_exception,
    fpu_exception,
    fpu_exception,
    mmu_exception,
};

void init_exception_vector()
{
    __asm__ volatile ("movec %0,%%vbr" : : "Ia" (exception_vector));
}
