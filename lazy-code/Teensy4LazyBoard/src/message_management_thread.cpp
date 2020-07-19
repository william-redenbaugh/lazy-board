/*
Author: William Redenbaugh
Last Edit Date: 7/3/2020
*/

#include "message_management_thread.hpp"

// Message management handler, lets us deal with messaging stuff in another thread. 
MessageManagement message_management; 

systime_t message_thread_begin_tick; 
systime_t message_thread_end_tick;

extern void start_message_management(void);
extern void loop(void);

void run_general_instructions(void);
void run_keybinding_instructions(void);
void run_image_flash(void);
void run_rgb_instructions(void);

/**************************************************************************/
/*!
    @brief Allows us to start up the message management stuff. 
*/
/**************************************************************************/
extern void start_message_management(void){
    message_management.begin();
}

/**************************************************************************/
/*!
    @brief Where our message management stuff sits.(in our main thread. )
*/
/**************************************************************************/
extern void loop(void) {
    message_thread_begin_tick = chVTGetSystemTimeX();
    // Running our message management on the main loop thread. 
    if(message_management.run()){
        switch (message_management.latest_message_enum()){
        case (MessageData_MessageType_GENERAL_INSTRUCTIONS):
        run_general_instructions();
        break;
        
        case(MessageData_MessageType_PROGRAM_KEYBINDINGS):
        run_keybinding_instructions();
        break;

        case(MessageData_MessageType_PROGRAM_RGB_ANIMATIONS):
        run_rgb_instructions();
        break;

        case(MessageData_MessageType_PROGRAM_DISPLAY):
        break;

        case(MessageData_MessageType_IMAGE_FLASH):
        run_image_flash();
        break;

        default:
        break;
        }
    }

    // So we have the whole system run every 200 milliseconds. 
    message_thread_end_tick = message_thread_begin_tick + TIME_I2MS(200);
    if(message_thread_end_tick > chVTGetSystemTimeX())
    chThdSleepUntil(message_thread_end_tick);

}

/**************************************************************************/
/*!
    @brief Whenever we get new general instruction data, it should sit here. 
*/
/**************************************************************************/
inline void run_general_instructions(void){
    
    switch(message_management.get_latest_general_instructions()){
    case(GeneralInstructions_MainInstrEnum_DO_NOTHING):
    break;

    case(GeneralInstructions_MainInstrEnum_REBOOT):
    break;

    case(GeneralInstructions_MainInstrEnum_FREE_MEM):
    break;

    case(GeneralInstructions_MainInstrEnum_STATUS):
    break;

    default:
    break;
    }
}

/**************************************************************************/
/*!
    @brief Whenever we get new image information for flashing, we put that information here. 
*/
/**************************************************************************/
inline void run_image_flash(void){
    message_management.process_image_data();
    ImageFlash latest_image_flash_data = message_management.image_data_instructions();
    process_image_flash(latest_image_flash_data);
   
}

/**************************************************************************/
/*!
    @brief Whenever we get new rgb information, it should sit here. 
*/
/**************************************************************************/
inline void run_rgb_instructions(void){
    message_management.process_rgb_instructions();
    GeneralRGBData rgb_data = message_management.get_rgb_general_instructions();
    if(!rgb_data.more_data){
        switch(rgb_data.message_type){
            case(GeneralRGBData_RGBMessageType_MATRIX_CYCLE_INDIVIDUAL):
            change_led_animation(MATRIX_CYCLE_INDIVIDUAL); 
            break;

            case(GeneralRGBData_RGBMessageType_MATRIX_CYCLE_ALL):
            change_led_animation(MATRIX_CYCLE_ALL);
            break;

            case(GeneralRGBData_RGBMessageType_MATRIX_STATIC_INDIVIDUAL):
            change_led_animation(MATRIX_STATIC_INDIVIDUAL); 
            break;

            case(GeneralRGBData_RGBMessageType_MATRIX_KEYTRIGGER_REACTIVE):
            change_led_animation(MATRIX_REACTIVE);
            break;

            case(GeneralRGBData_RGBMessageType_MATRIX_TRIGGER_RIPPLE):
            change_led_animation(MATRIX_RIPPLE);
            break;
        }
    }
}

/**************************************************************************/
/*!
    @brief Whenever we get new keybinding information, it should process here. 
*/
/**************************************************************************/
inline void run_keybinding_instructions(void){
    message_management.process_keybinding_information();
    ProgramKeybindings keybind_buff = message_management.get_keybinding_information(); 
    uint16_t keymap[16];
    
    keymap[0] = keybind_buff.macro_zero;
    keymap[1] = keybind_buff.macro_one;
    keymap[2] = keybind_buff.macro_two;
    keymap[3] = keybind_buff.macro_three;
    keymap[4] = keybind_buff.macro_four;
    keymap[5] = keybind_buff.macro_five;
    keymap[6] = keybind_buff.macro_six;
    keymap[7] = keybind_buff.macro_seven;
    keymap[8] = keybind_buff.macro_eight;
    keymap[9] = keybind_buff.macro_nine;
    keymap[10] = keybind_buff.macro_ten;
    keymap[11] = keybind_buff.macro_eleven;
    keymap[12] = keybind_buff.macro_twelve;
    keymap[13] = keybind_buff.macro_thirteen;
    keymap[14] = keybind_buff.macro_fourteen;
    keymap[15] = keybind_buff.macro_fifteen;

    reprogram_key(keymap, sizeof(keymap));    
}