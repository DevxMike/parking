/*
 * uart_rx_buffer.h
 *
 *  Created on: May 19, 2022
 *      Author: Michael
 */

#include "uart_rx_buffer.h"

void buffer::write_to_buffer(char byte) volatile{
    data[buffer_write_pt] = byte;                            //write to buffer
    buffer_write_pt = (buffer_write_pt + 1) % buffer_len;    // increment buffer write pointer
}

char buffer::read_from_buffer() volatile{
    if(buffer_read_pt != buffer_write_pt){
        char byte = data[buffer_read_pt];                    //read data from buffer
        buffer_read_pt = (buffer_read_pt + 1) % buffer_len;  //increment read pointer

        return byte;                                         //return data
    }

    return -1;                                               //return -1 if buffer is empty
}
