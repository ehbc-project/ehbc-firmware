OUTPUT_FORMAT(elf32-m68k)
OUTPUT_ARCH(m68k)

ENTRY(_start)

MEMORY
{
    RAM(rwx)    : ORIGIN = 0x00010000, LENGTH = 1M - 64K
}

PROVIDE_HIDDEN(__stack_size = 16K);

SECTIONS
{
    .stack 0x10000 :
    {
        PROVIDE_HIDDEN(__stack_start = .);
        . = __stack_size;
        PROVIDE_HIDDEN(__stack_end = .);
    } >RAM

    .bss ALIGN(4) :
    {
        PROVIDE_HIDDEN(__bss_start = .);
        *(.bss .bss.*)
        *(COMMON)
        PROVIDE_HIDDEN(__bss_end = .);
    } >RAM

    .text 0x20000 :
    {
        KEEP(*(.text.startup .text.startup.*))
        KEEP(*(.text .text.*))
    } >RAM

    PROVIDE_HIDDEN(__data_start = LOADADDR(.data));
    .data :
    {
        PROVIDE_HIDDEN(__data_init_start = .);
        *(.data .data.*)
        PROVIDE_HIDDEN(__data_init_end = .);
    } >RAM

    .init :
    {
        KEEP(*(SORT_NONE(.init)))
    }

    .fini :
    {
        KEEP(*(SORT_NONE(.fini)))
    }

    . = ALIGN(4);
    .init_array :
    {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP(*(.init_array .ctors))
        PROVIDE_HIDDEN(__init_array_end = .);
    } >RAM

    . = ALIGN(4);
    .fini_array :
    {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP(*(.fini_array .dtors))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } >RAM

    .ctors :
    {
        KEEP(*(SORT(.ctors.*)))
        KEEP(*(.ctors))
    }

    .dtors :
    {
        KEEP(*(SORT(.dtors.*)))
        KEEP(*(.dtors))
    }

    .rodata :
    {
        KEEP(*(.rodata .rodata.*))
    }
}
