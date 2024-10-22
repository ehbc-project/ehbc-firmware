.text

.global         setjmp
setjmp:
    movea.l     %sp@(4),%a0             | get jmp_buf address

    movem.l     %d1-%d7/%a0-%a7,%a0@    | save registers D1-D7 and A0-A7
    move.l      #.Lreturn,%a0@(28)      | save return address to A0 of jmp_buf

                                        | assume that there is a fpu
                                        | I couldn't find any information to
                                        | determine whether a fpu exists in gas
    fmovem.x    %fp0-%fp7,%a0@(60)      | save registers FP0-FP7

    move.l      #0,%d0                  | returns 0 when not jumped
.Lreturn:                               | returns other value when jumped
    rts

.global         longjmp
longjmp:
    movea.l     %sp@(4),%a0             | get jmp_buf address
    move.l      %sp@(8),%d0             | get value

    MOVEM.L     %a0@,%d1-%d7/%a0-%a7    | load registers
    
    JMP         %a0@                    | jump
