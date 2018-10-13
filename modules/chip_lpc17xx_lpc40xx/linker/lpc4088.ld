/* Linker script to configure memory regions. */
MEMORY
{
  FLASH (rx)    : ORIGIN = 0x00000000, LENGTH = 256k
  RAM   (rwx)   : ORIGIN = 0x20000000, LENGTH = 32k
}

