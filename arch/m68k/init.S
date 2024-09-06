.org 0

.global         main

.text
.global         __init_arch
__init_arch:

.Linit_bss:
    /* initialize .bss section */
    movea.l     #__bss_start,%a0
    movea.l     #__bss_end,%a1
0:
    cmpa.l      %a1,%a0
    bge         1f
    clr.l       %a1@+
    bra         0b
1:  

.Linit_data:
    /* initialize .data section */
    movea.l     #__data_init_start,%a0
    movea.l     #__data_start,%a1
    movea.l     #__data_init_end,%a2
0:
    cmpa.l      %a2,%a0
    bge         1f
    move.l      %a1@+,%a0@+
    bra         0b
1:

.Lrun_ctors:
    /* run ctors */
    movea.l     #__init_array_start,%a2
    movea.l     #__init_array_end,%a3
0:
    cmpa.l      %a3,%a2
    bge         1f
    movea.l     %a2@+,%a4
    jsr         %a4@
1:

.Lcall_main:
    /* call main */
    jsr         main

    /* reset when main finished */
    reset
0:
    bra         0b
