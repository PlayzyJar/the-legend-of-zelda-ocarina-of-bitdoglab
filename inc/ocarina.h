#ifndef OCARINA_H
#define OCARINA_H

#include "define_pins.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "math.h"

#include "inc/songs/song_of_storms.h"

// Configuração da frequência do buzzer (em Hz)
#define top 65535

// função que muda as notas
#define change_note(note) pwm_set_clkdiv(slice_num, clock_get_hz(clk_sys) / (notas[note] * top))

// configuracoes
#define VOLUME_BUZZER 4096


static uint slice_num;


// Definição de uma função para inicializar o PWM no pino do buzzer
void pwm_init_buzzer(uint pin, uint slice_num);

#endif