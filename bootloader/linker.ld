ENTRY(boot)
OUTPUT_FORMAT("binary")
SECTIONS {
    . = 0x100000;
    .text :
    {
        *(.boot)
        *(.text)
    }
    .rodata :
    {
        *(.rodata)
    }
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(.bss)
    }
}