    ORG         0

    PUBLIC      main

    SECTION     .text
__init_arch::
    ; initialize .bss section
    LEA         __bss_start,A0
    LEA         __bss_end,A1
.loop_bss:
    CMPA.L      A1,A0
    BGE         .end_bss
    CLR.L       (A0)+
    BRA         .loop_bss
.end_bss:  

    ; initialize .data section
    LEA         __data_init_start,A0
    LEA         __data_start,A1
    LEA         __data_init_end,A2
.loop_data:
    CMPA.L      A2,A0
    BGE         .end_data
    MOVE.L      (A1)+,(A0)+
    BRA         .loop_data
.end_data:

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

    ANDI.W      #$F1FF,SR               ; enable interrupt

    ; call main
    JSR         main

    ; reaching here is not expected.
.reset:
    RESET
    BRA         .reset
