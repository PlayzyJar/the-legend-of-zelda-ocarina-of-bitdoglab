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

void play_ocarina_note(int note_index) {
    if (note_index == OCARINA_NONE) return;
    
    pwm_set_wrap(slice_num, 4095);
    pwm_set_gpio_level(BUZZER_PIN, VOLUME_BUZZER);
    change_note(note_index);
}

void stop_ocarina_note() {
    pwm_set_gpio_level(BUZZER_PIN, 0);
    current_note_state = NOTE_OFF;  // Atualiza o estado global
}

void change_note(int note) {
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_clkdiv(slice_num, clock_get_hz(clk_sys) / (notas[note] * 4096));

    // Adicionado -----
    pwm_set_wrap(slice_num, 4095);
    // --------

    pwm_set_gpio_level(BUZZER_PIN, VOLUME_BUZZER);
}

// Sequência correta (Song of Storms)
const int song_of_storms_sequence[SEQUENCE_LENGTH] = {
    OCARINA_RE, OCARINA_FA, OCARINA_RE_HIGH, 
    OCARINA_RE, OCARINA_FA, OCARINA_RE_HIGH
};

// Buffer do jogador
NoteBuffer player_buffer = {
    .buffer = {OCARINA_NONE}, 
    .current_index = 0,
    .is_sequence_correct = false
};

void reset_player_buffer() {
    for (int i = 0; i < SEQUENCE_LENGTH; i++) {
        player_buffer.buffer[i] = OCARINA_NONE;
    }
    player_buffer.current_index = 0;
    player_buffer.is_sequence_correct = false;
}

// Verifica se a nota atual está correta
void check_note_sequence(int note) {
    if (player_buffer.current_index >= SEQUENCE_LENGTH) {
        reset_player_buffer();
    }

    // Compara com a sequência de referência
    if (note == song_of_storms_sequence[player_buffer.current_index]) {
        player_buffer.buffer[player_buffer.current_index] = note;
        player_buffer.current_index++;
        
        // Sequência completa?
        if (player_buffer.current_index == SEQUENCE_LENGTH) {
            player_buffer.is_sequence_correct = true;
            sleep_us(50000);
            printf("sequencia correta!");
            play_song_of_storms();  // Toca a música completa!
            reset_player_buffer();  // Prepara para próxima tentativa
        }
    } else {
        reset_player_buffer();  // Nota errada, reseta tudo
    }
}