#ifndef OCARINA_H
#define OCARINA_H

#include "define_pins.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "math.h"

#include "inc/songs/song_of_storms.h"
#include "inc/timers_callback.h"

// Configuração da frequência do buzzer (em Hz)
#define top 65535

// função que muda as notas
#define change_note(note) pwm_set_clkdiv(slice_num, clock_get_hz(clk_sys) / (notas[note] * top))

// configuracoes
#define VOLUME_BUZZER 1024

// Notas da Ocarina (índices do array 'notas[]')
typedef enum {
    // Notas base (primeira oitava)
    OCARINA_DO      = 0,   // DO4  (261.63 Hz)
    OCARINA_RE      = 2,   // RE4  (293.66 Hz)
    OCARINA_MI      = 4,   // MI4  (329.63 Hz)
    OCARINA_FA      = 5,   // FA4  (349.23 Hz)
    OCARINA_SOL     = 7,   // SOL4 (392.00 Hz)
    OCARINA_LA      = 9,   // LA4  (440.00 Hz)
    OCARINA_SI      = 11,  // SI4  (493.88 Hz)
    
    // Oitavas acima (segunda oitava)
    OCARINA_DO_HIGH  = 12, // DO5  (523.25 Hz)
    OCARINA_RE_HIGH  = 14, // RE5  (587.33 Hz)
    OCARINA_MI_HIGH  = 16, // MI5  (659.26 Hz)
    OCARINA_FA_HIGH  = 17, // FA5  (698.46 Hz)
    OCARINA_SOL_HIGH = 19, // SOL5 (783.99 Hz)
    OCARINA_LA_HIGH  = 21, // LA5  (880.00 Hz)
    OCARINA_SI_HIGH  = 23, // SI5  (987.77 Hz)
    
    OCARINA_NONE     = -1  // Silêncio
} OcarinaNotes;

static uint slice_num;


// Definição de uma função para inicializar o PWM no pino do buzzer
void pwm_init_buzzer(uint pin, uint slice_num);

void play_song_of_storms(void);

void play_ocarina_note(int note_index, uint32_t duration_us);

#endif