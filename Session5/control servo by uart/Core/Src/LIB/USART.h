/*
 * USART.h
 *
 *  Created on: Aug 1, 2026
 *      Author: PC
 */

#ifndef SRC_LIB_USART_H_
#define SRC_LIB_USART_H_

#include "stm32f103xb.h"
#include <stdint.h>

void USART1_init();
void UART_transmit(char*arr);
uint8_t UART_ReceiveByte(void);



#endif /* SRC_LIB_USART_H_ */
