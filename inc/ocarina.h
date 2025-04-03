/**
 * @file ocarina.h
 * @brief Ocarina music controller for Zelda-like sound system
 * 
 * Handles note playback, sequence validation, and song management
 * using PWM audio generation on Raspberry Pi Pico.
 */

 #ifndef OCARINA_H
 #define OCARINA_H
 
 #include "define_pins.h"
 #include "pico/stdlib.h"
 #include "hardware/pwm.h"
 #include "hardware/clocks.h"
 #include "hardware/gpio.h"
 #include "math.h"
 
 #include "inc/songs/song_of_storms.h"
 #include "inc/timers_callback.h"
 
 /** PWM top value for frequency calculation */
 #define PWM_TOP 65535
 
 /** Default buzzer volume level (50% duty cycle) */
 #define VOLUME_BUZZER 512
 
 /** Length of note sequences */
 #define SEQUENCE_LENGTH 6
 
 /**
  * @enum OcarinaNotes
  * @brief Musical notes available on the ocarina
  * 
  * Notes are defined by their position in the frequency array.
  * Contains both base octave and high octave notes.
  */
 typedef enum {
     /* Base octave notes */
     OCARINA_DO      = 0,   /**< C4 (261.63 Hz) */
     OCARINA_RE      = 2,   /**< D4 (293.66 Hz) */
     OCARINA_MI      = 4,   /**< E4 (329.63 Hz) */
     OCARINA_FA      = 5,   /**< F4 (349.23 Hz) */
     OCARINA_SOL     = 7,   /**< G4 (392.00 Hz) */
     OCARINA_LA      = 9,   /**< A4 (440.00 Hz) */
     OCARINA_SI      = 11,  /**< B4 (493.88 Hz) */
     
     /* High octave notes */
     OCARINA_DO_HIGH  = 12, /**< C5 (523.25 Hz) */
     OCARINA_RE_HIGH  = 14, /**< D5 (587.33 Hz) */
     OCARINA_MI_HIGH  = 16, /**< E5 (659.26 Hz) */
     OCARINA_FA_HIGH  = 17, /**< F5 (698.46 Hz) */
     OCARINA_SOL_HIGH = 19, /**< G5 (783.99 Hz) */
     OCARINA_LA_HIGH  = 21, /**< A5 (880.00 Hz) */
     OCARINA_SI_HIGH  = 23, /**< B5 (987.77 Hz) */
     
     OCARINA_NONE     = -1  /**< Silence/special value */
 } OcarinaNotes;
 
 /**
  * @struct NoteBuffer
  * @brief Tracks player's note input sequence
  */
 typedef struct {
     int buffer[SEQUENCE_LENGTH];  /**< Circular buffer for note storage */
     int current_index;            /**< Current position in buffer */
     bool is_sequence_correct;     /**< Sequence validation flag */
 } NoteBuffer;
 
 /** Global PWM slice number */
 static uint slice_num;
 
 /** Player's note input buffer */
 extern NoteBuffer player_buffer;
 
 /**
  * @brief Initialize PWM for buzzer output
  * @param pin GPIO pin number
  * @param slice_num PWM slice number
  */
 void pwm_init_buzzer(uint pin, uint slice_num);
 
 /**
  * @brief Play the complete Song of Storms
  */
 void play_song_of_storms(void);
 
 /**
  * @brief Play the secret discovery tune
  */
 void play_secret_tune(void);
 
 /**
  * @brief Play single ocarina note
  * @param note_index Index from OcarinaNotes enum
  */
 void play_ocarina_note(int note_index);
 
 /**
  * @brief Stop currently playing note
  */
 void stop_ocarina_note(void);
 
 /**
  * @brief Change PWM frequency for current note
  * @param note Note index to play
  */
 void change_note(int note);
 
 /**
  * @brief Reset player's note sequence buffer
  */
 void reset_player_buffer(void);
 
 /**
  * @brief Validate note against current sequence
  * @param note Current note to check
  */
 void check_note_sequence(int note);
 
 #endif