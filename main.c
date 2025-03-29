#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"

#include "inc/song_of_storms.h"

// Configuração da frequência do buzzer (em Hz)
#define top 65535

// configuracoes
#define VOLUME_BUZZER 2048

// Mapeando hardware
#define BUZZER_PIN 21

#define change_note(note) pwm_set_clkdiv(slice_num, clock_get_hz(clk_sys) / (notas[note] * top))

// Definição de uma função para inicializar o PWM no pino do buzzer
void pwm_init_buzzer(uint pin, uint slice_num);

int main()
{
    stdio_init_all();

    // Inicializar o PWM no pino do buzzer
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_init_buzzer(BUZZER_PIN, slice_num);

    for (int count = 0; count < 23; count++) {
        pwm_set_gpio_level(BUZZER_PIN, VOLUME_BUZZER);
        change_note(musica[count]);
        sleep_us(times_on[count]);

        pwm_set_gpio_level(BUZZER_PIN, 0);
        sleep_us(times_off[count]);
    }

    while (true)
    {
    }
}

// Definição de uma função para inicializar o PWM no pino do buzzer
void pwm_init_buzzer(uint pin, uint slice_num)
{
    // Configurar o pino como saída de PWM
    gpio_set_function(pin, GPIO_FUNC_PWM);

    // Obter o slice do PWM associado ao pino

    // Configurar o PWM com frequência desejada
    pwm_config config = pwm_get_default_config();

    pwm_init(slice_num, &config, true);

    pwm_set_gpio_level(pin, 0);
}