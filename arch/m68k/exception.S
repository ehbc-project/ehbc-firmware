.global         system_dump
.global         syscall_handler

.text

.global         error_handler
error_handler:
    | dump registers
    movem.l     %d0-%d7/%a0-%a7,%sp@-

    | get stack pointer
    move.w      %sp@(64),%d0
    btst        #13,%d0                 | test user / supervisor
    beq         .Lsp_usp
    btst        #12,%d0                 | test master / interrupt
    beq         .Lsp_skip
    bra         .Lsp_msp
.Lsp_usp:
    move.l      %usp,%a0
    bra         .Lsp_end
.Lsp_msp:
    movec.l     %msp,%a0
    bra         .Lsp_end
.Lsp_end:
    move.l      %a0,%sp@(60)            | store to A7
.Lsp_skip:

    subq.l      #4,%sp

    | dump system status
    bra         system_dump

.global         trap0_handler
trap0_handler:
    link.w      %fp,#0

    movem.l     %d0-%d4,%sp@-
    jsr         syscall_handler

    adda.l      #20,%sp
    unlk        %fp
    rte
