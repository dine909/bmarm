#ifndef LEDCONFIG_H
#define LEDCONFIG_H
#include "chip_led.h"

const BOARD_LED leds[]={
    {1,18,1}, // green
    {0,13,1}, // green
    {1,13,0}, // blue
    {2,19,0}, // blue
};

#define LED_COUNT (sizeof(leds)/sizeof(BOARD_LED))
const uint32_t ledCount(){
    return  LED_COUNT;
}
#endif // LEDCONFIG_H
