KINETIS_FLASH_START = 0x410;

MEMORY
{
  m_interrupts          (RX)  : ORIGIN = 0x00000000, LENGTH = 0x00000400
  m_flash_config        (RX)  : ORIGIN = 0x00000400, LENGTH = 0x00000010
}


SECTIONS
{
    .interrupts :
    {
      __VECTOR_TABLE = .;
      . = ALIGN(4);
      KEEP(*(.isr_vector))     /* Startup code */
      . = ALIGN(4);
    } > m_interrupts

    .flash_config :
    {
      . = ALIGN(4);
      KEEP(*(.FlashConfig))    /* Flash Configuration Field (FCF) */
      . = ALIGN(4);
    } > m_flash_config

    __NDATA_ROM = __etext + (__data_end__ - __data_start__);
    .ncache.init : AT(__NDATA_ROM)
    {
      __noncachedata_start__ = .;   /* create a global symbol at ncache data start */
      *(NonCacheable.init)
      . = ALIGN(4);
      __noncachedata_init_end__ = .;   /* create a global symbol at initialized ncache data end */
    } > RAM

    . = __noncachedata_init_end__;
    .ncache :
    {
      *(NonCacheable)
      . = ALIGN(4);
      __noncachedata_end__ = .;     /* define a global symbol at ncache data end */
    } > RAM

    __DATA_END = __NDATA_ROM + (__noncachedata_init_end__ - __noncachedata_start__);
    text_end = ORIGIN(FLASH) + LENGTH(FLASH);
    ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")


}
