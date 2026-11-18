/*
 * BM83_UART.h
 *
 *  Created on: Nov 17, 2026
 *      Author: Jonathan
 */

#ifndef INC_BM83_UART_H_
#define INC_BM83_UART_H_

#include "main.h"
#include <stdbool.h>

HAL_StatusTypeDef BM83_Init(UART_HandleTypeDef *port);
void BM83_Process(bool audio_allowed, bool play_pause_pressed);


#endif /* INC_BM83_UART_H_ */
