#ifndef _PROTO_MSG_UNPACK_H
#define _PROTO_MSG_UNPACK_H

// Arduino standard library
#include <Arduino.h> 

// Nanopb serialization and deserialization stuff
#include <pb.h> 
#include <pb_common.h> 
#include <pb_decode.h> 
#include <pb_encode.h> 

// Our Protobuffer Messages!
#include "messagedata.pb.h"
#include "RGBMatrixChange.pb.h"

MessageData unpack_message_data(uint8_t *arr, uint32_t len);
RGBMatrixChange unpack_rgb_matrix_data(uint8_t *arr, uint32_t len); 

#endif 