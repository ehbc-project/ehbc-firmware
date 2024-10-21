    SECTION     .text
setjmp::
    MOVEA.L     (4,SP),A0               ; get jmp_buf address

    MOVEM.L     D1-D7/A0-A7,(A0)        ; save registers D1-D7 and A0-A7
    MOVE.L      #.return,(28,A0)        ; save return address to A0 of jmp_buf

    IF  (__VASM&$40)==$40               ; if fpu exists
    FMOVEM.X    FP0-FP7,(60,A0)         ; save registers FP0-FP7
    ENDIF

    MOVE.L      #0,D0                   ; returns 0 when not jumped
.return:                                ; returns other value when jumped
    RTS


longjmp::
    MOVEA.L     (4,SP),A0               ; get jmp_buf address
    MOVE.L      (8,SP),D0               ; get value

    MOVEM.L     (A0),D1-D7/A0-A7        ; load registers
    
    JMP         (A0)                    ; jump
