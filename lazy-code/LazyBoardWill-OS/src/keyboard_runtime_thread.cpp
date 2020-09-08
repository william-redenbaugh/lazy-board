/*
Author: William Redenbaugh
Last Edit Date: 7/24/2020
*/

#include "keyboard_runtime_thread.hpp"

// Setting up the current keymap information. 
static volatile uint16_t current_keymap[NUM_ROWS * NUM_COLS];

/*!
*   @brief Whether a new click has been triggered 
*/
static bool new_click = false; 

/*!
* @brief Current key state information
*/
static KeyState key_state; 

/*!
*   @brief key state information
*   @note Used so we don't double unpress/depress a key. 
*/
static KeyState prev_key_state;

/*!
*   @brief Key State information for latest changes. 
*   @note Whenever there are any changes in button presses the program, they are saved in this array 
*/
static KeyState pressed_changed_key_states; 

/*!
*   @brief Millis timestamp of start of subroutine. 
*/
static uint32_t last_millis; 

/*!
*   @brief Mutex that deals with any thread "unsafe" operarations
*/
static MutexLock keyboard_mutex; 

static void keyboard_message_callback(MessageReq *msg); 
void keyboard_runtime_func(void); 
__attribute__((always_inline)) static void check_new_press(uint8_t x); 
__attribute__((always_inline)) static void check_new_release(uint8_t x); 
__attribute__((always_inline)) static void sleep_keyboard_thread(void); 
__attribute__((always_inline)) static void check_new_press(void); 
extern void reprogram_key(uint16_t map[], size_t map_size); 
void reset_keymap(void); 
void save_keymap_eeprom(void); 
void load_keymap_eeprom(void); 
uint16_t convert_proto_keymap(ProgramKeybindings_KeyType proto_key);

void keyboard_message_callback(MessageReq *msg){

}

/*!
    @brief Thread function and stack space for dealing the keyboard runtime stuff. 
*/
void keyboard_runtime_func(void){

    // Setup the keyboard gpio stuff. 
    start_keyboard_gpio();
    // Clean up the keymaps. 
    reset_keymap();

    // Set all of the previous key states to (1)
    memset((void*)prev_key_state, 1, sizeof(prev_key_state));
    // Sets all of the animation states to (1) 
    memset((void*)pressed_changed_key_states, 1, sizeof(pressed_changed_key_states));

    // Setup the keyboard 
    Keyboard.begin();

    add_message_callback(MessageData_MessageType_UPDATE_KEYBINDINGS, keyboard_message_callback); 

    while(1){
        last_millis = millis(); 

        // Reads in the keyboard data from the matrix. 
        read_keyboard_gpio();
        get_keyboard_values(key_state);   

        // All shared thread resources are touched right here. 
        keyboard_mutex.lockWaitIndefinite(); 
        // Run through 2D array, check which keys are pressed and which arent. 
        for(uint8_t x = 0; x < NUM_ROWS * NUM_COLS; x++){
            check_new_press(x);
            check_new_release(x);
        }
        keyboard_mutex.unlock(); 

        // END OF KEYSTROKE KEYBOARD OUTPUT // 

        // Checking if there are any new clicks 
        check_new_press();
        // Sleep thread for remainder of 14 milliseconds 
        sleep_keyboard_thread(); 
    }   
}

/*!
*   @brief Checking to see if there is a new press on the keyboard 
*   @param uint8_t x which key are we looking at? 
*/
__attribute__((always_inline)) static void check_new_press(uint8_t x){
    if((key_state[x] == 0) && !(key_state[x] == prev_key_state[x])){
        pressed_changed_key_states[x] = 0; 
        new_click = true; 
        Keyboard.press(current_keymap[x]);
        // Setting the previous key state to the next key_state
        prev_key_state[x] = key_state[x];
        //trigger_color(random(64000));
    }
}

/*!
*   @brief Checking to see if there is a new key release on the keyboard 
*   @param uint8_t x which key are we looking at. 
*/
__attribute__((always_inline)) static void check_new_release(uint8_t x){
    if((key_state[x] == 1) && !(key_state[x] == prev_key_state[x])){
        new_click = true; 
        Keyboard.release(current_keymap[x]);
        // Setting the previous key state to the next key_state
        prev_key_state[x] = key_state[x];
        key_state[x] = 0; 
        //trigger_color(random(64000));
    }
}

/*!
*   @brief Sleeps the keyboard thread for the remainder of the time. 
*/
__attribute__((always_inline)) static void sleep_keyboard_thread(void){
    // How much time has passed
    uint32_t millis_calc = millis() - last_millis; 
    if(millis_calc < 14)
        os_thread_delay_ms(14 - millis_calc);
}

/*!
*   @brief Checking to see if there are a new presses to send to LED strip thread, might move this into one of theo other functions. 
*/
__attribute__((always_inline)) static void check_new_press(void){
    if(new_click){
        // Send keystroke information to the LED strip thread, casts to volatile unsigned 8 bit integer. 
        trigger_keymap((volatile uint8_t*)pressed_changed_key_states);
        new_click = false; 
        memset((void*)pressed_changed_key_states, 1, sizeof(pressed_changed_key_states));
    }
}

/*!
   @brief Allows us to reprogram the current keymap. 
*/
extern void reprogram_key(uint16_t map[], size_t map_size){
    keyboard_mutex.lockWaitIndefinite(); 
    for(size_t i = 0; i < map_size; i++)
        current_keymap[i] = convert_proto_keymap(ProgramKeybindings_KeyType(map[i]));
    keyboard_mutex.unlock(); 
}

/*!
   @brief Allows us to reset our keymap to the default preloaded values at compile time. 
*/
void reset_keymap(void){
    keyboard_mutex.lockWaitIndefinite();
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
    keyboard_mutex.unlock(); 
}

/*!
   @brief Allows us to save our current keymap configuration into eeprome
*/
void save_keymap_eeprom(void){
    uint8_t x = 0; 
    keyboard_mutex.lockWaitIndefinite();
    for(uint8_t i = 0; i < 16; i++){
        EEPROM.write(x, current_keymap[i] >> 8);
        x++;
        EEPROM.write(x, current_keymap[i]);
        x++; 
    }
    keyboard_mutex.unlock(); 
}

/*!
   @brief Allows us to load our current keypmap from eeprome into memeory so we can use it. 
*/
void load_keymap_eeprom(void){
   keyboard_mutex.lockWaitIndefinite(); 
   for(uint8_t i = 0; i < 16; i++){
       current_keymap[i] = (EEPROM.read(i * 2) << 8) | EEPROM.read(i*2 + 1);
   } 
   keyboard_mutex.unlock(); 
}

/*!
   @brief Converts the protobuffer messages into actual keymap values. 
*/
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