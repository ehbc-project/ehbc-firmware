.global         main

.text

.global         __init_arch
__init_arch:
    | initialize .bss section
    lea         __bss_start,%a0
    lea         __bss_end,%a1
.Lloop_bss:
    cmpa.l      %a1,%a0
    bge         .Lend_bss
    clr.l       %a0@+
    bra         .Lloop_bss
.Lend_bss:  

    | initialize .data section
    lea         __data_init_start,%a0
    lea         __data_start,%a1
    lea         __data_init_end,%a2
.Lloop_data:
    cmpa.l      %a2,%a0
    bge         .Lend_data
    move.l      %a1@+,%a0@+
    bra         .Lloop_data
.Lend_data:

    andi.w      #0xF0FF,%sr             | enable interrupt

    | run ctors
    lea         __init_array_start,%a2
    lea         __init_array_end,%a3
.Lloop_ctors:
    cmpa.l      %a3,%a2
    bge         .Lend_ctors
    movea.l     %a2@+,%a4
    jsr         %a4@
    bra         .Lloop_ctors
.Lend_ctors:

    | call main
    jsr         main

    | reaching here is not expected.
0:
    reset
    bra         0b
