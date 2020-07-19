/*
Author: William Redenbaugh
Last Edit Date: 7/3/2020
*/

#ifndef _MESSAGEMANAGEMENT_HPP
#define _MESSAGEMANAGEMENT_HPP
#include "Arduino.h"

// Protobuffer Stuff. 
#include "pb.h"
#include "pb_decode.h"
#include "pb_encode.h"

// Our message data. 
#include "status.pb.h"
#include "messagedata.pb.h"
#include "general_instructions.pb.h"
#include "program_keybindings.pb.h"
#include "general_rgb_instructions.pb.h"
#include "image_flash.pb.h"

class MessageManagement{
public: 
    // Initialization Sequence for Message management object
    void begin(void);
    
    // function to run for message management, should be running in void loop command
    // since that function has the most free stack space. 
    bool run(void);
    
    // Holds the latest message data. 
    MessageData_MessageType latest_message_enum(void);

    // Returns the latest boolean value if whether eor not to get latest instruction data. 
    bool latest_return_message(void);

    // Returns the size of the message
    int32_t return_message_size(void);

    // Returns the latest General instruction data. 
    GeneralInstructions_MainInstrEnum get_latest_general_instructions(void);

    // Allows to unpack and pass keybinding information. 
    void process_keybinding_information(void);
    ProgramKeybindings get_keybinding_information(void);
    
    // Allows to unpack and pass rgb information
    void process_rgb_instructions(void);
    GeneralRGBData get_rgb_general_instructions(void);

    // Allows us to unpack and understand what 
    // our image data instructions are doing. 
    void process_image_data(void);
    ImageFlash image_data_instructions(void);

    // Test code for making sure that the protobuffers are
    // packing and unpacking properly
    bool testing_message_protobuffers(void);

private:
    // Handler for processing general instructions. 
    void process_general_instructions(void);

    // Handler for processing matrix information
    void processing_matrix_information(void);

    // Handler for processing LED strip information
    void processing_led_strip_information(void);
    
    // latest message data
    MessageData latest_message_data; 

    // latest general instruction data. 
    GeneralInstructions general_instructions; 

    // Latest keybinding data. 
    ProgramKeybindings latest_keybinding_info; 

    // Latest RGB general instruction data; 
    GeneralRGBData latest_general_rgb_data; 

    // Latest Image data
    ImageFlash latest_image_flash_data; 
};

#endif 
