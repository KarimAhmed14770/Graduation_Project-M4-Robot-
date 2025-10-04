/**
@file: MCAL_EXTERNAL_INTERRUPTS.c
@author: M4_TEAM
@creation_date: date
*/


/********************************INCLUDES SECTION START****************************************/
#include "MCAL_EXTERNAL_INTERRUPTS.h"
/********************************INCLUDES SECTION END******************************************/



/********************************DATA TYPES DEFINITIONS SECTION START**************************/
void (*EXTI0_InterruptHandler)(void)=NULL;
void (*EXTI1_InterruptHandler)(void)=NULL;
void (*EXTI2_InterruptHandler)(void)=NULL;
void (*EXTI3_InterruptHandler)(void)=NULL;
void (*EXTI4_InterruptHandler)(void)=NULL;
void (*EXTI5_InterruptHandler)(void)=NULL;
void (*EXTI6_InterruptHandler)(void)=NULL;
void (*EXTI7_InterruptHandler)(void)=NULL;
void (*EXTI8_InterruptHandler)(void)=NULL;
void (*EXTI9_InterruptHandler)(void)=NULL;
void (*EXTI10_InterruptHandler)(void)=NULL;
void (*EXTI11_InterruptHandler)(void)=NULL;
void (*EXTI12_InterruptHandler)(void)=NULL;
void (*EXTI13_InterruptHandler)(void)=NULL;
void (*EXTI14_InterruptHandler)(void)=NULL;
void (*EXTI15_InterruptHandler)(void)=NULL;

/********************************DATA TYPES DEFINITIONS SECTION END****************************/




/********************************HELPER FUNCTIONS PROTOTYPES SECTION START*********************/
static Std_ReturnType_t extix_disable(Exti_Config_t *Exti_ptr);
static Std_ReturnType_t exti_pin_init(Exti_Config_t *Exti_ptr);
static Std_ReturnType_t exti_pin_set_mode(Exti_Config_t *Exti_ptr);
static Std_ReturnType_t extix_enable(Exti_Config_t *Exti_ptr);
static Std_ReturnType_t extix_set_interrupt_handler(Exti_Config_t *Exti_ptr);
/********************************HELPER FUNCTIONS PROTOTYPES SECTION END***********************/





/********************************FUNCTIONS DEFINITIONS(APIS) SECTION START*********************/
Std_ReturnType_t EXTI_init(Exti_Config_t *Exti_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==Exti_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		//disable exti interrupt line and disable nvic interrupt line
			//retval=extix_disable(Exti_ptr);
		//enable afio clock
			RCC_AFIO_CLK_ENABLE();
		//initialize gpio pin and setmode
			retval=exti_pin_init(Exti_ptr);
		//set mode (rising or falling edge)
		  retval=exti_pin_set_mode(Exti_ptr);
		//initialize callback
		  retval=extix_set_interrupt_handler(Exti_ptr);
		//enable exti line and enable nvic
		  retval=extix_enable(Exti_ptr);
	}
	return retval;
}
Std_ReturnType_t EXTI_deinit(Exti_Config_t *Exti_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==Exti_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		retval=extix_disable(Exti_ptr);
	}
	return retval;
}
/********************************FUNCTIONS DEFINITIONS (APIS) SECTION END**********************/




/********************************HELPER FUNCTIONS DEFINITIONS SECTION START********************/
static Std_ReturnType_t extix_disable(Exti_Config_t *Exti_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==Exti_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		CLEAR_BIT_K(EXTI_IMR,Exti_ptr->gpio_pin.pin_index);
		if(Exti_ptr->gpio_pin.pin_index==PIN0)
		{
			NVIC_EXTI0_DISABLE();
		}
		else if(Exti_ptr->gpio_pin.pin_index==PIN1)
		{
			NVIC_EXTI1_DISABLE();
		}
		else if(Exti_ptr->gpio_pin.pin_index==PIN2)
		{
			NVIC_EXTI2_DISABLE();
		}
		else if(Exti_ptr->gpio_pin.pin_index==PIN3)
		{
			NVIC_EXTI3_DISABLE();
		}
		else if(Exti_ptr->gpio_pin.pin_index==PIN4)
		{
			NVIC_EXTI4_DISABLE();
		}
		else if(Exti_ptr->gpio_pin.pin_index>=PIN5 &&Exti_ptr->gpio_pin.pin_index<=PIN9)
		{
			NVIC_EXTI5_9_DISABLE();
		}
		else if(Exti_ptr->gpio_pin.pin_index>=PIN10 &&Exti_ptr->gpio_pin.pin_index<=PIN15)
		{
			NVIC_EXTI15_10_DISABLE();
		}
		else
		{
			retval=E_NOT_OK;
		}
		
	}
	return retval;
}
static Std_ReturnType_t exti_pin_init(Exti_Config_t *Exti_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==Exti_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		Exti_ptr->gpio_pin.mode=INPUT_MODE;
		Exti_ptr->gpio_pin.conf=FLOATING_INPUT;
		retval=gpio_pin_init(&Exti_ptr->gpio_pin);
		switch(Exti_ptr->gpio_pin.port_index)
		{
			case PORTA:
				switch(Exti_ptr->gpio_pin.pin_index)
				{
					case PIN0:
					EXTI0_PORTA_SELECT();
					break;
					
					case PIN1:
					EXTI1_PORTA_SELECT();
					break;
					
					case PIN2:
					EXTI2_PORTA_SELECT();
					break;
					
					case PIN3:
					EXTI3_PORTA_SELECT();
					break;
					
					case PIN4:
					EXTI4_PORTA_SELECT();
					break;
					
					case PIN5:
					EXTI5_PORTA_SELECT();
					break;
					
					case PIN6:
					EXTI6_PORTA_SELECT();
					break;
					
					case PIN7:
					EXTI7_PORTA_SELECT();
					break;
					
					case PIN8:
					EXTI8_PORTA_SELECT();
					break;
					
					case PIN9:
					EXTI9_PORTA_SELECT();
					break;
					
					case PIN10:
					EXTI10_PORTA_SELECT();
					break;
					
					case PIN11:
					EXTI11_PORTA_SELECT();
					break;
					
					case PIN12:
					EXTI12_PORTA_SELECT();
					break;
					
					case PIN13:
					EXTI13_PORTA_SELECT();
					break;
					
					case PIN14:
					EXTI14_PORTA_SELECT();
					break;
					
					case PIN15:
					EXTI15_PORTA_SELECT();
					break;
					
					default: retval=E_NOT_OK;
				}
			break;
			
			case PORTB:
				switch(Exti_ptr->gpio_pin.pin_index)
				{
					case PIN0:
					EXTI0_PORTB_SELECT();
					break;
					
					case PIN1:
					EXTI1_PORTB_SELECT();
					break;
					
					case PIN2:
					EXTI2_PORTB_SELECT();
					break;
					
					case PIN3:
					EXTI3_PORTB_SELECT();
					break;
					
					case PIN4:
					EXTI4_PORTB_SELECT();
					break;
					
					case PIN5:
					EXTI5_PORTB_SELECT();
					break;
					
					case PIN6:
					EXTI6_PORTB_SELECT();
					break;
					
					case PIN7:
					EXTI7_PORTB_SELECT();
					break;
					
					case PIN8:
					EXTI8_PORTB_SELECT();
					break;
					
					case PIN9:
					EXTI9_PORTB_SELECT();
					break;
					
					case PIN10:
					EXTI10_PORTB_SELECT();
					break;
					
					case PIN11:
					EXTI11_PORTB_SELECT();
					break;
					
					case PIN12:
					EXTI12_PORTB_SELECT();
					break;
					
					case PIN13:
					EXTI13_PORTB_SELECT();
					break;
					
					case PIN14:
					EXTI14_PORTB_SELECT();
					break;
					
					case PIN15:
					EXTI15_PORTB_SELECT();
					break;
					
					default: retval=E_NOT_OK;
				}
			break;
			
			case PORTC:
				switch(Exti_ptr->gpio_pin.pin_index)
				{
					case PIN0:
					EXTI0_PORTC_SELECT();
					break;
					
					case PIN1:
					EXTI1_PORTC_SELECT();
					break;
					
					case PIN2:
					EXTI2_PORTC_SELECT();
					break;
					
					case PIN3:
					EXTI3_PORTC_SELECT();
					break;
					
					case PIN4:
					EXTI4_PORTC_SELECT();
					break;
					
					case PIN5:
					EXTI5_PORTC_SELECT();
					break;
					
					case PIN6:
					EXTI6_PORTC_SELECT();
					break;
					
					case PIN7:
					EXTI7_PORTC_SELECT();
					break;
					
					case PIN8:
					EXTI8_PORTC_SELECT();
					break;
					
					case PIN9:
					EXTI9_PORTC_SELECT();
					break;
					
					case PIN10:
					EXTI10_PORTC_SELECT();
					break;
					
					case PIN11:
					EXTI11_PORTC_SELECT();
					break;
					
					case PIN12:
					EXTI12_PORTC_SELECT();
					break;
					
					case PIN13:
					EXTI13_PORTC_SELECT();
					break;
					
					case PIN14:
					EXTI14_PORTC_SELECT();
					break;
					
					case PIN15:
					EXTI15_PORTC_SELECT();
					break;
					
					default: retval=E_NOT_OK;
				}
			break;
			
			default:retval=E_NOT_OK;
			
		}
	}
	return retval;
}

static Std_ReturnType_t exti_pin_set_mode(Exti_Config_t *Exti_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==Exti_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		switch(Exti_ptr->mode){
			case RISING_EDGE:
				SET_BIT_K(EXTI_RTSR,Exti_ptr->gpio_pin.pin_index);
			break;
			
			case FALLING_EDGE:
				SET_BIT_K(EXTI_FTSR,Exti_ptr->gpio_pin.pin_index);
			break;
			
			retval=E_NOT_OK;
		}
	}
	return retval;
}
static Std_ReturnType_t extix_enable(Exti_Config_t *Exti_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==Exti_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		SET_BIT_K(EXTI_IMR,Exti_ptr->gpio_pin.pin_index);
		if(Exti_ptr->gpio_pin.pin_index==PIN0)
		{
			NVIC_EXTI0_ENABLE();
		}
		else if(Exti_ptr->gpio_pin.pin_index==PIN1)
		{
			NVIC_EXTI1_ENABLE();
		}
		else if(Exti_ptr->gpio_pin.pin_index==PIN2)
		{
			NVIC_EXTI2_ENABLE();
		}
		else if(Exti_ptr->gpio_pin.pin_index==PIN3)
		{
			NVIC_EXTI3_ENABLE();
		}
		else if(Exti_ptr->gpio_pin.pin_index==PIN4)
		{
			NVIC_EXTI4_ENABLE();
		}
		else if(Exti_ptr->gpio_pin.pin_index>=PIN5 &&Exti_ptr->gpio_pin.pin_index<=PIN9)
		{
			NVIC_EXTI5_9_ENABLE();
		}
		else if(Exti_ptr->gpio_pin.pin_index>=PIN10 &&Exti_ptr->gpio_pin.pin_index<=PIN15)
		{
			NVIC_EXTI15_10_ENABLE();
		}
		else
		{
			retval=E_NOT_OK;
		}
	}
	return retval;
}
static Std_ReturnType_t extix_set_interrupt_handler(Exti_Config_t *Exti_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==Exti_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		switch (Exti_ptr->gpio_pin.pin_index){
			case PIN0:
					EXTI0_InterruptHandler=Exti_ptr->InterruptHandler;
					break;
					
					case PIN1:
					EXTI1_InterruptHandler=Exti_ptr->InterruptHandler;
					break;
					
					case PIN2:
					EXTI2_InterruptHandler=Exti_ptr->InterruptHandler;
					break;
					
					case PIN3:
					EXTI3_InterruptHandler=Exti_ptr->InterruptHandler;
					break;
					
					case PIN4:
					EXTI4_InterruptHandler=Exti_ptr->InterruptHandler;
					break;
					
					case PIN5:
					EXTI5_InterruptHandler=Exti_ptr->InterruptHandler;
					break;
					
					case PIN6:
					EXTI6_InterruptHandler=Exti_ptr->InterruptHandler;
					break;
					
					case PIN7:
					EXTI7_InterruptHandler=Exti_ptr->InterruptHandler;
					break;
					
					case PIN8:
					EXTI8_InterruptHandler=Exti_ptr->InterruptHandler;
					break;
					
					case PIN9:
					EXTI9_InterruptHandler=Exti_ptr->InterruptHandler;
					break;
					
					case PIN10:
					EXTI10_InterruptHandler=Exti_ptr->InterruptHandler;
					break;
					
					case PIN11:
					EXTI11_InterruptHandler=Exti_ptr->InterruptHandler;
					break;
					
					case PIN12:
					EXTI12_InterruptHandler=Exti_ptr->InterruptHandler;
					break;
					
					case PIN13:
					EXTI13_InterruptHandler=Exti_ptr->InterruptHandler;
					break;
					
					case PIN14:
					EXTI14_InterruptHandler=Exti_ptr->InterruptHandler;
					break;
					
					case PIN15:
					EXTI15_InterruptHandler=Exti_ptr->InterruptHandler;
					break;
					
					default: retval=E_NOT_OK;
		}
	}
	return retval;
}
/********************************HELPER FUNCTIONS DEFINITIONS SECTION END**********************/

/***************ISR definitions************************/
void EXTI0_IRQHandler(void){
	
	NVIC_EXTI0_CLEAR_FLAG();
	EXTI0_CLEAR_FLAG();
	if(EXTI0_InterruptHandler)
	{
		EXTI0_InterruptHandler();
	}

}

void EXTI1_IRQHandler(void){
	
	NVIC_EXTI1_CLEAR_FLAG();
	EXTI1_CLEAR_FLAG();
	if(EXTI1_InterruptHandler)
	{
		EXTI1_InterruptHandler();
	}

}

void EXTI2_IRQHandler(void){
	
	NVIC_EXTI2_CLEAR_FLAG();
	EXTI2_CLEAR_FLAG();
	if(EXTI2_InterruptHandler)
	{
		EXTI2_InterruptHandler();
	}

}


void EXTI3_IRQHandler(void){
	
	NVIC_EXTI3_CLEAR_FLAG();
	EXTI3_CLEAR_FLAG();
	if(EXTI3_InterruptHandler)
	{
		EXTI3_InterruptHandler();
	}

}
void EXTI4_IRQHandler(void){
	
	NVIC_EXTI4_CLEAR_FLAG();
	EXTI4_CLEAR_FLAG();
	if(EXTI4_InterruptHandler)
	{
		EXTI4_InterruptHandler();
	}

}

void EXTI9_5_IRQHandler(void){
		
	NVIC_EXTI5_9_CLEAR_FLAG();
	if(EXTI5_READ_FLAG())
	{
		EXTI5_CLEAR_FLAG();
		if(EXTI5_InterruptHandler)
		{
			EXTI5_InterruptHandler();
		}
	}
	else if(EXTI6_READ_FLAG())
	{
		EXTI6_CLEAR_FLAG();
		if(EXTI6_InterruptHandler)
		{
			EXTI6_InterruptHandler();
		}
	}
	else if(EXTI7_READ_FLAG())
	{
		EXTI7_CLEAR_FLAG();
		if(EXTI7_InterruptHandler)
		{
			EXTI7_InterruptHandler();
		}
	}
	else if(EXTI8_READ_FLAG())
	{
		EXTI8_CLEAR_FLAG();
		if(EXTI8_InterruptHandler)
		{
			EXTI8_InterruptHandler();
		}
	}
	else if(EXTI9_READ_FLAG())
	{
		EXTI9_CLEAR_FLAG();
		if(EXTI9_InterruptHandler)
		{
			EXTI9_InterruptHandler();
		}
	}
}
	
	void EXTI15_10_IRQHandler(void){
	
		NVIC_EXTI15_10_CLEAR_FLAG();
	if(EXTI10_READ_FLAG())
	{
		EXTI10_CLEAR_FLAG();
		if(EXTI10_InterruptHandler)
		{
			EXTI10_InterruptHandler();
		}
	}
	else if(EXTI11_READ_FLAG())
	{
		EXTI11_CLEAR_FLAG();
		if(EXTI11_InterruptHandler)
		{
			EXTI11_InterruptHandler();
		}
	}
	else if(EXTI12_READ_FLAG())
	{
		EXTI12_CLEAR_FLAG();
		if(EXTI12_InterruptHandler)
		{
			EXTI12_InterruptHandler();
		}
	}
	else if(EXTI13_READ_FLAG())
	{
		EXTI13_CLEAR_FLAG();
		if(EXTI13_InterruptHandler)
		{
			EXTI13_InterruptHandler();
		}
	}
	else if(EXTI14_READ_FLAG())
	{
		EXTI14_CLEAR_FLAG();
		if(EXTI14_InterruptHandler)
		{
			EXTI14_InterruptHandler();
		}
	}
	else if(EXTI15_READ_FLAG())
	{
		EXTI15_CLEAR_FLAG();
		if(EXTI15_InterruptHandler)
		{
			EXTI15_InterruptHandler();
		}
	}

}
