#include "ledconfig.h"
#include "vendor_system.h"

void setLed(uint32_t ledid, uint32_t state)
{
    BOARD_LED led=leds[ledid];
    GPIOPinWrite(led.port,led.pin,(state^led.invert)?led.pin:0);
}

void initLeds()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    for(uint32_t i=0;i<LED_COUNT;i++)
    {
        BOARD_LED led=leds[i];
        GPIOPinTypeGPIOOutput(led.port, led.pin);
        setLed(i,0);
    }

}

