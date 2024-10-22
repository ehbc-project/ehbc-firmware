.text

.global         get_irqmask_level
get_irqmask_level:
    move.w      %sr,%d0
    andi.w      #0x0700,%d0
    lsr.w       #8,%d0
    rts

.global         set_irqmask_level
set_irqmask_level:
    move.b      %sp@(4),%d0
    andi.b      #7,%d0
    lsl.w       #8,%d0
    move.w      %sr,%d1
    andi.w      #0xF0FF,%d1
    or.w        %d0,%d1
    move.w      %d1,%sr
    rts
