/**
@file: MCAL_UART.h
@author: M4_TEAM 
@creation_date: 25/4/2024
*/


#ifndef MCAL_UART_H
#define MCAL_UART_H

/*******************INCLUDES SECTION START************************/
#include "../GPIO/MCAL_GPIO.h"
/*******************INCLUDES SECTION END  ************************/


unsigned short bauderateCalculator(unsigned short usart, unsigned long baude_rate);

void USART_Init(unsigned short usart, unsigned long baude_rate);

char USART_RX(unsigned short uart);

void USART_TX(unsigned short uart, char c);


#endif
