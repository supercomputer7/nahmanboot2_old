ENTRY(boot)
OUTPUT_FORMAT("binary")
SECTIONS {
    . = 0x1000;
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