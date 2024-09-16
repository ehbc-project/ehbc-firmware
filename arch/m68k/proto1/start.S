.org 0

.global __init_arch
.global __stack_end

.section    .text.startup
.global     _start
_start:
    movea.l     #__stack_end,%sp        /* Configure Stack Pointer */
    movea.l     %sp,%fp                 /* Configure Frame Pointer */

    /* init vga */

    /* Jump to __init_arch */
    bra         __init_arch

__helloworld_str:
.asciz "Hello, World!\r\n"
