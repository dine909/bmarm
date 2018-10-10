#ifndef LED_H
#define LED_H
#include "stdint.h"
#include "led.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct  {
    int32_t port;
    int32_t pin;
    int32_t invert;
} BOARD_LED;

#ifdef __cplusplus
}
#endif

#endif // LED_H
