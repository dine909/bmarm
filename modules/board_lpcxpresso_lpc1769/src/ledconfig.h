#ifndef LEDCONFIG_H
#define LEDCONFIG_H
#include "chip_led.h"

const BOARD_LED leds[]={
    {0,22,0},
};

#define LED_COUNT (sizeof(leds)/sizeof(BOARD_LED))
const uint32_t ledCount(){
    return  LED_COUNT;
}

#endif // LEDCONFIG_H
