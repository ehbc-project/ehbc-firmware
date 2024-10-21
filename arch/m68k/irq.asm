    SECTION     .text

get_irqmask_level::
    MOVE.W      SR,D0
    ANDI.W      #$0700,D0
    LSR.W       #8,D0
    RTS

set_irqmask_level::
    MOVE.B      (4,SP),D0
    ANDI.B      #7,D0
    LSL.W       #8,D0
    MOVE.W      SR,D1
    ANDI.W      #$F0FF,D1
    OR.W        D0,D1
    MOVE.W      D1,SR
    RTS
