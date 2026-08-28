/*
 * USART.c
 *
 *  Created on: Aug 1, 2026
 *      Author: PC
 */
#include <USART.h>

void USART1_init(){
	RCC->APB2ENR |= (1<<14);
	USART1->CR1 = 0;
	USART1->CR2 = 0;
	USART1->CR3 = 0;

	//config baudrate
	USART1->BRR = 0x271;

	USART1->CR1 = (1<<3)|(1<<2);
	USART1->CR1 |= (1<<13);
}

void UART_transmit(char*arr){
	while(*arr){
		while(!(USART1->SR & (1<<7)));  //bit set khi đã đc gửi vào thanh ghi dịch
		USART1->DR = *arr++;
	}

	while(!(USART1->SR & (1<<6))); // truyền hoàn tất}

}

uint8_t UART_ReceiveByte(void){
    while( !(USART1->SR & (1<<5)) ); // chờ RXNE
    return (uint8_t)(USART1->DR & 0xFF);
}





