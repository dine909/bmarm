#include "ledconfig.h"
#include "fsl_port.h"
#include "fsl_gpio.h"

void initLeds()
{
    for(uint32_t i=0;i<sizeof(leds)/sizeof (BOARD_LED);i++)
    {
        BOARD_LED led=leds[i];
        /* Port Clock Gate Control: Clock enabled */
        if(led.port==PORTA)
            CLOCK_EnableClock(kCLOCK_PortA);
        else if(led.port==PORTB)
            CLOCK_EnableClock(kCLOCK_PortB);
        else if(led.port==PORTC)
            CLOCK_EnableClock(kCLOCK_PortC);
        else if(led.port==PORTD)
            CLOCK_EnableClock(kCLOCK_PortD);
        else if(led.port==PORTE)
            CLOCK_EnableClock(kCLOCK_PortE);

        /* PORT is configured as GPIO */
        PORT_SetPinMux(led.port, led.pin, kPORT_MuxAsGpio);

        /* Init output LED GPIO. */
        gpio_pin_config_t led_config = {
            kGPIO_DigitalOutput, 0,
        };

        GPIO_PinInit(led.gpio, led.pin, &led_config);
    }

}


void setLed(uint32_t ledid, uint32_t state)
{
    BOARD_LED led=leds[ledid];
    GPIO_PinWrite(led.gpio, led.pin,state^led.invert );
}
