/*
 * parking_app.cpp
 *
 *  Created on: Jun 13, 2022
 *      Author: Michael
 */
#include "parking_app.h"
#include "usart.h"
#include "gpio.h"
#include "main.h"
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif
void _run_app(){
	parking_app::main();
}
#ifdef __cplusplus
}
#endif

packet parking_app::uart_deserializer{ parking_app::packet_callback, NULL };
buffer parking_app::rx_buffer{ };

uint8_t uart_data = 0;

void parking_app::main(){
	HAL_UART_Receive_IT(&huart2, &uart_data, 1);

	while(1){
		while(!rx_buffer.is_empty()){
			uint8_t data = (uint8_t)rx_buffer.read_from_buffer();
			uart_deserializer.deserialize_data(&data, 1);
		}

		HAL_Delay(1);
	}
}

void parking_app::packet_callback(const packet_defs::packet_structure& p, void* any){
	if(strcmp((char*)p.data, "open") == 0){
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	}
	else if(strcmp((char*)p.data, "close") == 0){
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	parking_app::rx_buffer.write_to_buffer((char)uart_data);
	HAL_UART_Receive_IT(&huart2, &uart_data, 1);
}
