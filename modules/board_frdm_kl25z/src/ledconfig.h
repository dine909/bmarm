#ifndef LEDCONFIG_H
#define LEDCONFIG_H
#include "chip_led.h"

BOARD_LED leds[]={
    PORTB,GPIOB,18u,1,
    PORTB,GPIOB,19u,1,
    PORTD,GPIOD,1u,1
};

#define LED_COUNT (sizeof(leds)/sizeof(BOARD_LED))

const uint32_t ledCount(){
    return  LED_COUNT;
}

#endif // LEDCONFIG_H
