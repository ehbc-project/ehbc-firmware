    ORG         0

    PUBLIC      system_dump
    PUBLIC      syscall_handler

    SECTION     .text
error_handler::
    LINK.W      A6,#0

    ; dump registers
    MOVEM.L     D0-D7/A0-A5,-(SP)

    ; get stack pointer
    MOVE.W      (56,SP),D0
    BTST        #13,D0
    BEQ         .sp_usp
    BTST        #12,D0
    BEQ         .sp_isp
    DC.W        $4E7A, $8803            ; MOVEC MSP, A0
    BRA         .sp_end
.sp_isp:
    MOVEA.L     A6,A0
    BRA         .sp_end
.sp_usp:
    MOVE.L      USP,A0
.sp_end:
    MOVE.L      A0,-(SP)

    ; dump system status
    JSR         system_dump

    ; should not reach here
    UNLK        A6
    RTE

trap0_handler::
    LINK.W      A6,#0

    MOVEM.L     D0-D4,-(SP)
    JSR         syscall_handler

    ADDA.L      #20,SP
    UNLK        A6
    RTE
