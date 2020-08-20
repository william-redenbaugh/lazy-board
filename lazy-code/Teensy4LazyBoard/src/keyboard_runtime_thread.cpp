/*
Author: William Redenbaugh
Last Edit Date: 7/3/2020
*/

#include "keyboard_runtime_thread.hpp"
#include "kb_macros_define.h"
#include "led_matrix_runtime.hpp"
#include "program_keybindings.pb.h"
#include "spi_display_runtime.hpp"
#include "EEPROM.h"

extern void start_keyboard_runtime_thread(void);
extern void reprogram_key(uint16_t map[], size_t map_size);

// Allows us to reset the current keymap. 
void reset_keymap(void);
uint16_t convert_proto_keymap(ProgramKeybindings_KeyType proto_key);

// Setting up the current keymap information. 
volatile uint16_t current_keymap[NUM_ROWS * NUM_COLS];

// Checking if there is a new click.
bool new_click = false; 

MUTEX_DECL(keymap_mutx);

/**************************************************************************/
/*!
    @brief Thread function and stack space for dealing the keyboard runtime stuff. 
*/
/**************************************************************************/
static THD_WORKING_AREA(keyboard_runtime_thread_wa, 4096);
static THD_FUNCTION(keyboard_runtime_thread, arg){
    (void)arg; 

    start_keyboard_gpio();
    Keyboard.begin();
    reset_keymap();
   
   // Latest keystate information. 
    KeyState key_state; 

    // previous key state information information
    KeyState prev_key_state;

    for(uint8_t i = 0; i < NUM_ROWS * NUM_COLS; i++)
        prev_key_state[i] = 1; 

    systime_t kb_thread_begin_tick;
    systime_t kb_thread_end_tick; 
 
    while(1){
        // Get current tick  aneiobfk
        kb_thread_begin_tick = chVTGetSystemTimeX();

        // Reads in the keyboard data from the matrix. 
        read_keyboard_gpio();
        get_keyboard_values(key_state);   

        // Lock Down our resource!
        chMtxLock(&keymap_mutx);
        // Run through 2D array, check which keys are pressed and which arent. 
        for(uint8_t x = 0; x < NUM_ROWS * NUM_COLS; x++){
            
            if((key_state[x] == 0) && !(key_state[x] == prev_key_state[x])){
                new_click = true; 
                Keyboard.press(current_keymap[x]);
                // Setting the previous key state to the next key_state
                prev_key_state[x] = key_state[x];
                trigger_color(random(64000));
            }
            if((key_state[x] == 1) && !(key_state[x] == prev_key_state[x])){
                new_click = true; 
                Keyboard.release(current_keymap[x]);
                // Setting the previous key state to the next key_state
                prev_key_state[x] = key_state[x];
                key_state[x] = 0; 
                trigger_color(random(64000));
            }
        }

        chMtxUnlock(&keymap_mutx);
        // END OF KEYSTROKE KEYBOARD OUTPUT // 

        if(new_click){
            // Send keystroke information to the LED strip thread, casts to volatile unsigned 8 bit integer. 
            trigger_keymap((volatile uint8_t*)key_state);
            new_click = false; 
        }
        
        // we sleep the remainder of the time for the keyboard. 
        kb_thread_end_tick = kb_thread_begin_tick + TIME_I2MS(33);
        if(kb_thread_end_tick > chVTGetSystemTimeX())
            chThdSleepUntil(kb_thread_end_tick);
    }   
}

/**************************************************************************/
/*!
    @brief Allows us to start up our keyboard runtime thread
*/
/**************************************************************************/
extern void start_keyboard_runtime_thread(void){
    chThdCreateStatic(keyboard_runtime_thread_wa, 
                      sizeof(keyboard_runtime_thread_wa), 
                      NORMALPRIO + 3, 
                      keyboard_runtime_thread, 
                      NULL);
}

/**************************************************************************/
/*!
   @brief  Allows us to reprogram the current keymap. 
*/
/**************************************************************************/
extern void reprogram_key(uint16_t map[], size_t map_size){
    chMtxLock(&keymap_mutx);
    for(size_t i = 0; i < map_size; i++)
        current_keymap[i] = convert_proto_keymap(ProgramKeybindings_KeyType(map[i]));
    chMtxUnlock(&keymap_mutx);
}

/**************************************************************************/
/*!
   @brief    Allows us to reset our keymap to the default preloaded values at compile time. 
*/
/**************************************************************************/
void reset_keymap(void){
    current_keymap[0] = DEFAULT_KB_MACRO_0;
    current_keymap[1] = DEFAULT_KB_MACRO_1; 
    current_keymap[2] = DEFAULT_KB_MACRO_2; 
    current_keymap[3] = DEFAULT_KB_MACRO_3;
    current_keymap[4] = DEFAULT_KB_MACRO_4; 
    current_keymap[5] = DEFAULT_KB_MACRO_5; 
    current_keymap[6] = DEFAULT_KB_MACRO_6; 
    current_keymap[7] = DEFAULT_KB_MACRO_7; 
    current_keymap[8] = DEFAULT_KB_MACRO_8; 
    current_keymap[9] = DEFAULT_KB_MACRO_9; 
    current_keymap[10] = DEFAULT_KB_MACRO_10; 
    current_keymap[11] = DEFAULT_KB_MACRO_11; 
}

/**************************************************************************/
/*!
   @brief    Allows us to save our current keymap configuration into eeprome
*/
/**************************************************************************/
void save_keymap_eeprom(void){
    uint8_t x = 0; 
    for(uint8_t i = 0; i < 16; i++){
        EEPROM.write(x, current_keymap[i] >> 8);
        x++;
        EEPROM.write(x, current_keymap[i]);
        x++; 
    }
}

/**************************************************************************/
/*!
   @brief    Allows us to load our current keypmap from eeprome into memeory so we can use it. 
*/
/**************************************************************************/
void load_keymap_eeprom(void){
   for(uint8_t i = 0; i < 16; i++){
       current_keymap[i] = (EEPROM.read(i * 2) << 8) | EEPROM.read(i*2 + 1);
   } 
}

/**************************************************************************/
/*!
   @brief    Converts the protobuffer messages into actual keymap values. 
*/
/**************************************************************************/
uint16_t convert_proto_keymap(ProgramKeybindings_KeyType proto_key){
    switch(proto_key){
        case(ProgramKeybindings_KeyType_CTRL):
        return KEY_LEFT_CTRL;
        
        case(ProgramKeybindings_KeyType_SHIFT):
        return KEY_LEFT_SHIFT;
        
        case(ProgramKeybindings_KeyType_ALT):
        return KEY_LEFT_ALT;
        
        case(ProgramKeybindings_KeyType_GUI):
        return KEY_LEFT_GUI;
        
        case(ProgramKeybindings_KeyType_LEFT_CTRL):
        return KEY_LEFT_CTRL;

        case(ProgramKeybindings_KeyType_LEFT_SHIFT):
        return KEY_LEFT_SHIFT;

        case(ProgramKeybindings_KeyType_LEFT_ALT):
        return KEY_LEFT_ALT; 

        case(ProgramKeybindings_KeyType_LEFT_GUI):
        return KEY_LEFT_GUI; 

        case(ProgramKeybindings_KeyType_RIGHT_CTRL):
        return KEY_RIGHT_CTRL; 

        case(ProgramKeybindings_KeyType_RIGHT_SHIFT):
        return KEY_RIGHT_SHIFT;

        case(ProgramKeybindings_KeyType_RIGHT_ALT):
        return KEY_RIGHT_ALT;

        case(ProgramKeybindings_KeyType_RIGHT_GUI):
        return KEY_RIGHT_GUI; 

        case(ProgramKeybindings_KeyType_SYSTEM_POWER_DOWN):
        return KEY_SYSTEM_POWER_DOWN;

        case(ProgramKeybindings_KeyType_SYSTEM_SLEEP):
        return KEY_SYSTEM_SLEEP;

        case(ProgramKeybindings_KeyType_SYSTEM_WAKE_UP):
        return KEY_SYSTEM_WAKE_UP;

        case(ProgramKeybindings_KeyType_PLAY):
        return KEY_MEDIA_PLAY; 

        case(ProgramKeybindings_KeyType_MEDIA_PAUSE):
        return KEY_MEDIA_PAUSE;

        case(ProgramKeybindings_KeyType_RECORD):
        return KEY_MEDIA_RECORD; 

        case(ProgramKeybindings_KeyType_FAST_FORWARD):
        return KEY_MEDIA_FAST_FORWARD;

        case(ProgramKeybindings_KeyType_REWIND):
        return KEY_MEDIA_REWIND;

        case(ProgramKeybindings_KeyType_NEXT_TRACK):
        return KEY_MEDIA_NEXT_TRACK;

        case(ProgramKeybindings_KeyType_PREV_TRACK):
        return KEY_MEDIA_PREV_TRACK;

        case(ProgramKeybindings_KeyType_STOP):
        return KEY_MEDIA_STOP; 

        case(ProgramKeybindings_KeyType_EJECT):
        return KEY_MEDIA_EJECT;

        case(ProgramKeybindings_KeyType_RANDOM_PLAY):
        return KEY_MEDIA_RANDOM_PLAY;

        case(ProgramKeybindings_KeyType_PLAY_PAUSE):
        return KEY_MEDIA_PLAY_PAUSE;

        case(ProgramKeybindings_KeyType_MUTE):
        return KEY_MEDIA_MUTE; 

        case(ProgramKeybindings_KeyType_VOLUME_INC):
        return KEY_MEDIA_VOLUME_INC;

        case(ProgramKeybindings_KeyType_VOLUME_DEC):
        return KEY_MEDIA_VOLUME_DEC;
    
        default:
        
        // Converts to keys that the program can understand
        if(proto_key >= 30 && proto_key <= 127)
            return (   (proto_key - 26)  | 0xF000 );
        else if(proto_key >= 128 && proto_key <= 139)
            return (   (proto_key - 24)  | 0xF000 );
        break;
    }

    return KEY_SPACE;
}