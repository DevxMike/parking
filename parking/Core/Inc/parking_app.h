/*
 * parking_app.h
 *
 *  Created on: Jun 13, 2022
 *      Author: Michael
 */
#include "uart_rx_buffer.h"
#include "uart_api.h"

#ifndef INC_PARKING_APP_H_
#define INC_PARKING_APP_H_

class parking_app{
private:
	static packet uart_deserializer;
	static void packet_callback(const packet_defs::packet_structure& p, void* any);

public:
	static buffer rx_buffer;
	static void main();
};

#ifdef __cplusplus
extern "C"{
#endif
void _run_app();
#ifdef __cplusplus
}
#endif

#endif /* INC_PARKING_APP_H_ */
