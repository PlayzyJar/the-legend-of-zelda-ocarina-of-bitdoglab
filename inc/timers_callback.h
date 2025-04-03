/**
 * @file timers_callback.h
 * @brief GPIO interrupt handlers and timer management
 * 
 * Handles button debouncing, note timing, and state transitions
 * for the ocarina controller.
 */

 #ifndef TIMERS_CALLBACK_H
 #define TIMERS_CALLBACK_H
 
 #include "define_pins.h"
 #include "inc/ocarina.h"
 #include "inc/joystick.h"
 
 #include <stdbool.h>
 #include "pico/stdlib.h"
 #include "pico/bootrom.h"
 #include "hardware/gpio.h"
 #include <stdio.h>
 
 /** Minimum time between button presses (debounce) in milliseconds */
 #define DEBOUNCE_TIME_MS 250
 
 /**
  * @enum MusicState
  * @brief Playback status of the music system
  */
 typedef enum {
     MUSIC_PLAYING,  /**< Audio is currently playing */
     MUSIC_PAUSED    /**< Audio playback is paused */
 } MusicState;
 
 /**
  * @enum NoteState
  * @brief Status of individual note playback
  */
 typedef enum {
     NOTE_OFF,       /**< No note currently playing */
     NOTE_PLAYING    /**< Note is actively sounding */
 } NoteState;
 
 /**
  * @enum GameState
  * @brief Main state machine states
  */
 typedef enum {
     STATE_IDLE,         /**< Waiting for user input */
     STATE_LISTENING,    /**< Recording note sequence */
     STATE_VALIDATING,   /**< Checking completed sequence */
     STATE_PLAYING_SONG, /**< Playing full song */
     STATE_SONG_COMPLETE /**< Song finished playing */
 } GameState;
 
 /** Global game state machine */
 extern volatile GameState game_state;
 
 // Last interrupt timestamps for debouncing
 static volatile uint32_t last_interruption_b = 0;
 static volatile uint32_t last_interruption_a = 0;
 static volatile uint32_t last_interruption_jstck = 0;
 
 /** Global music playback state */
 extern volatile MusicState music_state;
 
 /** Flag to request music playback stop */
 extern volatile bool should_stop_music;
 
 /** Current note playback status */
 extern volatile NoteState current_note_state;
 
 /** Index of currently playing note */
 extern volatile int current_note_index;
 
 /**
  * @brief GPIO interrupt callback handler
  * @param gpio GPIO pin that triggered interrupt
  * @param events Event type that triggered interrupt
  */
 void btn_callback(uint gpio, uint32_t events);
 
 /**
  * @brief Initialize GPIO pin with interrupt handler
  * @param pin GPIO pin number to initialize
  */
 void gpio_pin_init(uint8_t pin);
 
 #endif