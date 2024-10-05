OUTPUT_FORMAT(elf32-m68k)
OUTPUT_ARCH(m68k)

ENTRY(_start)

MEMORY
{
    RAM(rw)     : ORIGIN = 0x00000000, LENGTH = 4032M
    FLASH(rx)   : ORIGIN = 0xFD000000, LENGTH = 16M
    ISAIO(rw)   : ORIGIN = 0xFE000000, LENGTH = 64K
    ISAMEM(rw)  : ORIGIN = 0xFE001000, LENGTH = 16M - 64K
    MMIO(rw)    : ORIGIN = 0xFF000000, LENGTH = 16M
}

PROVIDE_HIDDEN(__stack_size = 16K);
PROVIDE_HIDDEN(__istack_size = 16K);

SECTIONS
{
    .stack ALIGN(16) :
    {
        PROVIDE_HIDDEN(__stack_start = .);
        . = __stack_size;
        PROVIDE_HIDDEN(__stack_end = .);
        PROVIDE_HIDDEN(__istack_start = .);
        . = __stack_size + __istack_size;
        PROVIDE_HIDDEN(__istack_end = .);
    } >RAM

    .bss ALIGN(4) :
    {
        PROVIDE_HIDDEN(__bss_start = .);
        *(.bss .bss.*)
        *(COMMON)
        PROVIDE_HIDDEN(__bss_end = .);
    } >RAM

    .text :
    {
        KEEP(*(.text.startup .text.startup.*))
        KEEP(*(.text .text.*))
    } >FLASH

    PROVIDE_HIDDEN(__data_start = LOADADDR(.data));
    .data :
    {
        PROVIDE_HIDDEN(__data_init_start = .);
        *(.data .data.*)
        PROVIDE_HIDDEN(__data_init_end = .);
    } >RAM AT >FLASH

    .init :
    {
        KEEP(*(SORT_NONE(.init)))
    } >FLASH

    .fini :
    {
        KEEP(*(SORT_NONE(.fini)))
    } >FLASH

    . = ALIGN(4);
    .init_array :
    {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP(*(.init_array .ctors))
        PROVIDE_HIDDEN(__init_array_end = .);
    } >FLASH

    . = ALIGN(4);
    .fini_array :
    {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP(*(.fini_array .dtors))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } >FLASH

    .ctors :
    {
        KEEP(*(SORT(.ctors.*)))
        KEEP(*(.ctors))
    } >FLASH

    .dtors :
    {
        KEEP(*(SORT(.dtors.*)))
        KEEP(*(.dtors))
    } >FLASH

    .rodata :
    {
        KEEP(*(.rodata .rodata.*))
    } >FLASH
}
