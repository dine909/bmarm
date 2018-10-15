#ifndef VENDOR_SYSTEM
#define VENDOR_SYSTEM
#include "inc/lm4f120h5qr.h"
#include "driverlib/systick.h"
#include "stdint.h"

#include "board.h"

#include "inc/hw_types.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "inc/hw_ints.h"

void SystemCoreClockUpdate(){
    //
    // Set the clocking to run directly from the crystal.
    //
//    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
//                       SYSCTL_XTAL_16MHZ);

    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                    SYSCTL_OSC_MAIN);

    //
    //
    // Enable the peripherals used by this example.
    //
}
int SysTick_Config(uint32_t rate){
//    ROM_IntEnable(INT_GPIOA);

    ROM_IntMasterEnable();

    ROM_SysTickPeriodSet(ROM_SysCtlClockGet()/(SystemCoreClock/rate));
    ROM_SysTickIntEnable();
    ROM_SysTickEnable();
//    SysTickPeriodSet(rate);
//    SysTickEnable();

    return 0;
}

void SystemInit(){

}

#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif


#endif
