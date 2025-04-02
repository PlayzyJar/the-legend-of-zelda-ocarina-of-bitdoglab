#include "inc/timers_callback.h"

// Variáveis globais para controle da nota
volatile NoteState current_note_state = NOTE_OFF;
volatile int current_note_index = OCARINA_NONE;

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
                if (current_note_state == NOTE_OFF)
                {
                    // Tocar nova nota
                    JoystickState state;
                    joystick_update(&state);

                    switch (state.direction)
                    {
                    case UP:
                        current_note_index = (state.y_level == HIGH) ? OCARINA_DO_HIGH : OCARINA_DO;
                        break;
                    case UP_RIGHT:
                        current_note_index = (state.y_level == HIGH) ? OCARINA_RE_HIGH : OCARINA_RE;
                        break;
                    case RIGHT:
                        current_note_index = (state.x_level == HIGH) ? OCARINA_MI_HIGH : OCARINA_MI;
                        break;
                    case DOWN_RIGHT:
                        current_note_index = (state.x_level == HIGH) ? OCARINA_FA_HIGH : OCARINA_FA;
                        break;
                    case DOWN_LEFT:
                        current_note_index = (state.x_level == HIGH) ? OCARINA_SOL_HIGH : OCARINA_SOL;
                        break;
                    case LEFT:
                        current_note_index = (state.x_level == HIGH) ? OCARINA_LA_HIGH : OCARINA_LA;
                        break;
                    case UP_LEFT:
                        current_note_index = (state.y_level == HIGH) ? OCARINA_SI_HIGH : OCARINA_SI;
                        break;
                    default:
                        current_note_index = OCARINA_NONE;
                    }

                    if (current_note_index != OCARINA_NONE)
                    {
                        play_ocarina_note(current_note_index);
                        current_note_state = NOTE_PLAYING;
                        printf("Nota iniciada: %d\n", current_note_index);
                    }

                    if (current_note_index != OCARINA_NONE) {
                        check_note_sequence(current_note_index);  // <<< AQUI!
                    }
                }
                else
                {
                    // Parar nota atual
                    stop_ocarina_note();
                    current_note_state = NOTE_OFF;
                    printf("Nota parada\n");
                }
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
