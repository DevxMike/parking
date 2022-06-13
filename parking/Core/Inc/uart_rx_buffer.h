/*
 * uart_tx_buffer.h
 *
 *  Created on: May 19, 2022
 *      Author: Michael
 */

#ifndef INC_UART_TX_BUFFER_H_
#define INC_UART_TX_BUFFER_H_

#include <stdint.h>

const uint16_t buffer_len = 1000;

class buffer{
private:
    volatile char data[buffer_len];
    uint16_t buffer_read_pt;
    uint16_t buffer_write_pt;

public:
    buffer()
        : buffer_read_pt{ 0 }, buffer_write_pt{ 0 } {}       //init buffer pointers

    void write_to_buffer(char byte) volatile;
    char read_from_buffer() volatile;
    bool is_empty() volatile { return buffer_read_pt == buffer_write_pt; }
};

#endif /* INC_UART_TX_BUFFER_H_ */
