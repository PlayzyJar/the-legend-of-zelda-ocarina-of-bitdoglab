#ifndef TIMERS_CALLBACK_H
#define TIMERS_CALLBACK_H

#include "define_pins.h"

#include <stdbool.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/gpio.h"
#include <stdio.h>

#define DEBOUNCE_TIME 250 // debounce for buttons

void btn_callback(uint gpio, uint32_t events);
void gpio_pin_init(uint8_t pin);

#endif