.global         __init_arch
.global         __istack_end
.global         __stack_end
.global         exception_vector

.section        .text.startup

.global         _start
_start:
    bra         start

.text

start:
    lea         __istack_end,%sp        | set interrupt stack pointer

    ori.w       #0x1000,%sr             | switch to master state

    lea         __stack_end,%sp         | set master stack pointer
    movea.l     %sp,%fp                 | set frame pointer

    bset.b      #1,0xFF000000           | disable initial vector

    | init memory

    | init exception vector
    move.l      #exception_vector,%d0
    movec.l     %d0,%vbr

    | Jump to __init_arch
    bra         __init_arch
