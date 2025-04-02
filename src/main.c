#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"

#include "inc/ocarina.h"
#include "inc/define_pins.h"
#include "inc/timers_callback.h"
#include "inc/songs/song_of_storms.h"

#include "inc/joystick.h"
#include "inc/led_functions.h"

#include "inc/drivers/frames_3x5.h"
#include "inc/drivers/frames_5x5.h"
#include "inc/drivers/letras_3x5.h"
#include "inc/drivers/letras_5x5.h"

volatile MusicState music_state;
volatile bool should_stop_music;

int main()
{
    // PIO configuration variables
    PIO pio;
    uint sm, offset;

    RGBColor cor = {0, 50, 255};
    JoystickState js;
    uint8_t speed = 1; // regula a velocidade da música (1, 2, 4 ou 8)
    double intensity = 0.1;         // LED intensity
    uint8_t font_width = 5;         // Font size (5x5)

    stdio_init_all();

    gpio_pin_init(BTN_A);
    gpio_pin_init(BTN_B);
    gpio_pin_init(JSTICK);

    // Inicializar o PWM no pino do buzzer
    slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_init_buzzer(BUZZER_PIN, slice_num);

    if (!init_system(&pio, &sm, &offset))
    {
        return 1; // Exit if initialization fails
    }

    joystick_init();

    // play_song_of_storms();

    while (true)
    {
        joystick_update(&js);
        display_joystick_frame(js, cor, pio, sm, intensity);


        // if (music_state == MUSIC_PLAYING)
        // {
        //     for (int count = 0; count < 43; count++)
        //     {
        //         if (should_stop_music)
        //         {
        //             should_stop_music = false; 
        //             break;
        //         }

        //         pwm_set_gpio_level(BUZZER_PIN, VOLUME_BUZZER);
        //         change_note(musica[count]);
        //         sleep_us(times_on[count]);

        //         pwm_set_gpio_level(BUZZER_PIN, 0);
        //         sleep_us(times_off[count] / speed);
        //     }
        // }
        // else
        // {
        //     // Se estiver pausado, apenas espera um pouco para não travar o sistema
        //     sleep_ms(100);
        // }

        sleep_us(100000);
    }
}
