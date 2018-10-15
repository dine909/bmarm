#ifndef BOARD_H
#define BOARD_H
#include "stdint.h"

#include "inc/lm4f120h5qr.h"

#include "inc/hw_types.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"


#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

int SysTick_Config(uint32_t rate);
void SystemCoreClockUpdate();

extern uint32_t SystemCoreClock;

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif // BOARD_H
