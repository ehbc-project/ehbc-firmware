.global         __init_arch
.global         __istack_end
.global         __stack_end
.global         exception_vector

.section        .text.startup

.global         _start
_start:
    bra         start

.text

start:
    | init memory
    lea         0xFF000008,%a0
    clr.b       %a0@(0)                 | clear SCU ABR
    clr.b       %a0@(1)
    clr.b       %a0@(2)
    clr.b       %a0@(3)
    clr.b       %a0@(4)
    clr.b       %a0@(5)
    clr.b       %a0@(6)
    clr.b       %a0@(7)

.Lsetup_bank:
    | check if it's running on an emulator
    | We've cleared all ABRs, so we cannot read valid data in a real machine
    movea.w     #0,%a1
    move.l      #0xAA55AA55,%d0
    move.l      #0x55AA55AA,%d1
    move.l      %d0,%a1@
    move.l      %d1,%a1@(4)
    cmp.l       %a1@,%d0
    bne         .Lprobe_bank
    cmp.l       %a1@(4),%d1
    bne         .Lprobe_bank
    bra         .Lend_setup_bank

.Lprobe_bank:
    | %d0, %d1: probe data (unchanged)
    | %d2: bank size for each byte (even)
    | %d3: bank size for each byte (odd)
    | %d4: probe address (temp)
    | %d5: abr index
    | %a0: ABR base address (unchanged)
    | %a1: probe address
    clr.l       %d2
    clr.l       %d5

0:
    move.b      #0xFF,%a0@(%d5)         | set value to max
    move.l      #0x8000,%a1             | start from 512kiB

1:
    move.l      %d0,%a1@(-8)            | test memory
    move.l      %d1,%a1@(-4)
    cmp.l       %a1@(-8),%d0
    bne         1f
    cmp.l       %a1@(-4),%d1
    bne         1f
                                        | if test success
    move.l      %a1,%d4                 | x2 test address
    lsl.l       #1,%d4
    movea.l     %d4,%a1
    bra         1b                      | test next address

1:                                      | if test failed
    move.l      %a1,%d4                 | get bank size
    lsr.l       #8,%d4                  | shift 20 bits to right
    lsr.l       #8,%d4
    lsr.l       #4,%d4
    andi.l      #0xFF,%d4
    btst        #0,%d5                  | test if even bank
    beq         1f
    lsl.l       #8,%d3                  | if bank odd
    or.l        %d4,%d3                 | store bank size to %d3
    bra         2f
1:
    lsl.l       #8,%d2                  | if bank even
    or.l        %d4,%d2                 | store bank size to %d2
2:
    clr.b       %a0@(%d5)               | clear abr
    addq.l      #1,%d5                  | set next abr to test
    cmpi.b      #8,%d5                  | check if we should break loop
    blt         0b                      | loop if not

.Lcheck_memory_present:
    tst.l       %d2                     | check if all banks are not present
    bne         .Lsetup_bank_register
    tst.l       %d3
    bne         .Lsetup_bank_register

0:  | no memory modules are present. hang
    bra         0b

.Lsetup_bank_register:
    | %d0: abr data
    | %d1: bank size (temp)
    | %d2: bank size for each byte (even)
    | %d3: bank size for each byte (odd)
    | %d4: abr index
    | %a0: ABR base address
    clr.l       %d0
    clr.l       %d4

0:
    btst        #0,%d4                  | test if even bank
    beq         1f
    rol.l       #8,%d3                  | get next bank size from %d3 if odd
    move.b      %d3,%d1
    bra         2f
1:
    rol.l       #8,%d2                  | get next bank size from %d2 if even
    move.b      %d2,%d1
2:
    add.b       %d1,%d0                 | accumulate bank size
    move.b      %d0,%a0@(%d4)           | store to abr
    addq.l      #1,%d4                  | set next abr to store
    cmpi.l      #8,%d4                  | check if we should break loop
    blt         0b                      | loop if not

.Lend_setup_bank:

    lea         __istack_end,%sp        | set interrupt stack pointer

    ori.w       #0x1000,%sr             | switch to master state

    lea         __stack_end,%sp         | set master stack pointer
    movea.l     %sp,%fp                 | set frame pointer

    bset.b      #1,0xFF000000           | disable initial vector

    | init exception vector
    move.l      #exception_vector,%d0
    movec.l     %d0,%vbr

    | Jump to __init_arch
    bra         __init_arch
