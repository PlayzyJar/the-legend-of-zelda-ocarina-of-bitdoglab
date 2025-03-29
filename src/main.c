#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"

#include "inc/ocarina.h"
#include "inc/define_pins.h"
#include "inc/timers_callback.h"

int main()
{
    stdio_init_all();

    gpio_pin_init(BTN_A);
    gpio_pin_init(BTN_B);

    // Inicializar o PWM no pino do buzzer
    slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_init_buzzer(BUZZER_PIN, slice_num);

    for (int count = 0; count < 23; count++)
    {
        pwm_set_gpio_level(BUZZER_PIN, VOLUME_BUZZER);
        change_note(musica[count]);
        sleep_us(times_on[count]);

        pwm_set_gpio_level(BUZZER_PIN, 0);
        sleep_us(times_off[count]);
    }

    while (true)
    {
        for (int count = 0; count < 23; count++)
        {
            pwm_set_gpio_level(BUZZER_PIN, VOLUME_BUZZER);
            change_note(musica[count]);
            sleep_us(times_on[count]);

            pwm_set_gpio_level(BUZZER_PIN, 0);
            sleep_us(times_off[count]);
        }

        sleep_us(500000);
    }
}
