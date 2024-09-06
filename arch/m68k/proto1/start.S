.org 0

.global __init_arch
.global __stack_end

/* Feature Enable Register
Bit No. Default Description
0       0       Disable reset vector
1       0       Enable ISA bus controller
2       0       Enable DRAM controller
3       0       Enable DRAM refresh
*/
.equ            CHIPSET_FER,    0xFF00000

/* Power Management Register */
.equ            CHIPSET_PMR,    0xFF000001

/* Clock Config Register */
.equ            CHIPSET_CCR,    0xFF000002

/* Memory Controller Timing Register
Bit No. Default Description
0-1     00      Refresh time:   00 = 8ms
                                01 = 16ms
                                10 = 32ms
                                11 = 64ms
*/
.equ            CHISPET_MCTR,   0xFF000003

/* Memory Controller Config Register
Bit No. Default Description
0-2     000     DRAM column/row address signal mapping mode
3       x       Reserved
4-6     000     DRAM bank size  000: 512kiB
                                001: 1MiB
                                010: 2MiB
                                011: 4MiB
                                100: 8MiB
                                101: 16MiB
                                110: 32MiB
                                111: 64MiB
7       0       DRAM RAS mapping mode
*/
.equ            CHIPSET_MCCR,   0xFF000004

/* Memory Test Pattern */
.equ            MEMTEST_PATTERN,0xAA55AA55


.section    .text.startup
.global     _start
_start:
    /* Disable reset vector, and enable all features */
    move.b      #0x01,CHIPSET_FER

    movea.l     #__stack_end,%sp        /* Configure Stack Pointer */
    movea.l     %sp,%fp                 /* Configure Frame Pointer */

.Linit_memory:
    /* Disable reset vector, and enable all features */
    move.b      #0x01,CHIPSET_FER

    /* 
    Current Memory Configuration:
        Refresh time: 8ms
        Address mapping mode: 0
        Bank size: 512kiB
        RAS mapping mode: 0
    */
    /* Test DRAM Timing */

    /* Test DRAM bank layout */
    clr.l       %d0
    movea.l     %d0,%a0
    moveq.l     #8,%d0
0:
    move.l      %a0,%a0@
    cmpa.l      %a0@,%a0
    clr.l       %a0@
    adda.l      #2048,%a0                       /* common bank layout: A0-A10 */
    bne         1f
    ori.b       #1,%d1
1:
    ror.b       #1,%d1
    dbne        %d0,0b

    
    

    /* Test DRAM bank size */
    ori.b       #0x70,CHIPSET_MCCR              /* set bank size to 64MiB */
    clr.l       %d0
    movea.l     %d0,%a0
    moveq.l     #127,%d0
0:
    move.l      #MEMTEST_PATTERN,%a0@           /* test memory */
    cmpi.l      #MEMTEST_PATTERN,%a0@
    clr.l       %a0@
    adda.l      #524288,%a0                     /* add 512k */
    dbne        %d0,0b                          /* test next address */
                                                /* if not equal, break */

    /* Generate MCCR bank size field data */
    clr.l       %d0
    movea.l     %d0,%a0
    move.l      #0x100000,%d1
    moveq.l     #6,%d2

0:
    cmp.l       %a0,%d1
    asl.l       #1,%d1                          /* x2 every loop */
    blt         1f                              /* break if %a0 < %d1 */
    addq.l      #1,%d0
1:
    dblt        %d0,0b

    /* Store MCCR bank size field data */
    lsl.b       #4,%d0
    move.b      CHIPSET_MCCR,%d1
    andi.b      #0x8F,%d1
    or.b        %d0,%d1
    move.b      %d1,CHIPSET_MCCR
    
    /* Jump to __init_arch */
    bra         __init_arch
