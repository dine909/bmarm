#ifndef LEDCONFIG_H
#define LEDCONFIG_H
#include "chip_led.h"

BOARD_LED leds[]={
    PORTB,GPIOB,18u,1,
    PORTB,GPIOB,19u,1,
    PORTD,GPIOD,13u,1
};

#endif // LEDCONFIG_H
