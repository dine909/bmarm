#ifndef LEDCONFIG_H
#define LEDCONFIG_H

#include "board.h"
#include "chip_led.h"

#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

const BOARD_LED leds[]={
    {GPIO_PORTF_BASE,RED_LED,0},
    {GPIO_PORTF_BASE,BLUE_LED,0},
    {GPIO_PORTF_BASE,GREEN_LED,0}
};

#define LED_COUNT (sizeof(leds)/sizeof(BOARD_LED))

const uint32_t ledCount(){
    return  LED_COUNT;
}

#endif // LEDCONFIG_H
