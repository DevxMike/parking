/*
 * uart_api.h
 *
 *  Created on: May 19, 2022
 *      Author: Michael
 */

#ifndef INC_UART_API_H_
#define INC_UART_API_H_
#include <stdint.h>

namespace packet_defs{
    enum{
        header_len = 4,
        packet_start = 0x69,
        max_data_len = 0xFF
    };

    typedef struct{
        uint8_t packet_start;
        uint8_t packet_type;
        uint8_t packet_length;
        uint8_t packet_crc;
    }header;

    typedef struct packet_structure{
        header packet_header;
        uint8_t data[packet_defs::max_data_len + packet_defs::header_len];
        packet_structure& operator=(const packet_structure& p);
    }packet_structure;

    typedef void (*callback_type)(const packet_defs::packet_structure& p, void* any);

    enum packet_states{
        waiting_for_start = 0,
        waiting_for_type = 1,
        waiting_for_len = 2,
        waiting_for_crc = 3,
        data_acquisition = 4
    };
}

class packet{
private:
    packet_defs::packet_structure packet_structure;
    packet_defs::callback_type callback;
    void* context;
    uint8_t packet_internal_state; //for the purpose of deserialization
    uint8_t get_crc(const packet_defs::packet_structure& p) const;

public:
    packet(packet_defs::callback_type packet_callback, void* packet_context):
        callback{ packet_callback }, context{ packet_context }, packet_internal_state{ 0 } {};

    void serialize_data(const packet_defs::packet_structure& p);
    void deserialize_data(const uint8_t* data, uint8_t data_len);
    const packet_defs::packet_structure& get_packet() const;
};

#endif /* INC_UART_API_H_ */
