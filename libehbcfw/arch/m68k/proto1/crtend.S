.global         __fini_array_start
.global         __fini_array_end
    
.text

.global         _exit
_exit:
    | run dtors
    lea         __fini_array_start,%a2
    lea         __fini_array_end,%a3
.Lloop_dtors:
    cmpa.l      %a3,%a2
    bge         .Lend_dtors
    movea.l     %a2@+,%a4
    jsr         %a4@
    bra         .Lloop_dtors
.Lend_dtors:

                                        | try to boot from next device
    clr.l       %d0                     | category 0, function 0
    move.b      %sp@(4),%d1             | return code
    trap        #0                      | syscall

0:                                      | should not reach here
    bra         0b
