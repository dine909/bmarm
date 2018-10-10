#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "board.h"

void setUSBHostPower(int on)
{
    GPIO_PinWrite(GPIOE, 6U, on&1);
}

void init_board_usb_host_power(int on){

    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);
    PORT_SetPinMux(PORTE, 6U, kPORT_MuxAsGpio);

    gpio_pin_config_t usb_pwr_config = {
        kGPIO_DigitalOutput, 0,
    };
    GPIO_PinInit(GPIOE, 6U, &usb_pwr_config);
    setUSBHostPower(on);

}

