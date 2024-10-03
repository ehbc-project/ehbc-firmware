
do_system_call::
    MOVE.L      (8,SP),D1
    MOVE.L      (4,SP),D0
    TRAP        #0

    RTS
