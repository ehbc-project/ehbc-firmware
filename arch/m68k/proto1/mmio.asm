    ORG         0

MMIO_BASE      = $FF000000

    SECTION     .text
mmio_read_8::
    MOVE.L      #MMIO_BASE,D0
    OR.W        (6,SP),D0
    MOVEA.L     D0,A0
    MOVE.B      (A0),D0
    RTS

mmio_read_16::
    MOVE.L      #MMIO_BASE,D0
    OR.W        (6,SP),D0
    MOVEA.L     D0,A0
    MOVE.W      (A0),D0
    RTS

mmio_write_8::
    MOVE.L      #MMIO_BASE,D0
    OR.W        (6,SP),D0
    MOVEA.L     D0,A0
    MOVE.B      (11,SP),(A0)
    RTS

mmio_write_16::
    MOVE.L      #MMIO_BASE,D0
    OR.W        (6,SP),D0
    MOVEA.L     D0,A0
    MOVE.W      (10,SP),(A0)
    RTS
