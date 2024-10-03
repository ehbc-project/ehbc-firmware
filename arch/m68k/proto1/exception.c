#include <stdint.h>
#include <stddef.h>

void error_handler();
void trap0_handler();

__attribute__((interrupt_handler))
static void unimplemented_instruction()
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
static void interrupt_autovector()
{

}

void (*exception_vector[256])() = {
    NULL,
    NULL,
    error_handler,
    error_handler,

    error_handler,
    error_handler,
    chk_instruction,
    NULL,

    error_handler,
    trace,
    unimplemented_instruction,
    unimplemented_instruction,

    NULL,
    error_handler,
    error_handler,
    error_handler,

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,

    error_handler,
    interrupt_autovector,
    interrupt_autovector,
    interrupt_autovector,
    interrupt_autovector,
    interrupt_autovector,
    interrupt_autovector,
    interrupt_autovector,

    trap0_handler,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    
    error_handler,
    error_handler,
    error_handler,
    error_handler,
    error_handler,
    error_handler,
    error_handler,
    error_handler,
    error_handler,
};
