#include <stdint.h>
#include <stddef.h>

void bus_error();
void address_error();
void other_exception();

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
