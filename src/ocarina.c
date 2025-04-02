#include "inc/ocarina.h"

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

void play_song_of_storms() {
    uint8_t speed = 1; // Velocidade padrão (ajustável conforme necessário)
    
    for (int count = 0; count < 43; count++) {
        if (should_stop_music) {
            should_stop_music = false;
            break;
        }
        
        pwm_set_gpio_level(BUZZER_PIN, VOLUME_BUZZER);
        change_note(musica[count]);
        sleep_us(times_on[count]);
        
        pwm_set_gpio_level(BUZZER_PIN, 0);
        sleep_us(times_off[count] / speed);
    }
}

void play_ocarina_note(int note_index, uint32_t duration_us) {
    if (note_index == OCARINA_NONE) return;
    
    pwm_set_gpio_level(BUZZER_PIN, VOLUME_BUZZER);
    change_note(note_index);
    sleep_us(duration_us);
    pwm_set_gpio_level(BUZZER_PIN, 0);
}