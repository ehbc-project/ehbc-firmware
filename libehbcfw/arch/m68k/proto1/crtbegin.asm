    PUBLIC      main
    PUBLIC      _exit

    PUBLIC      __bss_start
    PUBLIC      __bss_end
    PUBLIC      __init_array_start
    PUBLIC      __init_array_end
    PUBLIC      __stack_end

    SECTION     .text.startup
_start::
    LEA         __stack_end,SP          ; set stack pointer
    MOVEA.L     SP,A6                   ; set frame pointer

    ; initialize .bss section
    LEA         __bss_start,A0
    LEA         __bss_end,A1
.loop_bss:
    CMPA.L      A1,A0
    BGE         .end_bss
    CLR.L       (A0)+
    BRA         .loop_bss
.end_bss:  

    ; run ctors
    LEA         __init_array_start,A2
    LEA         __init_array_end,A3
.loop_ctors:
    CMPA.L      A3,A2
    BGE         .end_ctors
    MOVEA.L     (A2)+,A4
    JSR         (A4)
    BRA         .loop_ctors
.end_ctors:

    MOVE.L      #0,-(SP)                ; envp
    MOVE.L      #0,-(SP)                ; argv
    MOVE.L      #0,-(SP)                ; argc

    JSR         main

    ADD.L       #12,SP                  ; cleanup stack

    MOVE.B      D0,-(SP)                ; return value
    JSR         _exit                   ; exit

.loop:                                  ; should not reach here
    BRA         .loop
