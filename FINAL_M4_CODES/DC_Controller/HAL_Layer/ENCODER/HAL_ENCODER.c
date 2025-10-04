/**
@file: HAL_ENCODER.c
@author: M4_TEAM
@creation_date: date
*/


/********************************INCLUDES SECTION START****************************************/
#include "HAL_ENCODER.h"
/********************************INCLUDES SECTION END******************************************/



/********************************DATA TYPES DEFINITIONS SECTION START**************************/
static Exti_Config_t interrupt_pin;
static Pin_Config_t digital_pin;
/********************************DATA TYPES DEFINITIONS SECTION END****************************/




/********************************HELPER FUNCTIONS PROTOTYPES SECTION START*********************/

/********************************HELPER FUNCTIONS PROTOTYPES SECTION END***********************/





/********************************FUNCTIONS DEFINITIONS(APIS) SECTION START*********************/
Std_ReturnType_t Encoder_init(Encoder_t *Encoder_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==Encoder_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		
			digital_pin.port_index=Encoder_ptr->gpio_port_index;
			digital_pin.pin_index=Encoder_ptr->gpio_pin_index;
			digital_pin.mode=INPUT_MODE;
			digital_pin.conf=FLOATING_INPUT;
		
			interrupt_pin.gpio_pin.port_index=Encoder_ptr->Exti_port_index;
			interrupt_pin.gpio_pin.pin_index=Encoder_ptr->Exti_pin_index;
			interrupt_pin.mode=RISING_EDGE;
			interrupt_pin.InterruptHandler=Encoder_ptr->Encoder_Interrupt;
			retval=gpio_pin_init(&digital_pin);
			retval=EXTI_init(&interrupt_pin);
	
	}
	return retval;
}

//aa
Std_ReturnType_t Encoder_deinit(Encoder_t *Encoder_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==Encoder_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		
			digital_pin.port_index=Encoder_ptr->gpio_port_index;
			digital_pin.pin_index=Encoder_ptr->gpio_pin_index;
			digital_pin.mode=OUTPUT_MODE_2;
			digital_pin.conf=OUTPUT_PP;
		
		
			interrupt_pin.gpio_pin.port_index=Encoder_ptr->Exti_port_index;
			interrupt_pin.gpio_pin.pin_index=Encoder_ptr->Exti_pin_index;
			interrupt_pin.mode=RISING_EDGE;
			interrupt_pin.InterruptHandler=Encoder_ptr->Encoder_Interrupt;
			retval=gpio_pin_init(&digital_pin);
			retval=EXTI_deinit(&interrupt_pin);
	
	}
	return retval;
}
/********************************FUNCTIONS DEFINITIONS (APIS) SECTION END**********************/




/********************************HELPER FUNCTIONS DEFINITIONS SECTION START********************/

/********************************HELPER FUNCTIONS DEFINITIONS SECTION END**********************/



