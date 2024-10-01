    ORG         0

    PUBLIC      handle_error

    SECTION     .text
bus_error:: 
address_error::
other_exception::
    ; set frame pointer
    MOVEA.L     SP,A6

    ; dump registers
    MOVE.L      A5,-(SP)
    MOVE.L      A4,-(SP)
    MOVE.L      A3,-(SP)
    MOVE.L      A2,-(SP)
    MOVE.L      A1,-(SP)
    MOVE.L      A0,-(SP)
    MOVE.L      D7,-(SP)
    MOVE.L      D6,-(SP)
    MOVE.L      D5,-(SP)
    MOVE.L      D4,-(SP)
    MOVE.L      D3,-(SP)
    MOVE.L      D2,-(SP)
    MOVE.L      D1,-(SP)
    MOVE.L      D0,-(SP)

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

    ; jump to error handler
    JSR         handle_error

    RTS
