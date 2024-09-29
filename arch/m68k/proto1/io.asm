    ORG         0

ISAIO_BASE      = $FE000000

    SECTION     .text
io_read_8::
    MOVE.L      #ISAIO_BASE,D0
    OR.W        (6,SP),D0
    MOVEA.L     D0,A0
    MOVE.B      (A0),D0
    RTS

io_read_16::
    MOVE.L      #ISAIO_BASE,D0
    OR.W        (6,SP),D0
    MOVEA.L     D0,A0
    MOVE.W      (A0),D0
    RTS

io_write_8::
    MOVE.L      #ISAIO_BASE,D0
    OR.W        (6,SP),D0
    MOVEA.L     D0,A0
    MOVE.B      (11,SP),(A0)
    RTS

io_write_16::
    MOVE.L      #ISAIO_BASE,D0
    OR.W        (6,SP),D0
    MOVEA.L     D0,A0
    MOVE.W      (10,SP),(A0)
    RTS
