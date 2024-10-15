    PUBLIC      __fini_array_start
    PUBLIC      __fini_array_end
    
    SECTION     .text
_exit::
    ; run dtors
    LEA         __fini_array_start,A2
    LEA         __fini_array_end,A3
.loop_ctors:
    CMPA.L      A3,A2
    BGE         .end_ctors
    MOVEA.L     (A2)+,A4
    JSR         (A4)
    BRA         .loop_ctors
.end_ctors:

                                        ; try to boot from next device
    CLR.L       D0                      ; category 0, function 0
    MOVE.B      (4,SP),D1               ; return code
    TRAP        #0                      ; syscall

.loop:                                  ; should not reach here
    BRA         .loop
