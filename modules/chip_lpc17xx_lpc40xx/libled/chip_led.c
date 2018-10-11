#include "ledconfig.h"
#include "vendor_system.h"

void setLed(uint32_t ledid, uint32_t state)
{
    BOARD_LED led=leds[ledid];
    Chip_GPIO_WritePortBit(LPC_GPIO,led.port,led.pin,state^led.invert);
}

void initLeds()
{
    for(uint32_t i=0;i<sizeof(leds)/sizeof (BOARD_LED);i++)
    {
        BOARD_LED led=leds[i];
       Chip_GPIO_WriteDirBit(LPC_GPIO,led.port,led.pin, true);
       setLed(i,0);
    }

}

