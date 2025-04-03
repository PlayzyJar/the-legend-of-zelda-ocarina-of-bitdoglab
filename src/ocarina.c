/**
 * @file ocarina.c
 * @brief Implementation of ocarina music functions
 */

#include "inc/ocarina.h"

/**
 * @brief Initialize PWM hardware for buzzer
 * 
 * Configures GPIO pin for PWM output and sets default parameters
 * 
 * @param pin Buzzer GPIO pin
 * @param slice_num PWM slice number
 */
void pwm_init_buzzer(uint pin, uint slice_num) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    pwm_config config = pwm_get_default_config();
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0);
}

/**
 * @brief Play Song of Storms melody
 * 
 * Plays predefined note sequence with proper timing
 */
void play_song_of_storms() {
    uint8_t speed = 1;
    
    for (int count = 0; count < 43; count++) {
        if (should_stop_music) {
            should_stop_music = false;
            break;
        }
        
        pwm_set_gpio_level(BUZZER_PIN, VOLUME_BUZZER);
        change_note(song_of_storms[count]);
        sleep_us(times_on_sofs[count]);
        
        pwm_set_gpio_level(BUZZER_PIN, 0);
        sleep_us(times_off_sofs[count] / speed);
    }
}

void play_secret_tune() {
    uint8_t speed = 1; // Velocidade padrão (ajustável conforme necessário)
    
    for (int count = 0; count < 8; count++) {
        if (should_stop_music) {
            should_stop_music = false;
            break;
        }
        
        pwm_set_gpio_level(BUZZER_PIN, VOLUME_BUZZER);
        change_note(secret_tune[count]);
        sleep_us(times_on_st[count]);
        
        pwm_set_gpio_level(BUZZER_PIN, 0);
        sleep_us(times_off_st[count] / speed);
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

/** Correct sequence for Song of Storms */
const int song_of_storms_sequence[SEQUENCE_LENGTH] = {
    OCARINA_RE, OCARINA_FA, OCARINA_RE_HIGH, 
    OCARINA_RE, OCARINA_FA, OCARINA_RE_HIGH
};

/** Global player note buffer instance */
NoteBuffer player_buffer = {
    .buffer = {OCARINA_NONE}, 
    .current_index = 0,
    .is_sequence_correct = false
};

/**
 * @brief Reset player's note sequence buffer
 */
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

    if (note == song_of_storms_sequence[player_buffer.current_index]) {
        player_buffer.buffer[player_buffer.current_index] = note;
        player_buffer.current_index++;
        
        if (player_buffer.current_index == SEQUENCE_LENGTH) {
            player_buffer.is_sequence_correct = true;
            game_state = STATE_VALIDATING; // Muda para estado de validação
        }
    } else {
        reset_player_buffer();
    }
}