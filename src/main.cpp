#ifdef IS_BARE_METAL
#include "vendor_system.h"
#include "led.h"
#include "board.h"

volatile uint32_t g_systickCounter=0;


//#define HEARTBEAT_USE_ALL_LEDS
extern "C" void SysTick_Handler()
{
#ifdef HEARTBEAT_USE_ALL_LEDS
    for(uint32_t offset=0;offset<ledCount();offset++)
#else
#   define offset 0
#endif
    {
        uint32_t tickpos=(((1000/ledCount())*offset)+g_systickCounter)%1000;
        if(tickpos==0 || tickpos==200){
            setLed(offset,1);
        }else if(tickpos==100 || tickpos==300){
            setLed(offset,0);
        }
    }
    g_systickCounter++;
}

#endif
//__attribute__ ((section(".textnvm")))
int main()
{
#ifdef IS_BARE_METAL
    //    SystemInit();
    //    SystemSetupClocking();
    SystemCoreClockUpdate();
    initLeds();

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
