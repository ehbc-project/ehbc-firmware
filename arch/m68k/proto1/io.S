.org 0

.equ            ISAIO_BASE,     0xFE000000

.section    .text
.global     io_read_8
io_read_8:
    link.w      %fp,#0
    move.w      %fp@(8),%d0
    movea.l     #ISAIO_BASE,%a0
    move.b      %a0@(%d0:w),%d0
    unlk        %fp
    rts

.global     io_read_16
io_read_16:
    link.w      %fp,#0
    move.w      %fp@(8),%d0
    movea.l     #ISAIO_BASE,%a0
    move.w      %a0@(%d0:w),%d0
    unlk        %fp
    rts

.global     io_write_8
io_write_8:
    link.w      %fp,#0
    move.w      %fp@(8),%d0
    movea.l     #ISAIO_BASE,%a0
    move.b      %fp@(12),%a0@(%d0:w)
    unlk        %fp
    rts

.global     io_write_16
io_write_16:
    link.w      %fp,#0
    move.w      %fp@(8),%d0
    movea.l     #ISAIO_BASE,%a0
    move.w      %fp@(12),%a0@(%d0:w)
    unlk        %fp
    rts
