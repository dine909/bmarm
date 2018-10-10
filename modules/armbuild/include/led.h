#ifndef LED_H
#define LED_H
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

void initLeds();
void setLed(uint32_t ledid, uint32_t state);
#ifdef __cplusplus
}
#endif

#endif // LED_H
