.global         main
.global         mm_init
.global         _exit

.global         __bss_start
.global         __bss_end
.global         __init_array_start
.global         __init_array_end
.global         __stack_end
.global         __heap_start

.section        .text.startup

.global         _start
_start:
    lea         __stack_end,%sp         | set stack pointer
    movea.l     %sp,%fp                 | set frame pointer

    | initialize .bss section
    lea         __bss_start,%a0
    lea         __bss_end,%a1
.Lloop_bss:
    cmpa.l      %a1,%a0
    bge         .Lend_bss
    clr.l       %a0@+
    bra         .Lloop_bss
.Lend_bss:  

    | initialize mm
    move.l      #__heap_start,%sp@-
    move.l      #524288,%sp@-           | temp value: 512k
    jsr         mm_init
    addq.l      #8,%sp

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

    move.l      #0,%sp@-                | envp
    move.l      #0,%sp@-                | argv
    move.l      #1,%sp@-                | argc

    jsr         main

    adda.l      #12,%sp                 | cleanup stack

    move.b      %d0,%sp@-               | return value
    jsr         _exit                   | exit

0:                                      | should not reach here
    bra         0b
