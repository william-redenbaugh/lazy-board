// Rust's Files
use std::net::UdpSocket;
use std::{thread, time};
use std::time::Duration;
use std::sync::{mpsc, Arc, Mutex};
use std::rc::Rc;
use std::cell::{Cell, RefCell};

// Serial module that we are using
extern crate serial;

mod protobuf; 
use protobuf::{
    messagedata, 
    GifFrameUpdate
}; 

mod board_control; 
use board_control::{lazy_control};

fn main() {
    let mut p = serial::open("/dev/ttyACM0").unwrap();
    let port_ref = Rc::new(RefCell::new(p));
    let mut board_control = lazy_control::new_teensy_control(port_ref); 
    board_control.cycle_matrix_animations();   
    //board_control.send_white_image();     

}