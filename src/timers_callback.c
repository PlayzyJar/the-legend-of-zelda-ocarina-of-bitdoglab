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
                JoystickState state;
                joystick_update(&state);

                int note_to_play = OCARINA_NONE;

                switch (state.direction)
                {
                case UP:
                    note_to_play = (state.y_level == HIGH) ? OCARINA_DO_HIGH : OCARINA_DO;
                    break;
                case DOWN:
                    note_to_play = (state.y_level == HIGH) ? OCARINA_RE + 12 : OCARINA_RE;
                    break;
                case LEFT:
                    note_to_play = (state.y_level == HIGH) ? OCARINA_MI + 12 : OCARINA_MI;
                    break;
                case RIGHT:
                    note_to_play = (state.y_level == HIGH) ? OCARINA_FA + 12 : OCARINA_FA;
                    break;
                case UP_LEFT:
                    note_to_play = (state.y_level == HIGH) ? OCARINA_SOL + 12 : OCARINA_SOL;
                    break;
                case UP_RIGHT:
                    note_to_play = (state.y_level == HIGH) ? OCARINA_LA + 12 : OCARINA_LA;
                    break;
                case DOWN_LEFT:
                    note_to_play = (state.y_level == HIGH) ? OCARINA_SI + 12 : OCARINA_SI;
                    break;
                default:
                    note_to_play = OCARINA_NONE; // CENTER ou direções não mapeadas
                }

                play_ocarina_note(note_to_play, 187500); // Toca por 187500µs
            }
        }
        break;

    case JSTICK:
        if (current_time - last_interruption_jstck > DEBOUNCE_TIME)
        {
            last_interruption_jstck = current_time;
            if (events & GPIO_IRQ_EDGE_FALL)
            {
                printf("Botão JSTICK pressionado\n");
                reset_usb_boot(0, 0); // enter em bootsel mode
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