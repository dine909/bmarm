#ifndef VENDOR_SYSTEM
#define VENDOR_SYSTEM
#include "inc/lm4f120h5qr.h"
#include "driverlib/systick.h"
#include "stdint.h"

void SystemCoreClockUpdate(){

}
int SysTick_Config(uint32_t rate){
    SysTickPeriodSet(rate);
    SysTickEnable();
    return 1;
}
#endif
