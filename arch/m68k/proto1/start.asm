    ORG         0

    PUBLIC      __init_arch
    PUBLIC      __istack_end
    PUBLIC      __stack_end
    PUBLIC      exception_vector

    SECTION     .text.startup
_start::
    BRA         start

    SECTION     .text
start:
    LEA         __istack_end,SP         ; interrupt stack pointer

    ORI.W       #$1000,SR               ; switch to master state

    LEA         __stack_end,SP          ; master stack pointer
    MOVEA.L     SP,A6                   ; frame pointer

    BSET.B      #1,($FF000000)          ; disable initial vector
    BCLR.B      #7,($FF000000)          ; set clock to 33 MHz

    ANDI.W      #$F1FF,SR               ; enable interrupt

    MOVE.B      #$09,($FF000010)        ; irq0=disabled     irq1=level1
    MOVE.B      #$09,($FF000011)        ; irq2=disabled     irq3=level1
    MOVE.B      #$99,($FF000012)        ; irq4=level1       irq5=level1
    MOVE.B      #$99,($FF000013)        ; irq6=level1       irq7=level1
    MOVE.B      #$99,($FF000014)        ; irq8=level1       irq9=level1
    MOVE.B      #$99,($FF000015)        ; irq10=level1      irq11=level1
    MOVE.B      #$90,($FF000016)        ; irq12=level1      irq13=disabled
    MOVE.B      #$99,($FF000017)        ; irq14=level1      irq15=level1
    MOVE.B      #$A9,($FF000018)        ; irq16=level2      irq17=level1
    MOVE.B      #$90,($FF000019)        ; irq18=level1      irq19=disabled
    MOVE.B      #$00,($FF00001A)        ; irq20=disabled    irq21=disabled
    MOVE.B      #$00,($FF00001B)        ; irq22=disabled    irq23=disabled

    ; init memory

    ; init exception vector
    MOVE.L      #exception_vector,D0
    MOVEC.L     D0,VBR

    ; Jump to __init_arch
    BRA         __init_arch

; Test bank
;   Registers:
;     D0: Bank index (0 to 7)
;   Return:
;     D0: Bank size
;   Modifies: D0, D1, A0, A1
test_bank:
    CLR.L       D1
    RTS

__helloworld_str:
    DC.B "Hello, World!\r\n", 0
