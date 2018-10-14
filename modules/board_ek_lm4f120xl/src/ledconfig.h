#ifndef LEDCONFIG_H
#define LEDCONFIG_H
#include "chip_led.h"
#include "inc/lm4f120h5qr.h"

BOARD_LED leds[]={
    SYSCTL_RCGC2_GPIOF,8u,1,
};

#define LED_COUNT (sizeof(leds)/sizeof(BOARD_LED))

const uint32_t ledCount(){
    return  LED_COUNT;
}

#endif // LEDCONFIG_H
