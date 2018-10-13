/* Linker script to configure memory regions. */
MEMORY
{
  FLASH     (rx)    : ORIGIN = KINETIS_FLASH_START, LENGTH = 1m  -KINETIS_FLASH_START
  FLEXNVM   (rx)    : ORIGIN = 0x10000000,          LENGTH = 256k
  RAM       (rwx)   : ORIGIN = 0x1FFF0000,          LENGTH = 256k
}

SECTIONS
{
    .text_nvm :
    {
        FILL(0xff)
        *(.textnvm)
        *(.text.$m_nvm*)

    } > FLEXNVM

}
