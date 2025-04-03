/**
 * @file timers_callback.c
 * @brief Implementation of GPIO interrupt handlers
 */

 #include "inc/timers_callback.h"

 /** Current note playback status */
 volatile NoteState current_note_state = NOTE_OFF;
 
 /** Currently active note index */
 volatile int current_note_index = OCARINA_NONE;
 
 /** Global game state machine */
 volatile GameState game_state = STATE_IDLE;
 
 /**
  * @brief GPIO interrupt callback handler
  * 
  * Handles button presses with debouncing and manages:
  * - Play/pause functionality (Button A)
  * - Note input and validation (Button B)
  * - Boot mode entry (Joystick button)
  * 
  * @param gpio GPIO pin that triggered interrupt
  * @param events Event type that triggered interrupt
  */
 void btn_callback(uint gpio, uint32_t events) {
     uint32_t current_time = to_ms_since_boot(get_absolute_time());
 
     switch (gpio) {
         case BTN_A: // Play/Pause control
             if (current_time - last_interruption_a > DEBOUNCE_TIME_MS) {
                 last_interruption_a = current_time;
                 if (events & GPIO_IRQ_EDGE_FALL) {
                     printf("Button A pressed\n");
 
                     if (music_state == MUSIC_PLAYING) {
                         music_state = MUSIC_PAUSED;
                         should_stop_music = true;
                         printf("Music paused\n");
                         pwm_set_gpio_level(BUZZER_PIN, 0);
                     } else {
                         music_state = MUSIC_PLAYING;
                         printf("Music resumed\n");
                     }
                 }
             }
             break;
 
         case BTN_B: // Note input control
             if (current_time - last_interruption_b > DEBOUNCE_TIME_MS) {
                 last_interruption_b = current_time;
                 if (events & GPIO_IRQ_EDGE_FALL) {
                     if (current_note_state == NOTE_OFF) {
                         // Start new note based on joystick position
                         JoystickState state;
                         joystick_update(&state);
 
                         // Map joystick position to note
                         switch (state.direction) {
                             case UP:      current_note_index = (state.y_level == HIGH) ? OCARINA_DO_HIGH : OCARINA_DO; break;
                             case UP_RIGHT: current_note_index = (state.y_level == HIGH) ? OCARINA_RE_HIGH : OCARINA_RE; break;
                             case RIGHT:    current_note_index = (state.x_level == HIGH) ? OCARINA_MI_HIGH : OCARINA_MI; break;
                             case DOWN_RIGHT: current_note_index = (state.x_level == HIGH) ? OCARINA_FA_HIGH : OCARINA_FA; break;
                             case DOWN_LEFT: current_note_index = (state.x_level == HIGH) ? OCARINA_SOL_HIGH : OCARINA_SOL; break;
                             case LEFT:    current_note_index = (state.x_level == HIGH) ? OCARINA_LA_HIGH : OCARINA_LA; break;
                             case UP_LEFT: current_note_index = (state.y_level == HIGH) ? OCARINA_SI_HIGH : OCARINA_SI; break;
                             default:      current_note_index = OCARINA_NONE;
                         }
 
                         if (current_note_index != OCARINA_NONE) {
                             play_ocarina_note(current_note_index);
                             current_note_state = NOTE_PLAYING;
                             game_state = STATE_LISTENING;
                             check_note_sequence(current_note_index);
                             printf("Note started: %d\n", current_note_index);
                         }
                     } else {
                         // Stop current note
                         stop_ocarina_note();
                         current_note_state = NOTE_OFF;
                         printf("Note stopped\n");
                     }
                 }
             }
             break;
 
         case JSTICK: // Boot mode entry
             if (current_time - last_interruption_jstck > DEBOUNCE_TIME_MS) {
                 last_interruption_jstck = current_time;
                 if (events & GPIO_IRQ_EDGE_FALL) {
                     printf("Joystick button pressed - entering boot mode\n");
                     reset_usb_boot(0, 0);
                 }
             }
             break;
 
         default:
             break;
     }
 }
 
 /**
  * @brief Initialize GPIO pin with interrupt handler
  * 
  * Configures pin as input with pull-up and sets up falling edge interrupt
  * 
  * @param pin GPIO pin number to initialize
  */
 void gpio_pin_init(uint8_t pin) {
     gpio_init(pin);
     gpio_set_dir(pin, GPIO_IN);
     gpio_pull_up(pin);
     gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
 }