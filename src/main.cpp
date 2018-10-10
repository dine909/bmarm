
#include "vendor_system.h"
#include "led.h"


volatile uint32_t g_systickCounter=0;

extern "C" void SysTick_Handler()
{
    uint32_t tickpos=g_systickCounter%1000;
    if(tickpos==0 || tickpos==200){
        setLed(0,1);
    }else if(tickpos==100 || tickpos==300){
        setLed(0,0);
    }
    g_systickCounter++;
}

//__attribute__ ((section(".textnvm")))
int main()
{
    //    BOARD_InitPins();
    //    BOARD_BootClockRUN();
    SystemCoreClockUpdate();
    initLeds();

    if(SysTick_Config(SystemCoreClock / 1000U))
    {
        while(1)
        {
        }
    }

    return 0;
}
