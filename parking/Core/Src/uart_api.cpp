/*
 * uart_api.cpp
 *
 *  Created on: May 19, 2022
 *      Author: Michael
 */
#include "uart_api.h"

packet_defs::packet_structure& packet_defs::packet_structure::operator=(const packet_defs::packet_structure& p){
    packet_header.packet_start = p.packet_header.packet_start;
    packet_header.packet_type = p.packet_header.packet_type;
    packet_header.packet_length = p.packet_header.packet_length;
    packet_header.packet_crc = p.packet_header.packet_crc;

    for(uint16_t i = 0; i < packet_header.packet_length; ++i){
        data[i] = p.data[i];
    }

    return *this;
}

uint8_t packet::get_crc(const packet_defs::packet_structure& p) const{
    uint8_t tmp_crc = 0;

    tmp_crc += p.packet_header.packet_length;

    for(uint16_t i = 0; i < p.packet_header.packet_length; ++i){
        tmp_crc += p.data[i];
    }

    return tmp_crc;
}

void packet::serialize_data(const packet_defs::packet_structure& p){
    packet_structure = p;

    //header serialization
    packet_structure.data[0] = p.packet_header.packet_start;
    packet_structure.data[1] = p.packet_header.packet_type;
    packet_structure.data[2] = p.packet_header.packet_length;
    packet_structure.data[3] = packet_structure.packet_header.packet_crc = get_crc(p);

    //data serialization
    for(uint16_t i = packet_defs::header_len; i < p.packet_header.packet_length + packet_defs::header_len; ++i){
        packet_structure.data[i] = p.data[i - packet_defs::header_len];
    }
}

void packet::deserialize_data(const uint8_t* data, uint8_t data_len){
    for(uint8_t i = 0; i < data_len; ++i){
        uint8_t tmp = data[i];
        static uint8_t data_count;

        switch(packet_internal_state){
            case packet_defs::packet_states::waiting_for_start:
            if(tmp == packet_defs::packet_start){
                packet_structure.packet_header.packet_start = tmp;
                packet_internal_state = packet_defs::packet_states::waiting_for_type;
            }
            break;

            case packet_defs::packet_states::waiting_for_type:
                packet_structure.packet_header.packet_type = tmp;
                packet_internal_state = packet_defs::packet_states::waiting_for_len;
            break;

            case packet_defs::packet_states::waiting_for_len:
                packet_structure.packet_header.packet_length = tmp;
                data_count = 0;
                packet_internal_state = packet_defs::packet_states::waiting_for_crc;
            break;

            case packet_defs::packet_states::waiting_for_crc:
                packet_structure.packet_header.packet_crc = tmp;
                packet_internal_state = packet_defs::packet_states::data_acquisition;
            break;

            case packet_defs::packet_states::data_acquisition:
                packet_structure.data[data_count++] = tmp;
            break;
        }
        auto statement = packet_internal_state == packet_defs::packet_states::data_acquisition;

        if(data_count >= packet_structure.packet_header.packet_length && statement){
            uint8_t tmp_crc = get_crc(packet_structure);

            if(tmp_crc == packet_structure.packet_header.packet_crc){
                callback(packet_structure, context);
                for(uint16_t i = 0; i < packet_defs::max_data_len; ++i){
                	packet_structure.data[i] = 0;
                }
            }
            --i;
            packet_internal_state = packet_defs::packet_states::waiting_for_start;
        }
    }
}

const packet_defs::packet_structure& packet::get_packet() const{
    return packet_structure;
}

