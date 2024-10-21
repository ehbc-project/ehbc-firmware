    ORG         0

    PUBLIC      system_dump
    PUBLIC      syscall_handler

    SECTION     .text
error_handler::
    ; dump registers
    MOVEM.L     D0-D7/A0-A7,-(SP)

    ; get stack pointer
    MOVE.W      (64,SP),D0
    BTST        #13,D0                  ; test user / supervisor
    BEQ         .sp_usp
    BTST        #12,D0                  ; test master / interrupt
    BEQ         .sp_skip
    BRA         .sp_msp
.sp_usp:
    MOVE.L      USP,A0
    BRA         .sp_end
.sp_msp:
    DC.W        $4E7A, $8803            ; MOVEC MSP, A0
    BRA         .sp_end
.sp_end:
    MOVE.L      A0,(60,SP)              ; store to A7
.sp_skip:

    SUBQ.L      #4,SP

    ; dump system status
    BRA         system_dump


trap0_handler::
    LINK.W      A6,#0

    MOVEM.L     D0-D4,-(SP)
    JSR         syscall_handler

    ADDA.L      #20,SP
    UNLK        A6
    RTE
