
.equ ISAIO_BASE,    0xFE000000

.text

.global         io_read_8
io_read_8:
    move.l      #ISAIO_BASE,%d0
    or.w        %sp@(6),%d0
    movea.l     %d0,%a0
    move.b      %a0@,%d0
    rts

.global         io_read_16
io_read_16:
    move.l      #ISAIO_BASE,%d0
    or.w        %sp@(6),%d0
    movea.l     %d0,%a0
    move.w      %a0@,%d0
    rts

.global         io_write_8
io_write_8:
    move.l      #ISAIO_BASE,%d0
    or.w        %sp@(6),%d0
    movea.l     %d0,%a0
    move.b      %sp@(11),%a0@
    rts

.global         io_write_16
io_write_16:
    move.l      #ISAIO_BASE,%d0
    or.w        %sp@(6),%d0
    movea.l     %d0,%a0
    move.w      %sp@(10),%a0@
    rts
