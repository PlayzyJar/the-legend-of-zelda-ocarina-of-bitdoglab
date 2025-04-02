#ifndef TIMERS_CALLBACK_H
#define TIMERS_CALLBACK_H

#include "define_pins.h"
#include "inc/ocarina.h"
#include "inc/joystick.h"

#include <stdbool.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/gpio.h"
#include <stdio.h>

#define DEBOUNCE_TIME 250 // debounce for buttons

typedef enum {
    MUSIC_PLAYING,
    MUSIC_PAUSED
} MusicState;

static volatile uint32_t last_interruption_b = 0;
static volatile uint32_t last_interruption_a = 0;
static volatile uint32_t last_interruption_jstck = 0;

extern volatile MusicState music_state;
extern volatile bool should_stop_music;

static volatile uint8_t play_state = 1;

void btn_callback(uint gpio, uint32_t events);
void gpio_pin_init(uint8_t pin);

#endif