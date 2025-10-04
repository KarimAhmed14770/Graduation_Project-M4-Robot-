#include "MCAL_UART.h"
#include "../GPIO/MCAL_GPIO.h"

static void(*USART1_Handler)(void) = NULL;
static void(*USART2_Handler)(void) = NULL;
static void(*USART3_Handler)(void) = NULL;





void USART_Init(unsigned short usart, unsigned long baude_rate)
{
	Std_ReturnType_t err_status=0;
	//Enable the Alternate function for PINS
	//RCC->APB2ENR |= 1;
	//clock will differ from USART1 (36mhz) and USART2andUSART3(72mhz)
	//USART1 PA9 (TX)		&	PA10 (RX)
	//USART2 PA2(TX) & PA3(RX) 
	//USART3 PB10(TX) & PB11(RX)
	//BAUD RATE CALCULATIONS
		unsigned short BaudeRateReg;
		BaudeRateReg = bauderateCalculator(usart,baude_rate);
	//--------Init UART-------//
	
	
	//Enable UART
	if(usart == 1)
	{
	Pin_Config_t pinA9={
		.port_index=PORTA,
		.pin_index=PIN9,
		.mode=OUTPUT_MODE_50,
		.conf=AFO_PP,
		.logic=LOW
	};
		
	Pin_Config_t pinA10={
		.port_index=PORTA,
		.pin_index=PIN10,
		.mode=INPUT_MODE,
		.conf=FLOATING_INPUT,
		.logic=LOW
	};
		
	//ENABLE USART 1
		err_status=gpio_pin_init(&pinA9); //tx
		err_status=gpio_pin_init(&pinA10); //rx
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	//Setup the baude rate(9600)
		USART1_BRR=BaudeRateReg;
	//Enable 						USART, 				Transmit, 		 RECIEVE		TXE interrupt enable  Transmission complete
		USART1_CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE; 
		//USART1_CR1 |= USART_CR1_RXNEIE | USART_CR1_TXEIE;
		
		//SET_BIT_K(NVIC_ISER1,BIT5);
	}else if (usart == 2)
	{
	//ENABLE USART 2
	Pin_Config_t pinA2={
		.port_index=PORTA,
		.pin_index=PIN2,
		.mode=OUTPUT_MODE_50,
		.conf=AFO_PP,
		.logic=LOW
	};
		
	Pin_Config_t pinA3={
		.port_index=PORTA,
		.pin_index=PIN3,
		.mode=INPUT_MODE,
		.conf=FLOATING_INPUT,
		.logic=LOW
	};
		err_status=gpio_pin_init(&pinA2); //tx
		err_status=gpio_pin_init(&pinA3); //rx
		
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
		//RCC_USART2_CLK_ENABLE();

	//Setup the baude rate(9600)
		USART2_BRR = BaudeRateReg;
	//Enable 						USART, 				Transmit, 		 RECIEVE
		USART2_CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE; 
		//USART2_CR1 |= USART_CR1_RXNEIE | USART_CR1_TXEIE;
		//SET_BIT_K(NVIC_ISER1,BIT6);
	
	}
	else if(usart == 3)
	{
		Pin_Config_t pinB10={
			.port_index=PORTB,
			.pin_index=PIN10,
			.mode=OUTPUT_MODE_50,
			.conf=AFO_PP,
			.logic=LOW
};
		Pin_Config_t pinB11={
			.port_index=PORTB,
			.pin_index=PIN11,
			.mode=INPUT_MODE,
			.conf=FLOATING_INPUT,
			.logic=HIGH
};
	// ENABLE USART3
		err_status=gpio_pin_init(&pinB10); //tx
		err_status=gpio_pin_init(&pinB11); //rx
		RCC->APB1ENR |= RCC_APB1ENR_USART3EN; ///ENABLE UART 3
		//RCC_USART3_CLK_ENABLE();

	//Setup the baude rate(9600)
		USART3_BRR = BaudeRateReg;
	//Enable 						USART, 				Transmit, 		 RECIEVE
		USART3_CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
		//USART3_CR1 |= USART_CR1_RXNEIE | USART_CR1_TXEIE;

		//SET_BIT_K(NVIC_ISER1,BIT7);
	}
	
}

unsigned short bauderateCalculator(unsigned short usart, unsigned long baude_rate)
{
	unsigned long div = 36000000;
	unsigned long dec;
	unsigned short final;
	double frac = 36000000;
	double frac1 = 1.0;
	
	if (usart == 1)
	{
		div = 72000000;
		frac = 72000000;
	}
	div = div / (baude_rate*16);
	frac = frac/(baude_rate*16);
	frac = 16*(frac-div);
	dec = frac;
	frac1 = 100 *(frac - dec);
	
	if(frac1 >50)
	{
		dec++;
		if (dec == 16)
		{
			dec = 0;
			div++;
		}
	}
	final = (div<<4);
	final += dec;
	
	return final;
}



char USART_RX(unsigned short uart)
{
	char c = ' ';
	if(uart == 1)
	{
		while(USART1_SR & USART_SR_RXNE)
		{
			c = USART1_DR;
		}
	} else if (uart == 2)
	{
		while(USART2_SR & USART_SR_RXNE)
		{
			c = USART2_DR;
		}
		
	}else if(uart == 3)
	{
		while(USART3_SR & USART_SR_RXNE)
		{
			c = USART3_DR;
		}
	}
	return c;
}




void USART_TX(unsigned short uart, char c)
{
	if(uart == 1)
	{
		while(!(USART1_SR & USART_SR_TC))
		{}
		USART1_DR = c;		
	} else if (uart == 2)
	{
		while(!(USART2_SR & USART_SR_TC))
		{}
		USART2_DR = c;
	}else if(uart == 3)
	{
		while(!(USART3_SR & USART_SR_TC))
		{}
		USART3_DR = c;
	}
}



void USART1_IRQHandler(void)
{
	if(USART1_SR & USART_SR_RXNE)
	{
		if(USART1_Handler)
		{
			USART1_Handler();
		}
	}
	//Clear the flags
	SET_BIT_K(NVIC_ICPR1, BIT5);
	CLEAR_BIT_K(USART1_SR, BIT5);
}


void USART2_IRQHandler(void)
{
		if(USART2_SR & USART_SR_RXNE)
	{
		if(USART2_Handler)
		{
			USART2_Handler();
		}
	}
	//Clear the flags
	SET_BIT_K(NVIC_ICPR1, BIT6);
	CLEAR_BIT_K(USART2_SR, BIT5);
}


void USART3_IRQHandler(void)
{
		if(USART3_SR & USART_SR_RXNE)  //0x0020
	{
		if(USART3_Handler)
		{
			USART3_Handler();
		}
	}
	//Clear the flags
	SET_BIT_K(NVIC_ICPR1, BIT7);
	CLEAR_BIT_K(USART3_SR, BIT5);
}


void USART_Set_IRH(unsigned short usart, void (*ISR)(void))
{
	if(usart == 1)
	{
		USART1_Handler = ISR;
	}else if (usart == 2)
	{
		USART2_Handler = ISR;
	}else if(usart == 3)
	{
		USART3_Handler = ISR;
	}
}


