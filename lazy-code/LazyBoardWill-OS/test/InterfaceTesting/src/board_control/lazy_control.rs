// Protobuffer message create
extern crate quick_protobuf; 
use quick_protobuf::Writer; 

// Serial crate
extern crate serial;
use std::io::prelude::*;
use serial::prelude::*;

// Internal Protobuffer Crate for
// our own messagess
use crate::messagedata; 

// Refcel and Rc stuff so we can have multiple pointers to the same stuff
// Will be used for sharing a single serial object across multiple "objects", that will be found
// 
use std::rc::Rc;
use std::cell::{Cell, RefCell};

pub fn new_teensy_control(port_ref: Rc<RefCell<serial::unix::TTYPort>>) -> TeensyControl{
    let teensy = TeensyControl{
        serial_port: port_ref
    };

    return teensy;
}

pub fn generate_messagedata_header(message_size: u32, message_type: messagedata::mod_MessageData::MessageType, return_message: bool) -> Vec<u8>{
    // Generate the struct of mesage
    let val = messagedata::MessageData{
        message_size: message_size, 
        message_type: message_type, 
        return_message: return_message,  
    };

    let mut out = Vec::new();
    let mut writer = Writer::new(&mut out);
    writer
        .write_message(&val)
        .expect("Message couldn't write properly");
    
    // Generally speaking the first 
    // Byte indicates the size of the array. 
    // But we don't require it for our purposes. 
    out.remove(0);

    // Fills in the message data that will
    // Indiciate what type of message this is!
    for x in 0..(16-out.len()){
        out.push(0);
    }
    return out; 
}

pub struct TeensyControl{
    // Serial port passover
    pub serial_port: Rc<RefCell<serial::unix::TTYPort>>
}

impl TeensyControl{
    pub fn send_ping(&mut self){
        
        let mut message = generate_messagedata_header(
            0, 
            messagedata::mod_MessageData::MessageType::GENERAL_INSTRUCTIONS, 
            false
        ); 

        // Sending device data over serial. 
        let _result = self.serial_port.borrow_mut().write(&message);
    }

    pub fn send_white_image(&mut self){
        let mut message = generate_messagedata_header(
            0, 
            messagedata::mod_MessageData::MessageType::OLED_FRAME_UPDATE,
            false 
        ); 
        
        // Max LED value
        for x in 0..32768{
            message.push(255); 
        }

        let _result = self.serial_port.borrow_mut().write(&message); 
    }

    pub fn cycle_matrix_animations(&mut self){
        // Provides messagedata fields. 
        let val = messagedata::MessageData{
            message_size: 0, 
            message_type: messagedata::mod_MessageData::MessageType::CHANGE_RGB_MATRIX_ANIMATION, 
            return_message: false
        };

        // Scopes out the protobuff messaging so we save memory
        
        let mut out = Vec::new();
        let mut writer = Writer::new(&mut out);
        writer
            .write_message(&val)
            .expect("Message couldn't write properly");
        
        // Generally speaking the first 
        // Byte indicates the size of the array. 
        // But we don't require it for our purposes. 
        out.remove(0);

        // Fills in the message data that will
        // Indiciate what type of message this is!
        for x in 0..(16-out.len()){
            out.push(0);
        }

        // Converts into a boxed pointer. 
        let msg_fill = out.into_boxed_slice();
        

        // Sending device data over serial. 
        let _result = self.serial_port.borrow_mut().write(&msg_fill);
    }
}