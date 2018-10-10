#ifndef LED_H
#define LED_H
#include "fsl_common.h"
#include "led.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct  {
    PORT_Type* port;
    GPIO_Type* gpio;
    int32_t pin;
} BOARD_LED;

#ifdef __cplusplus
}
#endif

#endif // LED_H
