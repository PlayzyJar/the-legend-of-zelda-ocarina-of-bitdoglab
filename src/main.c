/**
 * @file main.c
 * @brief Zelda Ocarina Controller Main Application
 * 
 * Implements the state machine for ocarina note detection and song playback
 * using Raspberry Pi Pico hardware.
 */

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
 
 /** Global music playback state */
 volatile MusicState music_state;
 
 /** Flag to stop current music playback */
 volatile bool should_stop_music;
 
 /**
  * @brief Main application entry point
  * 
  * Initializes hardware and runs the ocarina state machine.
  * 
  * @return int Application status code (never returns)
  */
 int main()
 {
     // Hardware initialization
     PIO pio;
     uint sm, offset;
     
     const RGBColor led_color = {0, 50, 255}; /**< Default LED color (blue) */
     JoystickState js;
     const uint8_t music_speed = 1;           /**< Playback speed multiplier */
     const double led_intensity = 0.1;        /**< LED brightness level */
     const uint8_t font_width = 5;            /**< Display font size */
 
     stdio_init_all();
 
     // GPIO and peripheral initialization
     gpio_pin_init(BTN_A);
     gpio_pin_init(BTN_B);
     gpio_pin_init(JSTICK);
 
     // PWM audio initialization
     slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
     pwm_init_buzzer(BUZZER_PIN, slice_num);
 
     if (!init_system(&pio, &sm, &offset))
     {
         return 1; // Hardware initialization failed
     }
 
     joystick_init();
     game_state = STATE_IDLE;
 
     /**
      * @brief Main application loop
      * 
      * Implements the state machine for ocarina operation:
      * - Handles joystick input
      * - Manages note sequence validation
      * - Controls song playback
      */
     while (true) {
         // Update input devices
         joystick_update(&js);
         display_joystick_frame(js, led_color, pio, sm, led_intensity);
         
         // State machine controller
         switch (game_state) {
             case STATE_IDLE:
                 /**
                  * @state IDLE
                  * - Waits for user input
                  * - No active operations
                  */
                 break;
                 
             case STATE_LISTENING:
                 /**
                  * @state LISTENING
                  * - Managed by button callbacks
                  * - Collects note inputs
                  */
                 break;
                 
             case STATE_VALIDATING:
                 /**
                  * @state VALIDATING
                  * - Waits for current note to finish
                  * - Transitions to playback if sequence is complete
                  */
                 if (current_note_state == NOTE_OFF) {
                     printf("Correct sequence! Playing songs...\n");
                     game_state = STATE_PLAYING_SONG;
                     
                     // Add slight delay between songs
                     sleep_us(50000);
                     play_secret_tune();
                     sleep_us(500000);
                     play_song_of_storms();
                 }
                 break;
                 
             case STATE_PLAYING_SONG:
                 /**
                  * @state PLAYING_SONG
                  * - Songs play non-blocking
                  * - Checks for stop requests
                  */
                 if (should_stop_music) {
                     game_state = STATE_IDLE;
                     should_stop_music = false;
                 }
                 break;
                 
             case STATE_SONG_COMPLETE:
                 /**
                  * @state SONG_COMPLETE
                  * - Cleans up after playback
                  * - Returns to idle state
                  */
                 reset_player_buffer();
                 game_state = STATE_IDLE;
                 break;
                 
             default:
                 // Handle unexpected state
                 game_state = STATE_IDLE;
                 break;
         }
         
         // Main loop delay (10Hz refresh rate)
         sleep_us(100000);
     }
 }