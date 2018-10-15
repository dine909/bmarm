#ifdef IS_BARE_METAL
#include "vendor_system.h"
#include "led.h"
#include "board.h"


//__attribute__ ((section(".sram")))
volatile uint32_t g_systickCounter=0;

extern "C" void SysTick_Handler()
{
    // "Heartbeat" LED flash pattern
    uint32_t tickpos=g_systickCounter%1000;
    if(tickpos==0 || tickpos==200){
        setLed(0,1);
    }else if(tickpos==100 || tickpos==300){
        setLed(0,0);
    }
    g_systickCounter++;
}

#endif

int main()
{
#ifdef IS_BARE_METAL
    SystemCoreClockUpdate();
    initLeds();

    //Systick set to 1ms
    if(SysTick_Config(SystemCoreClock / 1000U))
    {
        while(1)
        {
        }
    }
#endif

    while(1){

    }

    return 0;
}
