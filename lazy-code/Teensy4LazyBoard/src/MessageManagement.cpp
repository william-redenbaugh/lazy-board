/*
Author: William Redenbaugh
Last Edit Date: 7/3/2020
*/

#include "MessageManagement.hpp"

/**************************************************************************/
/*!
    @brief Starts up our serial interface for doing message management stuff 
*/
/**************************************************************************/
void MessageManagement::begin(void){    
    // Doesn't really matter what we put in here
    // It's always running at 12 megabits anyway!
    Serial.begin(115200);
}

/**************************************************************************/
/*!
    @brief Serial message unpacker the message management class. Will then connect data to other message unpacking functions. 
*/
/**************************************************************************/
bool MessageManagement::run(void){

    if(Serial.available() >= 16){
        // Scoping the serial read and decoding so that it gets 
        // popped off the stack asap. 
            uint8_t message_instr_arr[16];
            // Popping off the oldest 16 bytes of information
            // for the message instruction data. 
            for(uint8_t i = 0; i < 16; i++)
                message_instr_arr[i] = Serial.read();
            // Message instruction unpacking
            // Message unpacking instructions. 
            pb_istream_t msg_in = pb_istream_from_buffer(message_instr_arr, 16);
            pb_decode(&msg_in, MessageData_fields, &this->latest_message_data);

        return true;
    }

    return false; 
}

/**************************************************************************/
/*!
    @brief Serial message unpacker the message management class. Will then connect data to other message unpacking functions. 
    @returns MessageData information
*/
/**************************************************************************/
MessageData_MessageType MessageManagement::latest_message_enum(void){
    return this->latest_message_data.message_type;
}

/**************************************************************************/
/*!
    @returns the latest boolean value of the return message. 
*/
/**************************************************************************/
bool MessageManagement::latest_return_message(void){
    return this->latest_message_data.return_message;
}

/**************************************************************************/
/*!
    @returns the latest size value of the return message. 
*/
/**************************************************************************/
int32_t MessageManagement::return_message_size(void){
    return this->latest_message_data.message_size;
}

/**************************************************************************/
/*!
    @brief Unpacks message general instruction enumerated types. 
*/
/**************************************************************************/
void MessageManagement::process_general_instructions(void){
    // Array with latest package information. 
    uint8_t general_instr_buff[this->latest_message_data.message_size];
    
    // Get latest data off serial device. 
    for(uint8_t i = 0; i < this->latest_message_data.message_size; i++)
        general_instr_buff[i] = Serial.read();
 
    pb_istream_t msg_in = pb_istream_from_buffer(general_instr_buff, this->latest_message_data.message_size);
    pb_decode(&msg_in, GeneralInstructions_fields, &this->general_instructions);
}

/**************************************************************************/
/*!
    @brief Processes keybinding information. 
*/
/**************************************************************************/
void MessageManagement::process_keybinding_information(void){
    uint8_t keybinding_buff[this->latest_message_data.message_size];
    // Get latest data off serial device. 
    for(uint8_t i = 0; i < this->latest_message_data.message_size; i++)
        keybinding_buff[i] = Serial.read();
    
    pb_istream_t msg_in = pb_istream_from_buffer(keybinding_buff, this->latest_message_data.message_size);
    pb_decode(&msg_in, ProgramKeybindings_fields, &this->latest_keybinding_info);
}

/**************************************************************************/
/*!
    @returns latest keybinding information. should be called after process_keybinding_informatio()
*/
/**************************************************************************/
ProgramKeybindings MessageManagement::get_keybinding_information(void){
    return this->latest_keybinding_info;
}

/**************************************************************************/
/*!
    @returns latest image flash information. Should be called after the "process_image_data()" function
*/
/**************************************************************************/
ImageFlash MessageManagement::image_data_instructions(void){
    return this->latest_image_flash_data; 
}

/**************************************************************************/
/*!
    @brief processes our latest image instruction data. 
    @note This does not process the actual image information, that data should still be in the Serial buffer. Might be changed later. 
*/
/**************************************************************************/
void MessageManagement::process_image_data(void){
    uint8_t image_data_buff [this->latest_message_data.message_size];
    // Deserialize our instructions. 
    for(uint8_t i = 0; i < this->latest_message_data.message_size; i++)
        image_data_buff[i] = Serial.read();
    
    pb_istream_t msg_in = pb_istream_from_buffer(image_data_buff, this->latest_message_data.message_size);
    pb_decode(&msg_in, ImageFlash_fields, &this->latest_image_flash_data);
}

/**************************************************************************/
/*!
    @brief Processes rgb instructions
*/
/**************************************************************************/
void MessageManagement::process_rgb_instructions(void){
     uint8_t rgb_data_buff[this->latest_message_data.message_size];
    // Get latest data off serial device. 
    for(uint8_t i = 0; i < this->latest_message_data.message_size; i++)
        rgb_data_buff[i] = Serial.read();
    
    pb_istream_t msg_in = pb_istream_from_buffer(rgb_data_buff, this->latest_message_data.message_size);
    pb_decode(&msg_in, GeneralRGBData_fields, &this->latest_general_rgb_data);
}

/**************************************************************************/
/*!
    @returns latest keybinding information. should be called after process_keybinding_informatio()
*/
/**************************************************************************/
GeneralRGBData MessageManagement::get_rgb_general_instructions(void){
    return latest_general_rgb_data;
}

/**************************************************************************/
/*!
    @returns The latest general instruction data, should be called right after we know we got new general instruction data.  
*/
/**************************************************************************/
GeneralInstructions_MainInstrEnum MessageManagement::get_latest_general_instructions(void){
    this->process_general_instructions();
    return this->general_instructions.main_instructions;
}

/**************************************************************************/
/*!
    @brief Tests to make sure that protbuffer serialization and deserialization is working properly. 
    @returns boolean value that test was successful.  
*/
/**************************************************************************/

bool MessageManagement::testing_message_protobuffers(void){
    MessageData message_data_out;
    message_data_out.message_size = 32;
    message_data_out.message_type = MessageData_MessageType_GENERAL_INSTRUCTIONS;

    // Put data into serialized format. 
    uint8_t buffer[16];
    memset(buffer, 0, sizeof(buffer));
    pb_ostream_t msg_out = pb_ostream_from_buffer(buffer, sizeof(buffer));
    pb_encode(&msg_out, MessageData_fields, &message_data_out);

    // Unpack serialsed data. 
    pb_istream_t msg_in = pb_istream_from_buffer(buffer, sizeof(buffer));

    MessageData message_data_in; 
    pb_decode(&msg_in, MessageData_fields, &message_data_in);

    if(message_data_in.message_size == message_data_out.message_size && message_data_in.message_type == message_data_out.message_type)
        return true; 
    
    return false; 
}