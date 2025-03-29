#include "inc/timers_callback.h"

void btn_callback(uint gpio, uint32_t events)
{
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    switch (gpio)
    {
    case BTN_A:
        if (current_time - last_interruption_a > DEBOUNCE_TIME)
        {
            last_interruption_a = current_time;
            if (events & GPIO_IRQ_EDGE_FALL)
            {
                printf("Botão A pressionado\n");

                // Alterna entre pausado e tocando
                if (music_state == MUSIC_PLAYING)
                {
                    music_state = MUSIC_PAUSED;
                    should_stop_music = true; // Sinaliza para pausar
                    printf("Música pausada\n");
                    pwm_set_gpio_level(BUZZER_PIN, 0);
                }
                else
                {
                    music_state = MUSIC_PLAYING;
                    printf("Música retomada\n");
                }
            }
        }
        break;

    case BTN_B:
        if (current_time - last_interruption_b > DEBOUNCE_TIME)
        {
            last_interruption_b = current_time;
            if (events & GPIO_IRQ_EDGE_FALL)
            {
                printf("Botão B pressionado\n");
                reset_usb_boot(0, 0); // enter in bootsel mode
            }
        }
        break;

    default:
        break;
    }
}

// init
void gpio_pin_init(uint8_t pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
    gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
}