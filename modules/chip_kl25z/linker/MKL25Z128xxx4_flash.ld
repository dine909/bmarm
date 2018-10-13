/* Linker script to configure memory regions. */
MEMORY
{
  FLASH (rx)    : ORIGIN = KINETIS_FLASH_START  , LENGTH = 128k - KINETIS_FLASH_START
  RAM   (rwx)   : ORIGIN = 0x1FFFF000           , LENGTH = 16k
}

