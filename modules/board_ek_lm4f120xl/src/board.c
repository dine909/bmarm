#include "board.h"

uint32_t SystemCoreClock=0;

void SystemCoreClockUpdate(){
    SystemCoreClock=ROM_SysCtlClockGet();
}

int SysTick_Config(uint32_t rate){
    ROM_SysTickPeriodSet(SystemCoreClock/(SystemCoreClock/rate));
    ROM_SysTickIntEnable();
    ROM_SysTickEnable();

    return 0;
}

void SystemInit(){
    //
    // Setup the system clock to run at 80 Mhz from PLL with crystal reference
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                    SYSCTL_OSC_MAIN);

    ROM_IntMasterEnable();

}

#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif
