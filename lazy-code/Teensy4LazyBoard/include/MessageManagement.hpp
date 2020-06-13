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
};

#endif 
