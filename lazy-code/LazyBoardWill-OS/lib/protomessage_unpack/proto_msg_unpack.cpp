#include "proto_msg_unpack.h"

MessageData unpack_message_data(uint8_t *arr, uint32_t len);

/*!
* @brief Allows us to quickly unpack the message data.
* @param uint8_t *arr(pointer to array that contains the message)
* @param uint32_t len(length of the array)
* @returns MessageData struct with all the information
*/
MessageData unpack_message_data(uint8_t *arr, uint32_t len){
    MessageData msg_data;
    pb_istream_t msg_in = pb_istream_from_buffer(arr, len);
    pb_decode(&msg_in, MessageData_fields, &msg_data);
    return msg_data; 
}

/*!
* @brief Quickly unpack the rgb matrix data. 
* @note Be careful with pointers!
* @param uint8_t *arr(pointer to array that contains the message)
* @param uint32_t len(length of the array)
* @returns MessageData struct with all the information
*/
RGBMatrixChange unpack_rgb_matrix_data(uint8_t *arr, uint32_t len){
    RGBMatrixChange msg_data;
    pb_istream_t msg_in = pb_istream_from_buffer(arr, len);
    pb_decode(&msg_in, RGBMatrixChange_fields, &msg_data);
    return msg_data; 
}