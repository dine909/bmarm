/* Linker script to configure board memory regions. */
MEMORY
{
    SRAM  (rwx) : ORIGIN = 0xA0000000, LENGTH = 0x10000000
}

SECTIONS{
    .data_sram :
    {
        FILL(0xff)
        *(.datasram)
        *(.data.$SRAM*)
    } > SRAM
}
